#include <bn_keypad.h>
#include <bn_log.h>


#include "player.h"
#include "bn_sprite_items_arutest.h"
#include "bn_sprite_items_jumpcloud.h"

namespace aru {

player::player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level) : 
    _level(level),
    _body(bn::sprite_items::arutest.create_sprite(x,y)),
    _jumpcloud(bn::sprite_items::arutest.create_sprite(x,y)),
    _idle(bn::create_sprite_animate_action_forever(_body, 8, bn::sprite_items::arutest.tiles_item(), 0, 1, 2, 1)),
    _jumpcloud_anim(bn::create_sprite_animate_action_once(_jumpcloud, 1, bn::sprite_items::jumpcloud.tiles_item(), 0, 1, 2, 3, 4, 5)),
    _max_xspeed(2),
    _max_yspeed(8),
    _accel(0.75),
    _g(1),
    _hitbox(x, y, 20, 64),
    _jump_timer(0),
    _jbuf_timer(0),
    _coyote_timer(0)
{
    _body.set_camera(cam);
    _jumpcloud.set_camera(cam);
    _jumpcloud.set_visible(false);

}

void player::update(){
    take_button_input();
    if(_xspeed > _target_xspeed) {
        _xspeed -= _accel;
    }
    if(_xspeed < _target_xspeed) {
        _xspeed += _accel;
    }

    if(_target_xspeed - _accel < _xspeed && _xspeed < _target_xspeed + _accel){
        // our speed will jitter so we need to set it to the target
        _xspeed = _target_xspeed;
    }

    if(!grounded()){
        if((!bn::keypad::a_held() || _jump_timer > 4) && _yspeed < _max_yspeed){
            _yspeed += _g;
        }
        ++_jump_timer;
    }

    //dont go off the edge of the map
    if(_hitbox.left() + _xspeed <= 0){
        _xspeed = 0;
    }else if(_hitbox.right() + _xspeed >= _level.width()){
        _xspeed = 0;
    }


    bool was_grounded = grounded();

    _hitbox.set_x(_hitbox.x() + _xspeed);
    _hitbox.set_y(_hitbox.y() + _yspeed);

    if(!grounded() && was_grounded && _yspeed >=0){
        _coyote_timer = 6; // 6 frames is 0.1s
    }

    

    if(grounded()){
        //put the bottom of the player at the top of the floor cell
        _hitbox.set_y((_hitbox.bottom() / 8).floor_integer() * 8 - bn::fixed(0.5)*_hitbox.height());
        _idle.update();
        _coyote_timer = 0;
    }

    _body.set_x(_hitbox.x());
    _body.set_y(_hitbox.y());


    if(_jbuf_timer > 0){
        // BN_LOG("jump buffer: ", _jbuf_timer);
        --_jbuf_timer;
    }

    if(_coyote_timer > 0){
        BN_LOG("coyote time: ", _coyote_timer);
        --_coyote_timer;
    }

    if(_jumpcloud_anim.done()){
        _jumpcloud.set_visible(false);
    }else{
        _jumpcloud_anim.update();
    }

}

void player::take_button_input(){
    if(bn::keypad::left_held()){

        _body.set_horizontal_flip(false);
        _target_xspeed = -_max_xspeed;

    }else if(bn::keypad::right_held()){

        _body.set_horizontal_flip(true);
        _target_xspeed = _max_xspeed;

    }else{
        _target_xspeed = 0;
    }

    if(grounded()){
        
        if(bn::keypad::a_pressed() || _jbuf_timer){
            //jbuf_timer will trigger if you pressed A just before hitting the ground
            jump();
        }else if(_yspeed > 0){
            _yspeed = 0;
            _jbuf_timer = 0;
        }
    }else{
        if(bn::keypad::a_pressed()){
            if(_coyote_timer){
                //in coyote time, we can still jump midair
                jump();
            }else{
                //we are in the air. can't jump now, so buffer the input 
                _jbuf_timer = 3; //3 frames = 0.05s
            }
        }
    }
}

void player::jump(){
    _yspeed = -_max_yspeed;
    _jump_timer = 0;
    _jumpcloud.set_x(_hitbox.x());
    _jumpcloud.set_y(_hitbox.y() + 17);
    _jumpcloud_anim.reset();
    _jumpcloud.set_visible(true);
}

bool player::grounded() const{
    if(bn::keypad::down_held() && 
        (_level.is_thin_ground(_hitbox.bottom_right()) || 
        _level.is_thin_ground(_hitbox.bottom_left()))){
        return false;
    }
    return _level.is_ground(_hitbox.bottom_right()) || _level.is_ground(_hitbox.bottom_left());
}

bn::fixed player::get_new_xspeed(bn::fixed speed, bn::fixed max_xspeed, bn::fixed accel) const{
    BN_ASSERT(speed >= 0);
    bn::fixed result = speed + accel;
    if(result > max_xspeed) { 
        result = _max_xspeed;
    }else if(result < 0){
        result = 0;
    }
    return result;
}



}