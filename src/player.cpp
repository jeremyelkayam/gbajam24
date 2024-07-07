#include <bn_keypad.h>
#include <bn_log.h>


#include "player.h"
#include "bn_sprite_items_aru.h"
#include "bn_sprite_items_jumpcloud.h"
#include "bn_sprite_items_sprintcloud.h"

namespace aru {

player::player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level) : 
    combat_entity(cam, x,y,PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_MAX_XSPEED, PLAYER_MAX_YSPEED,
        PLAYER_HP, PLAYER_CONTACT_DAMAGE, PLAYER_IFRAMES,level,bn::sprite_items::aru),
    _jumpcloud(cam,x,y,bn::sprite_items::jumpcloud,6),
    _sprintcloud(cam,x,y,bn::sprite_items::sprintcloud,9),
    _current_state(PSTATE::STAND),
    _idle(bn::create_sprite_animate_action_forever(_sprite, 10, 
        bn::sprite_items::aru.tiles_item(), 0, 1, 2, 1, 0, 1, 4, 1, 0, 1, 2, 1, 0, 1, 2, 1,
        3, 1, 2, 1)),
    _jump(bn::create_sprite_animate_action_forever(_sprite, 8, 
        bn::sprite_items::aru.tiles_item(), 7, 8)),
    _jumpsquat(bn::create_sprite_animate_action_once(_sprite, 2, 
        bn::sprite_items::aru.tiles_item(), 5, 6)),
    _fall(bn::create_sprite_animate_action_forever(_sprite, 8, 
        bn::sprite_items::aru.tiles_item(), 17, 18)),
    _hover(bn::create_sprite_animate_action_forever(_sprite, 8, 
        bn::sprite_items::aru.tiles_item(), 19, 20)),
    _run(bn::create_sprite_animate_action_forever(_sprite, 4, 
        bn::sprite_items::aru.tiles_item(), 9, 10, 11, 12, 13, 14, 15, 16)),
    _DUSTCLOUD_OFFSET(20),
    _jbuf_timer(0),
    _coyote_timer(0),
    _shoot_timer(0),
    _hover_timer(0)
{
    _sprite.set_z_order(0);
}

void player::update(){

    if(_current_state == PSTATE::JUMPSQUAT){
        if(_jumpsquat.done()){
            _jumpsquat.reset();
            jump();
        }
    }

    switch(_current_state){
        case PSTATE::HOVER:
            _hover.update();
            break;
        case PSTATE::JUMP:
            _jump.update();
            break;
        case PSTATE::FALL:
            _fall.update();
            break;
        case PSTATE::STAND:
            _idle.update();
            break;
        case PSTATE::RUN:
            _run.update();
            break;
        case PSTATE::JUMPSQUAT:
            _jumpsquat.update();
            break;
    }

    if((bn::keypad::left_held() || bn::keypad::right_held()) && _current_state == PSTATE::STAND){
        _current_state = PSTATE::RUN;
        _run.reset();
    }

    if(bn::keypad::left_pressed()){
        _target_xspeed = -_MAX_XSPEED;
        _sprintcloud.set_horizontal_flip(true);
        _sprite.set_horizontal_flip(false);
        if(_grounded){
            if(!_sprintcloud.visible()){
                _sprintcloud.start(_hitbox.x() + _DUSTCLOUD_OFFSET, _hitbox.y() + 15);
            }
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
        if(_current_state == PSTATE::RUN){
            _idle.reset();
            _current_state = PSTATE::STAND;
        }
    }
    

    if(_grounded){ 
        _hover_timer = PLAYER_HOVER_TIME;
        
        if(bn::keypad::a_pressed() || _jbuf_timer){
            _current_state = PSTATE::JUMPSQUAT;
        }
    }else{
        if(bn::keypad::a_pressed()){
            if(_coyote_timer){
                _current_state = PSTATE::JUMPSQUAT;
            }
        }
        if(bn::keypad::a_held() && _current_state == PSTATE::FALL && _hover_timer){
            _current_state = PSTATE::HOVER;
            _hover.reset();
        }

        if(_current_state == PSTATE::HOVER){
            --_hover_timer;
            _yspeed -= 0.5;
            if(_yspeed < -1){
                _yspeed = -1;
            }

            if(_hover_timer == 0 || bn::keypad::a_released()){
               _current_state = PSTATE::JUMP;
            }
        }

        if(_current_state != PSTATE::HOVER && _yspeed > 0){ 
            _current_state = PSTATE::FALL;
            _fall.reset();
        }

    }

    if(bn::keypad::b_pressed()){
        shoot();
    }



    
    // if(!grounded && was_grounded && _yspeed >=0){
    //     _coyote_timer = 12; // 12 frames is 0.2s
    // }
    



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
    combat_entity::update();
}

void player::jump(){
    combat_entity::jump();
    _jbuf_timer = 0;
    _jumpcloud.start(_hitbox.x(), _hitbox.y() + 8);
    _jump.reset();
    _current_state = PSTATE::JUMP;
}

bool player::on_thin_ground() const{
    if (bn::keypad::down_held())
    {
        return false;
    }
    return combat_entity::on_thin_ground();   
}

bool player::apply_gravity() const{
   return _current_state != PSTATE::HOVER && (!bn::keypad::a_held() || _jump_timer > 4) && _yspeed < _MAX_YSPEED;
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

void player::land(){
    combat_entity::land();
    _idle.reset();
    _current_state = PSTATE::STAND;
}

}