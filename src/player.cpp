#include <bn_keypad.h>
#include <bn_log.h>


#include "player.h"
#include "bn_sprite_items_arutest.h"
#include "bn_sprite_items_jumpcloud.h"
#include "bn_sprite_items_sprintcloud.h"

namespace aru {

player::player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level) : 
    entity(cam, x,y,PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_MAX_XSPEED, PLAYER_MAX_YSPEED,
        PLAYER_HP, PLAYER_CONTACT_DAMAGE, PLAYER_IFRAMES,level,bn::sprite_items::arutest),
    _jumpcloud(cam,x,y,bn::sprite_items::jumpcloud,6),
    _sprintcloud(cam,x,y,bn::sprite_items::sprintcloud,9),
    _idle(bn::create_sprite_animate_action_forever(_sprite, 8, bn::sprite_items::arutest.tiles_item(), 0, 1, 2, 1)),
    _DUSTCLOUD_OFFSET(40),
    _jbuf_timer(0),
    _coyote_timer(0),
    _shoot_timer(0)
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

    if(bn::keypad::b_pressed()){
        shoot();
    }


    
    // if(!grounded && was_grounded && _yspeed >=0){
    //     _coyote_timer = 12; // 12 frames is 0.2s
    // }
    
    //todo : adjust the player sprite up 2



    if(_jbuf_timer){
        --_jbuf_timer;
    }

    if(_coyote_timer){
        --_coyote_timer;
    }
    if(_shoot_timer){
        --_shoot_timer;
    }

    _jumpcloud.update();
    _sprintcloud.update();
    
    for(bullet &bullet : _bullets){
        bullet.update();
    }
    _bullets.remove_if(bullet_deletable);

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

void player::shoot(){
    if(_shoot_timer == 0){
        _shoot_timer = 12;
        _bullets.emplace_front(*_sprite.camera().get(), x(), y(), _level, facing_right());
    }
}

bool player::check_bullet_collision(enemy &enemy){
    bool result = false;
    for(bullet &bullet : _bullets){
        if(enemy.hitbox().intersects(bullet.hitbox())){
            bn::fixed hori_kb = 6 * (bullet.facing_right() ? 1 : -1); 
            enemy.hit(bullet.contact_damage(),hori_kb,-3);
            bullet.hit(enemy.contact_damage(),hori_kb,-3);
            result = true;
        }
    }
    return result;
}

}