#include <bn_keypad.h>
#include <bn_log.h>

#include "play_scene.h"

namespace aru {

play_scene::play_scene(common_stuff &cstuff, const level_data &ld) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(256,192)),
    _level(_cam, ld.map),
    _player(_cam,ld.spawnpoint.x(),ld.spawnpoint.y(),_level),
    _cam_mgr(_cam, _level, _player),
    _hud(_cstuff.savefile.ultramatter),
    _bg(ld.backdrop.create_bg(0,0)) {

    _bg.set_z_order(1);
}

bn::optional<scene_type> play_scene::update(){
    bn::optional<scene_type> result;

    _player.update();
    _cam_mgr.update();
    _hud.update();

    return result;
}

}