#include "camera_manager.h"

#include <bn_keypad.h>
#include <bn_log.h>

namespace aru {

camera_manager::camera_manager(bn::camera_ptr cam, const level &lv, const player &pl) :
    _cam(cam),
    _view(cam.x(), cam.y(), 240, 160),
    _level(lv),
    _player(pl),
    _shake_timer(0),
    _x_ease(_cam.x() - _player.x(), 50 * (_player.facing_right() ? 1 : -1), 60,
        easer::sine_ease_out),
    _y_ease(_cam.x() - _player.x(), _cam.x() - _player.x(), 120, easer::sine_ease_out) 
{

}

void camera_manager::update(){

    if(_fixed_target){
        _x_ease.set_target(_player.x() - _fixed_target->x());
        _y_ease.set_target(_player.y() - _fixed_target->y());
    }else{

    

        _x_ease.set_target(50 * (_player.facing_right() ? 1 : -1));


        switch(_player.state()){
            case PSTATE::HOVER:
                _y_ease.set_target(-50);
            break;
            case PSTATE::FALL:
                if(_player.time_since_state_change() > 30){
                    _y_ease.set_target(50);
                }
            break;
            case PSTATE::JUMP:
            break;
            default:
                _y_ease.set_target(-30);
        }
    }


    _x_ease.update();
    _y_ease.update();

    bn::fixed new_x = _player.x() + _x_ease.current();
    bn::fixed new_y = _player.y() + _y_ease.current();
    if(new_x < 120) new_x = 120;
    if(new_x > (_level.width() - 120)) new_x = _level.width() - 120;

    //don't go past the top row of tiles
    if(new_y < 88) new_y = 88;
    if(new_y > (_level.height() - 80)) new_y = _level.height() - 80;

    if(_shake_timer != 0){
        ++_shake_timer;
        if(_shake_timer == 2){
            new_y += 1;
            _shake_timer = 1;
        }else{
            new_y -= 1;
        }
    }

    _cam.set_position(new_x, new_y);
    _view.set_position(new_x, new_y);
}

bool camera_manager::on_screen(bn::fixed_rect &hitbox){
    return _view.intersects(hitbox);
}

void camera_manager::set_fixed_target(const bn::fixed_point &target){
    _fixed_target = target;
}

}
