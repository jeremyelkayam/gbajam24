#include <bn_keypad.h>
#include <bn_log.h>


#include "player.h"
#include "bn_sprite_items_arutest.h"
#include "bn_sprite_items_jumpcloud.h"
#include "bn_sprite_items_sprintcloud.h"

namespace aru {

player::player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level) : 
    entity(cam, x,y,20,54,50,10,level,bn::sprite_items::arutest),
    _jumpcloud(cam,x,y,bn::sprite_items::jumpcloud,6),
    _sprintcloud(cam,x,y,bn::sprite_items::sprintcloud,9),
    _idle(bn::create_sprite_animate_action_forever(_sprite, 8, bn::sprite_items::arutest.tiles_item(), 0, 1, 2, 1)),
    _DUSTCLOUD_OFFSET(40),
    _jbuf_timer(0),
    _coyote_timer(0)
{

}

void player::update(){

    if(bn::keypad::left_pressed()){
        _target_xspeed = -_MAX_XSPEED;
        _sprintcloud.set_horizontal_flip(true);
        _sprite.set_horizontal_flip(false);
        if(!_sprintcloud.visible() && _grounded){
            _sprintcloud.start(_hitbox.x() + _DUSTCLOUD_OFFSET, _hitbox.y() + 15);
        }
    }



    if(bn::keypad::right_pressed()){
        _target_xspeed = _MAX_XSPEED;
        _sprintcloud.set_horizontal_flip(false);
        _sprite.set_horizontal_flip(true);
        if(!_sprintcloud.visible() && _grounded){
            _sprintcloud.start(_hitbox.x() - _DUSTCLOUD_OFFSET, _hitbox.y() + 15);
        }
        
    }
    
    if(!bn::keypad::left_held() && !bn::keypad::right_held()){
        _target_xspeed = 0;
    }
    

    if(_grounded){ 
        
        if(bn::keypad::a_pressed() || _jbuf_timer){
            //jbuf_timer will trigger if you pressed A just before hitting the ground
            jump();
        }
    }else{
        if(bn::keypad::a_pressed()){
            if(_coyote_timer){
                jump();
            }else{
                _jbuf_timer = 6; //6 frames = 0.1s
            }
        }
    }


    
    // if(!grounded && was_grounded && _yspeed >=0){
    //     _coyote_timer = 12; // 12 frames is 0.2s
    // }



    _sprite.set_x(_hitbox.x());
    //slight hitbox adjustment to make the antennae not part of the box
    _sprite.set_y(_hitbox.y() - 2);


    if(_jbuf_timer){
        --_jbuf_timer;
    }

    if(_coyote_timer){
        --_coyote_timer;
    }

    _jumpcloud.update();
    _sprintcloud.update();
    

    //todo: there is a bit of jank where you fall in the tile for a frame... let's fix that  
    entity::update();
}

void player::jump(){
    entity::jump();
    _jbuf_timer = 0;
    _jumpcloud.start(_hitbox.x(), _hitbox.y() + 16);
}

bool player::on_thin_ground() const{
    if (bn::keypad::down_held())
    {
        return false;
    }
    return entity::on_thin_ground();   
}

bool player::apply_gravity() const{
   return  (!bn::keypad::a_held() || _jump_timer > 4) && _yspeed < _MAX_YSPEED;
}

}