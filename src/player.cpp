#include <bn_keypad.h>

#include "player.h"
#include "bn_sprite_items_arutest.h"

namespace aru {

player::player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, bn::fixed max_speed, bn::fixed accel) : 
    _body(bn::sprite_items::arutest.create_sprite(x,y)),
    _max_speed(max_speed),
    _accel(accel)
{
    _body.set_camera(cam);

}

void player::update(){
    take_button_input();
    if(_speed > _target_speed) {
        _speed -= _accel;
    }
    if(_speed < _target_speed) {
        _speed += _accel;
    }

    if(_target_speed - _accel < _speed && _speed < _target_speed + _accel){
        // our speed will jitter so we need to set it to the target
        _speed = _target_speed;
    }

    _body.set_x(_body.x() + _speed);

}

void player::take_button_input(){
    if(bn::keypad::left_held()){

        _body.set_horizontal_flip(false);
        _target_speed = -_max_speed;

    }else if(bn::keypad::right_held()){

        _body.set_horizontal_flip(true);
        _target_speed = _max_speed;

    }else {

        _target_speed = 0;

    }
}

bn::fixed player::get_new_speed(bn::fixed speed, bn::fixed max_speed, bn::fixed accel) const{
    BN_ASSERT(speed >= 0);
    bn::fixed result = speed + accel;
    if(result > max_speed) { 
        result = _max_speed;
    }else if(result < 0){
        result = 0;
    }
    return result;
}



}