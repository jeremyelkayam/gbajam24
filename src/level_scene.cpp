#include "level_scene.h"

#include <bn_log.h>
#include <bn_keypad.h>

namespace aru {

level_scene::level_scene(common_stuff &cstuff) :
    play_scene(cstuff, test_level_data)
{
    for(enemy_data e : test_level_data.enemies){
        if(e.enemy_type == 0) break;
        //enemies should only really spawn when they're close to being onscreen...
        // _enemy_data.emplace_back(e);
        _enemies.emplace_front(new enemy(_cam, e.coords.x(), e.coords.y(), _level, 
            _cstuff.rising_text_generator));
        BN_LOG("enemy type: ", e.enemy_type);
    }
}

void level_scene::set_transition_effects_enabled(bool enabled)
{
    play_scene::set_transition_effects_enabled(enabled);
    for(bn::unique_ptr<combat_entity> &e : _enemies){
        e->set_blending_enabled(enabled);
        e->set_mosaic_enabled(enabled);
    }
}

void level_scene::set_visible(bool visible)
{
    play_scene::set_visible(visible);
    for(bn::unique_ptr<combat_entity> &e : _enemies){
        e->set_visible(visible);
        e->set_visible(visible);
    }
}


bn::optional<scene_type> level_scene::update_scene_components()
{
    bn::optional<scene_type> result;

    if(_dying){
        _cam_mgr.update();
        if(_dying->done()){
            result = scene_type::LEVEL;
        }
        _dying->update();
        if(_dying->initial_fade_done()){
            set_visible(false);
            _player.set_visible(true);
            _player.set_effects_visible(false);
            _bg.set_visible(true);
            _bg.set_palette(common_stuff::monochrome_bg_palette(bn::color(0,0,0)));
            _bg.set_blending_enabled(false);
            _cam_mgr.set_screen_shake(true);
        }
    }else{
        if(_bg.blending_enabled()){
            set_transition_effects_enabled(false);
        }
        
        
        for(bn::unique_ptr<combat_entity> &e : _enemies){
            uint8_t old_hp = e->hp();
            if(old_hp != 0){
                //todo: refactor this to be less terrible and have less repeated code!
                //this can be part of the check enemy collision function tbh
                if(!_player.in_iframes() && _player.hitbox().intersects(e->hitbox())){
                    bn::fixed hori_kb = 6 * (_player.facing_right() ? 1 : -1); 
                    e->hit(_player.contact_damage(),hori_kb,-3);
                    _hud.update_enemy_hp("GLOBLIN", old_hp, e->hp(), e->max_hp());
                }
                if(e->hitbox().intersects(_player.hitbox())){
                    bn::fixed hori_kb = 6 * (_player.facing_right() ? -1 : 1); 
                    _player.hit(e->contact_damage(),hori_kb,-3);
                }
                if(_player.check_enemy_collision(*e.get())){
                    _hud.update_enemy_hp("GLOBLIN", old_hp, e->hp(), e->max_hp());
                }
            }
            e->update();
            if(e->just_exploded())
            {
                bn::fixed health_pct = bn::fixed(_player.hp()) / 
                    bn::fixed(_player.max_hp());

                bn::fixed health_drop_chance = 0.3 * (1 - health_pct);

                if(_cstuff.rand.get_fixed(1) < health_drop_chance)
                {
                    _health_pickups.emplace_front(new health_pickup(
                        _cam, e->x(), e->y(), _level, 5));
                }else{
                    _crcy_pickups.emplace_front(new currency_pickup(
                        _cam, e->x(), e->y(), _level, 10));

                }

            }
        }

        _enemies.remove_if(enemy_deletable);

        for(bn::unique_ptr<pickup> &p : _health_pickups)
        {
            p->update();
            if(p->hitbox().intersects(_player.hitbox())){
                _player.heal(p->collect());
            }
        }
        _health_pickups.remove_if(pickup_deletable);

        for(bn::unique_ptr<pickup> &p : _crcy_pickups)
        {
            p->update();
            if(p->hitbox().intersects(_player.hitbox())){
                uint16_t value = p->collect();
                _cstuff.current_save().ultramatter += value;
                _hud.update_currency(_cstuff.current_save().ultramatter);
                _player.pick_up_currency(value);
            }
        }
        _crcy_pickups.remove_if(pickup_deletable);
        
        result = play_scene::update_scene_components();

        if(_player.hp() == 0){
            _dying.emplace(_player, _cam);
            _cam_mgr.set_fixed_target(_player.pos());
            set_transition_effects_enabled(true);
            _player.set_blending_enabled(false);
            _player.set_effects_blending_enabled(true);
        }
        if(bn::keypad::start_pressed()){
            show_pause_info();
        }else if(bn::keypad::select_pressed()){
            show_pause_menu();
        }

    }

    return result;

}


}