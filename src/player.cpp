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
    _walk_xspeed(2),
    _sprint_xspeed(3.5),
    _max_yspeed(8),
    _accel(0.75),
    _g(1),
    _hitbox(x, y, 20, 58),
    _jump_timer(0),
    _jbuf_timer(0),
    _coyote_timer(0),
    _doubletap_timer(0)
{
    _body.set_camera(cam);

}

void player::update(){
    take_button_input();


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
        _xspeed -= _accel;
    }
    if(_xspeed < _target_xspeed) {
        _xspeed += _accel;
    }

    if(_target_xspeed - _accel < _xspeed && _xspeed < _target_xspeed + _accel){
        // our speed will jitter so we need to set it to the target
        _xspeed = _target_xspeed;
    }

    if(!on_flat_ground() && !sloped_ground_ytile){
        if((!bn::keypad::a_held() || _jump_timer > 4) && _yspeed < _max_yspeed){
            _yspeed += _g;
        }
        // BN_LOG("we are falling");
        // BN_LOG("sloped ground ytile: ", sloped_ground_ytile);
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
        }else if(sloped_ground_type == _level._DOWN_SLOPE){
            m =  1;
            b = ((center_xtile - sloped_ground_ytile) * -8);
        }
        BN_LOG("b = ", b);
        bn::fixed ycor = m * _hitbox.x() + b;
        BN_LOG("y = ", ycor);
        BN_LOG("on slope! xcor: ", _hitbox.x());
        _hitbox.set_y(ycor - bn::fixed(0.5)*_hitbox.height());
        if(_yspeed > 0){
            land();
        }


    }else if(on_flat_ground()){
        BN_LOG("feet ycor: ", _hitbox.bottom());
        BN_LOG("feet xcor: ", _hitbox.x());
        _idle.update();
        _coyote_timer = 0;
        // if(_level.is_thick_ground(bottom_center) || _level.is_thin_ground(bottom_center)){
        bn::fixed map_cell_bottom = (_hitbox.bottom() * bn::fixed(0.125)).floor_integer() * 8;
        _hitbox.set_y(map_cell_bottom - bn::fixed(0.5)*_hitbox.height());
        // }else if(_level.is_up_slope(bottom_center - bn::fixed_point(0,8))){
        //     _hitbox.set_y(map_cell_bottom - (_hitbox.x().floor_integer() % 8) - bn::fixed(0.5)*_hitbox.height());
            // BN_LOG("bottom: ", _hitbox.bottom());
        //     BN_LOG("xcor: ", _hitbox.x());
        //     BN_LOG("xcor remainder: ", _hitbox.x().floor_integer() % 8);
        // }

        if(_yspeed > 0){
            land();
        }                    
    }

    // _level.print_hitbox(_hitbox);

    

    _body.set_x(_hitbox.x());
    _body.set_y(_hitbox.y());


    if(_jbuf_timer){
        // BN_LOG("jump buffer: ", _jbuf_timer);
        --_jbuf_timer;
    }

    if(_coyote_timer){
        // BN_LOG("coyote time: ", _coyote_timer);
        --_coyote_timer;
    }

    if(_doubletap_timer){
        --_doubletap_timer;
        // BN_LOG("doubletap time: ", _doubletap_timer);
    }
    _jumpcloud.update();
    _sprintcloud.update();

}

void player::take_button_input(){
    const bn::fixed _DUSTCLOUD_OFFSET = 40;

    if(bn::keypad::left_pressed()){
        // BN_LOG("facing right? ", facing_right());
        if(!facing_right() && _doubletap_timer){
            //sprint
            _target_xspeed = -_sprint_xspeed;
            _sprintcloud.set_horizontal_flip(true);
            _sprintcloud.start(_hitbox.x() + _DUSTCLOUD_OFFSET, _hitbox.y() + 17);
        }else{
            //walk
            _target_xspeed = -_walk_xspeed;
            _body.set_horizontal_flip(false);
            _doubletap_timer = 30;
        }
    }

    if(bn::keypad::right_pressed()){
        if(facing_right() && _doubletap_timer){
            //sprint
            _target_xspeed = _sprint_xspeed;
            _sprintcloud.set_horizontal_flip(false);
            _sprintcloud.start(_hitbox.x() - _DUSTCLOUD_OFFSET, _hitbox.y() + 17);
        }else{
            //walk
            _target_xspeed = _walk_xspeed;
            _body.set_horizontal_flip(true);
            _doubletap_timer = 30;
        }
    }
    
    if(!bn::keypad::left_held() && !bn::keypad::right_held()){
        _target_xspeed = 0;
    }

    if(on_flat_ground()){ //|| on_sloped_ground()){
        
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
                _jbuf_timer = 3; //3 frames = 0.05s
            }
        }
    }
}

void player::jump(){
    _yspeed = -_max_yspeed;
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
        // || _level.is_up_slope(pos) || _level.is_down_slope(pos);
    }
    if(thin_ground && !thick_ground && bn::keypad::down_held()){
        return false;
    }

    return thick_ground || thin_ground;
}


// bool player::on_sloped_ground() const{
//     bn::fixed_point bottom_center(_hitbox.x(), _hitbox.bottom() - 8);
//     bn::fixed_point bottom_center_below(_hitbox.x(), _hitbox.bottom());
//     //todo: all of these repeated conversions from coordinates to map indices are expensive
//     return _level.is_down_slope(bottom_center) || _level.is_up_slope(bottom_center) 
//      || _level.is_down_slope(bottom_center_below) || _level.is_up_slope(bottom_center_below) ;
// }



void player::land(){
    _yspeed = 0;
    _jbuf_timer = 0;
}

}