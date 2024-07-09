#include "camera_manager.h"

#include <bn_keypad.h>
#include <bn_log.h>

namespace aru {

camera_manager::camera_manager(bn::camera_ptr cam, const level &lv, const player &pl) :
    _cam(cam),
    _level(lv),
    _player(pl) {

}

void camera_manager::update(){
    bn::fixed_point target_cam_offset(40 * (!_player.facing_right() ? 1 : -1), -30);
    bn::fixed xoffset = _cam.x() - _player.x();
    bn::fixed target_xoffset = 50 * (_player.facing_right() ? 1 : -1);
    bn::fixed cam_xspeed = _player.xspeed() * (_player.facing_right() ? bn::fixed(1.5) : bn::fixed(-1.5));


    if(bn::keypad::left_held() || bn::keypad::right_held()){
        if(xoffset < target_xoffset){
            xoffset += cam_xspeed;
        }else if(xoffset > target_xoffset){
            xoffset -= cam_xspeed;
        }
    }


    if(target_xoffset - cam_xspeed < xoffset && xoffset < target_xoffset + cam_xspeed){
        xoffset = target_xoffset;
    }

    bn::fixed yoffset = _cam.y() - _player.y();
    bn::fixed target_yoffset = _cam.y() - _player.y();
    bn::fixed cam_yspeed = 2;
    if(_player.on_flat_ground()){
    //             // BN_LOG(cam.y() - player.hitbox().bottom());
    //             // yoffset = player.hitbox().bottom();
    //             // target_yoffset = cam.y() - player.last_solid_ground();
    //             // if(yoffset > target_yoffset){
    //             //     yoffset += cam_yspeed;
    //             // }else if(yoffset < target_yoffset){
    //             //     yoffset -= cam_yspeed;
    //             // }
    }else{
        if(yoffset > 60){
            yoffset = 60;
        }else if(yoffset < -40){
            yoffset = -40;
        }

    }

    bn::fixed new_x = _player.x() + xoffset;
    bn::fixed new_y = _player.y() + yoffset;
    if(new_x < 120) new_x = 120;
    if(new_x > (_level.width() - 120)) new_x = _level.width() - 120;
    if(new_y < 88) new_y = 88;
    if(new_y > (_level.height() - 80)) new_y = _level.height() - 80;
    _cam.set_position(new_x, new_y);
}


}