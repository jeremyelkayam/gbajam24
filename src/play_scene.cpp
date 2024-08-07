#include <bn_keypad.h>
#include <bn_log.h>

#include "play_scene.h"

namespace aru {

play_scene::play_scene(common_stuff &cstuff, const level_data &ld) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(256,192)),
    _level(_cam, ld.map),
    _player(_cam,ld.spawnpoint.x(),ld.spawnpoint.y(),_level, cstuff.current_save(), 
        cstuff.rising_text_generator),
    _cam_mgr(_cam, _level, _player),
    _hud(_cstuff),
    _bg(ld.backdrop.create_bg(0,0)) {

    _bg.set_z_order(1);
    _cam_mgr.update();

    _warping.emplace(_player, warp_effect::direction::IN);
    _warping->update();

    update_bg_pos();    

    ld.music.play(0.8);
}

void play_scene::set_transition_effects_enabled(bool enabled){
    _bg.set_blending_enabled(enabled);
    _player.set_blending_enabled(enabled);
    _level.set_blending_enabled(enabled);
    _hud.set_blending_enabled(enabled);


    _bg.set_mosaic_enabled(enabled);
    _player.set_mosaic_enabled(enabled);
    _level.set_mosaic_enabled(enabled);
    _hud.set_mosaic_enabled(enabled);    
}

void play_scene::set_visible(bool visible){
    _bg.set_visible(visible);
    _player.set_visible(visible);
    _level.set_visible(visible);
    _hud.set_visible(visible);
}

bn::optional<scene_type> play_scene::update_scene_components(){
    bn::optional<scene_type> result;
    update_bg_pos();


    if(_warping){
        _warping->update();
        if(_warping->done()){
            if(_warping->get_direction() == warp_effect::direction::OUT){
                result = scene_type::LEVEL;
            }
            _warping.reset();
        }        
    }else{
        _cam_mgr.update();
        _player.update();
        _hud.update();
        _hud.update_hover_time(_player.hover_time());
        _hud.update_player_hp(_player.hp());
    }


    if(bn::keypad::l_pressed())
    {
        bool &slash_on_b = _cstuff.current_save().slash_on_b;
        if(_cstuff.current_save().slash_on_b)
        {
            slash_on_b = false;
        }
        else
        {
            slash_on_b = true;
        }
        
        BN_LOG("swapping. slash on b: ", slash_on_b);
    }


    return result;
}

void play_scene::update_bg_pos()
{
    bn::fixed bg_leftbound = -128;
    bn::fixed bg_width = 256;

    bn::fixed pct_traveled = (_cam.x() - 120) / (_level.width() - 240);

    _bg.set_x(bg_leftbound - pct_traveled*(bg_width - 240));
}

bn::optional<scene_type> play_scene::update()
{

    ++_cstuff.current_save().playtime;

    if(_pause_info){
        if(_pause_info->update()){
            hide_pause_info();
        }
    }else if(_pause_menu){
        bn::optional<scene_type> result = _pause_menu->update();
        if(result){
            if(result == scene_type::BACK){
                hide_pause_menu();
            }else{
                return result;
            }
        }
    }else{
        return update_scene_components();
    }

    return bn::optional<scene_type>();
}

void play_scene::show_pause_info(){
    BN_ASSERT(!_pause_info);
    BN_ASSERT(!_pause_menu);
    if(!_warping){
        BN_LOG("NO PAUSING DURING WARP ANIMATION");
        _player.set_blending_enabled(false);
        _pause_info.emplace(_cstuff.text_generator, _cstuff.current_save());
    }
}

void play_scene::hide_pause_info(){
    BN_ASSERT(_pause_info);
    BN_ASSERT(!_pause_menu);
    _pause_info.reset();
}

void play_scene::show_pause_menu(){
    BN_ASSERT(!_pause_info);
    BN_ASSERT(!_pause_menu);
    set_transition_effects_enabled(true);
    _pause_menu.emplace(_cstuff.text_generator);
}

void play_scene::hide_pause_menu(){
    BN_ASSERT(!_pause_info);
    BN_ASSERT(_pause_menu);
    set_transition_effects_enabled(false);
    _pause_menu.reset();
}

}