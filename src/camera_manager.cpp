#include "camera_manager.h"

#include <bn_keypad.h>
#include <bn_log.h>

namespace aru {

camera_manager::camera_manager(bn::camera_ptr cam, const level &lv, const player &pl) :
    _cam(cam),
    _level(lv),
    _player(pl),
    _x_ease(_cam.x() - _player.x(), 50 * (_player.facing_right() ? 1 : -1), 30),
    _y_ease(_cam.x() - _player.x(), _cam.x() - _player.x(), 30) {

}

void camera_manager::update(){

    _x_ease.set_target(50 * (_player.facing_right() ? 1 : -1));


    if(_player.on_flat_ground()){
        
    }else{

    }


    _x_ease.update();
    _y_ease.update();

    bn::fixed new_x = _player.x() + _x_ease.current();
    bn::fixed new_y = _player.y() + _y_ease.current();
    if(new_x < 120) new_x = 120;
    if(new_x > (_level.width() - 120)) new_x = _level.width() - 120;
    if(new_y < 88) new_y = 88;
    if(new_y > (_level.height() - 80)) new_y = _level.height() - 80;
    _cam.set_position(new_x, new_y);
}

camera_manager::easer::easer(const bn::fixed &start, const bn::fixed &target, 
    const uint8_t &ease_time) : 
        _ease_time(ease_time),
        _timer(0),
        _start(start),
        _target(target)
{

}

void camera_manager::easer::set_target(const bn::fixed &target)
{
    if(target != _target){
        _start = current();
        _target = target;
        _timer = 0;
    }
}

void camera_manager::easer::update()
{
    if(_timer < _ease_time) ++_timer;
}

bn::fixed camera_manager::easer::current(){
    return _start + (_target - _start) * (bn::fixed(_timer) / bn::fixed(_ease_time));
}


}