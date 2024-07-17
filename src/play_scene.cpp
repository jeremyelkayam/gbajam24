#include <bn_keypad.h>
#include <bn_log.h>

#include "play_scene.h"

namespace aru {

play_scene::play_scene(common_stuff &cstuff, const level_data &ld) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(256,192)),
    _level(_cam, ld.map),
    _player(_cam,ld.spawnpoint.x(),ld.spawnpoint.y(),_level, cstuff.savefile),
    _cam_mgr(_cam, _level, _player),
    _hud(_cstuff),
    _bg(ld.backdrop.create_bg(0,0)) {

    _bg.set_z_order(1);
    _bg.set_blending_enabled(true);
    _player.set_blending_enabled(true);
    _level.set_blending_enabled(true);
    _hud.set_blending_enabled(true);


    _bg.set_mosaic_enabled(true);
    _player.set_mosaic_enabled(true);
    _level.set_mosaic_enabled(true);
    _hud.set_mosaic_enabled(true);
}

bn::optional<scene_type> play_scene::update(){
    bn::optional<scene_type> result;

    _player.update();
    _cam_mgr.update();
    _hud.update();
    _hud.update_hover_time(_player.hover_time());
    _hud.update_player_hp(_player.hp());

    return result;
}

}