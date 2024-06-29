#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>
#include "bn_regular_bg_items_lab.h"
#include "bn_regular_bg_items_testbg.h"

#include "lab_scene.h"

namespace aru {

lab_scene::lab_scene() :
    _cam(bn::camera_ptr::create(128,128)),
    _level(_cam, bn::regular_bg_items::lab),
    _player(_cam,128,128,_level),
    _bg(bn::regular_bg_items::testbg.create_bg(0,0)){
    _bg.set_z_order(1);
    
}

bn::optional<scene_type> lab_scene::update(){
    bn::optional<scene_type> result;
    _player.update();

    return result;
}

}