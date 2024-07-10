#include <bn_keypad.h>
#include <bn_log.h>

#include "play_scene.h"

namespace aru {

play_scene::play_scene(common_stuff &cstuff, const bn::regular_bg_item &map, const bn::regular_bg_item &backdrop) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(128,128)),
    _level(_cam, map),
    _player(_cam,128,128,_level),
    _cam_mgr(_cam, _level, _player),
    _bg(backdrop.create_bg(0,0)) {

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