#include <bn_keypad.h>
#include <bn_log.h>

#include "play_scene.h"

namespace aru {

play_scene::play_scene(common_stuff &cstuff, const level_data &ld) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(256,192)),
    _level(_cam, ld.map),
    _player(_cam,ld.spawnpoint.x(),ld.spawnpoint.y(),_level, cstuff.savefile, 
        cstuff.rising_text_generator),
    _cam_mgr(_cam, _level, _player),
    _hud(_cstuff),
    _bg(ld.backdrop.create_bg(0,0)) {

    _bg.set_z_order(1);
    _cam_mgr.update();
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

bn::optional<scene_type> play_scene::update_scene_components(){
    bn::optional<scene_type> result;


    if(_warping){
        _warping->update();
    }else{
        _player.update();
        _cam_mgr.update();
        _hud.update();
        _hud.update_hover_time(_player.hover_time());
        _hud.update_player_hp(_player.hp());
    }
    

    return result;
}

bn::optional<scene_type> play_scene::update()
{

    if(_pause_info){
        if(_pause_info->update()){
            hide_pause_info();
        }
    }else{
        return update_scene_components();
    }

    return bn::optional<scene_type>();
}

void play_scene::show_pause_info(){
    BN_ASSERT(!_pause_info);
    _pause_info.emplace(_cstuff.rising_text_generator, _cstuff.savefile);
}

void play_scene::hide_pause_info(){
    BN_ASSERT(_pause_info);
    _pause_info.reset();
}

}