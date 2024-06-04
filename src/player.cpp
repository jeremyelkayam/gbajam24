#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_math.h>


#include "player.h"
#include "bn_sprite_items_arutest.h"
#include "bn_sprite_items_jumpcloud.h"
#include "bn_sprite_items_sprintcloud.h"

namespace aru {

player::player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level) : 
    _level(level),
    _jumpcloud(cam,x,y,bn::sprite_items::jumpcloud,6),
    _sprintcloud(cam,x,y,bn::sprite_items::sprintcloud,9),
    _body(bn::sprite_items::arutest.create_sprite(x,y)),
    _idle(bn::create_sprite_animate_action_forever(_body, 8, bn::sprite_items::arutest.tiles_item(), 0, 1, 2, 1)),
    _SPRINT_XSPEED(3),
    _MAX_YSPEED(8),
    _ACCEL(0.75),
    _G(1),
    _DUSTCLOUD_OFFSET(40),
    _hitbox(x, y, 20, 54),
    _jump_timer(0),
    _jbuf_timer(0),
    _coyote_timer(0)
{
    _body.set_camera(cam);

}

void player::update(){
    


    if(bn::keypad::left_pressed()){
        _target_xspeed = -_SPRINT_XSPEED;
        _sprintcloud.set_horizontal_flip(true);
        _body.set_horizontal_flip(false);
        _sprintcloud.start(_hitbox.x() + _DUSTCLOUD_OFFSET, _hitbox.y() + 15);
    }

    if(bn::keypad::right_pressed()){
        _target_xspeed = _SPRINT_XSPEED;
        _sprintcloud.set_horizontal_flip(false);
        _body.set_horizontal_flip(true);
        _sprintcloud.start(_hitbox.x() - _DUSTCLOUD_OFFSET, _hitbox.y() + 15);
        
    }
    
    if(!bn::keypad::left_held() && !bn::keypad::right_held()){
        _target_xspeed = 0;
    }


    uint16_t sloped_ground_ytile = 0;
    bn::regular_bg_map_cell sloped_ground_type = 0;
    uint16_t center_xtile = (_hitbox.x() * bn::fixed(0.125)).floor_integer();
    uint16_t foot_tile = (_hitbox.bottom() * bn::fixed(0.125)).ceil_integer();
    

    for(uint16_t ytile = (_hitbox.top() * bn::fixed(0.125)).floor_integer(); 
            ytile < foot_tile + 1; ++ytile){
        
        bn::regular_bg_map_cell tile_type = _level.cell_at(center_xtile, ytile);

        if(tile_type == _level._UP_SLOPE || 
           tile_type == _level._DOWN_SLOPE ||
           tile_type == _level._UP_HALFSLOPE_1 || 
           tile_type == _level._DOWN_HALFSLOPE_1 ||
           tile_type == _level._UP_HALFSLOPE_2 || 
           tile_type == _level._DOWN_HALFSLOPE_2){
            // BN_LOG("ur on an up slope");
            sloped_ground_ytile = ytile;
            sloped_ground_type = tile_type;
            break;
        }
    }

    if(on_flat_ground() || sloped_ground_ytile){ 
        
        if(bn::keypad::a_pressed() || _jbuf_timer){
            //jbuf_timer will trigger if you pressed A just before hitting the ground
            jump();
        }
    }else{
        if(bn::keypad::a_pressed()){
            if(_coyote_timer){
                //in coyote time, we can still jump midair
                jump();
            }else{
                //we are in the air. can't jump now, so buffer the input 
                _jbuf_timer = 6; //6 frames = 0.1s
            }
        }
    }

    bool ground_below_feet = false;
    uint16_t right_tile = (_hitbox.left() * bn::fixed(0.125)).ceil_integer();
    for(uint16_t xtile = (_hitbox.left() * bn::fixed(0.125)).floor_integer();
            xtile < right_tile; ++xtile){

        bn::regular_bg_map_cell tile_type = _level.cell_at(xtile, foot_tile);
        if(tile_type == _level._THICK_GROUND){
            ground_below_feet = true;
            // BN_LOG("standing on ground");
            break;
        }
    }



    if(_xspeed > _target_xspeed) {
        _xspeed -= _ACCEL;
    }
    if(_xspeed < _target_xspeed) {
        _xspeed += _ACCEL;
    }

    if(_target_xspeed - _ACCEL < _xspeed && _xspeed < _target_xspeed + _ACCEL){
        // our speed will jitter so we need to set it to the target
        _xspeed = _target_xspeed;
    }

    if(!on_flat_ground() && !sloped_ground_ytile){
        if((!bn::keypad::a_held() || _jump_timer > 4) && _yspeed < _MAX_YSPEED){
            _yspeed += _G;
        }
        ++_jump_timer;
    }

    // bn::fixed_rect hitbox_after_movement(_hitbox);
    // hitbox_after_movement.set_x(_hitbox.x() + _xspeed);
    // hitbox_after_movement.set_y(_hitbox.y() + _yspeed);

    //todo: less copypasted code
    if(bn::keypad::left_held()){
        for(bn::fixed ycor = _hitbox.top(); ycor <= _hitbox.bottom(); ycor += 8){
            if(_level.is_rightfacing_wall(bn::fixed_point(_hitbox.left(),ycor))) _xspeed = 0;
        }
    }else if(bn::keypad::right_held()){
        for(bn::fixed ycor = _hitbox.top(); ycor <= _hitbox.bottom(); ycor += 8){
            if(_level.is_leftfacing_wall(bn::fixed_point(_hitbox.right(),ycor))) _xspeed = 0;
        }
    }
    if(_yspeed < 0 && _level.is_ceiling(bn::fixed_point(_hitbox.x(),_hitbox.top()))){
        _yspeed = 0;
    }


    //dont go off the edge of the map
    if(_hitbox.left() + _xspeed <= 0 ||
       _hitbox.right() + _xspeed >= _level.width()){

        _xspeed = 0;
    }


    bool was_grounded = on_flat_ground();

    _hitbox.set_x(_hitbox.x() + _xspeed);
    _hitbox.set_y(_hitbox.y() + _yspeed);

    if(!on_flat_ground() && !sloped_ground_ytile && was_grounded && _yspeed >=0){
        _coyote_timer = 12; // 6 frames is 0.1s
    }

    
    if(sloped_ground_ytile){;
        //remember 7th grade algebra class? We're doing a slope equation today. 
        //y=mx+b
        bn::fixed b;
        bn::fixed m;
        if(sloped_ground_type == _level._UP_SLOPE){
            m =  -1;
            b = ((center_xtile + sloped_ground_ytile + 1) * 8);
        }else if(sloped_ground_type == _level._UP_HALFSLOPE_1){
            m =  -0.5;
            b = ((bn::fixed(center_xtile) * bn::fixed(0.5)).floor_integer()
                 + sloped_ground_ytile + 1) * 8;
        }else if(sloped_ground_type == _level._UP_HALFSLOPE_2){
            m =  -0.5;
            b = ((bn::fixed(center_xtile - 1) * bn::fixed(0.5)).floor_integer()
                 + sloped_ground_ytile + 1) * 8;
        }else if(sloped_ground_type == _level._DOWN_SLOPE){
            m =  1;
            b = -((center_xtile - sloped_ground_ytile) * 8);
        }else if(sloped_ground_type == _level._DOWN_HALFSLOPE_1){
            m =  0.5;
            b = ((sloped_ground_ytile - bn::fixed(center_xtile)*bn::fixed(0.5)) * 8);
        }else if(sloped_ground_type == _level._DOWN_HALFSLOPE_2){
            m =  0.5;
            b = ((sloped_ground_ytile - bn::fixed(center_xtile - 1)*bn::fixed(0.5)) * 8);
        }
        bn::fixed ycor = m * _hitbox.x() + b;
        if(_yspeed > 0){
            land();
        }
        if(_yspeed >=0){
            _hitbox.set_y(ycor - bn::fixed(0.5)*_hitbox.height());
        }
        


    }else if(on_flat_ground()){
        _idle.update();
        _coyote_timer = 0;
        bn::fixed map_cell_bottom = (_hitbox.bottom() * bn::fixed(0.125)).floor_integer() * 8;
        _hitbox.set_y(map_cell_bottom - bn::fixed(0.5)*_hitbox.height());
        if(_yspeed > 0){
            land();
        }                    
    }    

    _body.set_x(_hitbox.x());

    //slight hitbox adjustment to make the antennae not part of the box
    _body.set_y(_hitbox.y() - 2);


    if(_jbuf_timer){
        --_jbuf_timer;
    }

    if(_coyote_timer){
        --_coyote_timer;
    }

    _jumpcloud.update();
    _sprintcloud.update();

}

void player::jump(){
    _yspeed = -_MAX_YSPEED;
    _jump_timer = 0;
    _jumpcloud.start(_hitbox.x(), _hitbox.y() + 16);
}

bool player::on_flat_ground() const{
    bool thin_ground = false;
    bool thick_ground = false;
    for(bn::fixed xcor = _hitbox.left(); xcor <= _hitbox.right(); xcor += 8){
        bn::fixed_point pos(xcor, _hitbox.bottom());
        thin_ground = thin_ground || _level.is_thin_ground(pos);
        thick_ground = thick_ground || _level.is_thick_ground(pos) ;
    }
    if(thin_ground && !thick_ground && bn::keypad::down_held()){
        return false;
    }

    return thick_ground || thin_ground;
}

void player::land(){
    _yspeed = 0;
    _jbuf_timer = 0;
}

}