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

bn::optional<scene_type> level_scene::update_scene_components()
{
    if(_bg.blending_enabled()){
        set_transition_effects_enabled(false);
    }
    
    bn::optional<scene_type> result;
    
    for(bn::unique_ptr<combat_entity> &e : _enemies){
        uint8_t old_hp = e->hp();

        //todo: refactor this to be less terrible and have less repeated code!
        if(!_player.in_iframes() && _player.hitbox().intersects(e->hitbox())){
            bn::fixed hori_kb = 6 * (_player.facing_right() ? 1 : -1); 
            e->hit(_player.contact_damage(),hori_kb,-3);
            _hud.update_enemy_hp("GLOBLIN", old_hp, e->hp(), e->max_hp());
        }
        if(e->hitbox().intersects(_player.hitbox())){
            bn::fixed hori_kb = 6 * (_player.facing_right() ? -1 : 1); 
            _player.hit(e->contact_damage(),hori_kb,-3);
        }
        if(_player.check_bullet_collision(*e.get())){
            _hud.update_enemy_hp("GLOBLIN", old_hp, e->hp(), e->max_hp());
        }
        e->update();
    }
    _enemies.remove_if(enemy_deletable);

    result = play_scene::update_scene_components();


    if(_player.hp() == 0){
        BN_LOG("you died.");
        //todo: death animation
        result = scene_type::LEVEL;
    }
    if(bn::keypad::start_pressed()){
        show_pause_info();
    }else if(bn::keypad::select_pressed()){
        show_pause_menu();
    }

    return result;

}

}