#include "physics_entity.h"
#include "bn_sprite_items_explosion.h"
#include <bn_log.h>

namespace aru 
{
physics_entity::physics_entity(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, 
        const bn::fixed &max_xspeed, const bn::fixed &max_up_speed, 
        const bn::fixed &max_down_speed, level &level, 
        const bn::sprite_item &spritem): 
    entity(cam, x, y, width, height, spritem),
    _level(level),
    _MAX_XSPEED(max_xspeed),
    _MAX_UP_SPEED(max_up_speed),
    _MAX_DOWN_SPEED(max_down_speed),
    _ACCEL(0.75),
    _G(GRAVITY_ACCELERATION),
    _grounded(false),
    _jump_timer(0)
{

}

void physics_entity::update(){
    uint16_t sloped_ground_ytile = 0;
    bn::regular_bg_map_cell sloped_ground_type = 0;
    uint16_t center_xtile = (_hitbox.x() * bn::fixed(0.125)).floor_integer();
    uint16_t current_foot_tile = bottom_tile();

    //speed cap
    if(_xspeed > _MAX_XSPEED){
        _xspeed = _MAX_XSPEED;
    }
    if(_xspeed < -_MAX_XSPEED){
        _xspeed = -_MAX_XSPEED;
    }

    if(_yspeed > _MAX_DOWN_SPEED){
        _yspeed = _MAX_DOWN_SPEED;
    }
    if(_yspeed < _MAX_UP_SPEED){
        _yspeed = _MAX_UP_SPEED;
    }

    bool fgrounded = false;

    if(0 <= x() && x() <= _level.width() && 0 <= y() && y() <= _level.height() ){
        for(uint16_t ytile = (_hitbox.top() * bn::fixed(0.125)).floor_integer(); 
                ytile < current_foot_tile + 2; ++ytile){
            
            bn::regular_bg_map_cell tile_type = _level.cell_at(center_xtile, ytile);

            if(_level.tile_has_flag(tile_type, 
                tile_flags::UP_SLOPE | tile_flags:: DOWN_SLOPE | 
                tile_flags::UP_HALFSLOPE_A | tile_flags:: UP_HALFSLOPE_B | 
                tile_flags::DOWN_HALFSLOPE_A | tile_flags:: DOWN_HALFSLOPE_B)){
                sloped_ground_ytile = ytile;
                sloped_ground_type = tile_type;
                break;
            }
        }
        fgrounded = on_flat_ground();    
    }

    
    //Accelerate if necessary to reach target speed.
    if(_xspeed > _target_xspeed) {
        _xspeed -= _ACCEL;
    }
    if(_xspeed < _target_xspeed) {
        _xspeed += _ACCEL;
    }

    //set X speed to target if it's too fast.
    if(_target_xspeed - _ACCEL < _xspeed && _xspeed < _target_xspeed + _ACCEL){
        _xspeed = _target_xspeed;
    }


    //effects of gravity
    //MUST CHANGE FOR OTHER ENTITIES !!! 
    if(!fgrounded && !sloped_ground_ytile){
        //todo
        //REMOVE A BUTTON CLAUSE FOR OTHER ENTITIES...
        if(apply_gravity()){
            _yspeed += _G;
        }
        ++_jump_timer;
    }

    //collide with walls
    if((_xspeed != 0) && facing_wall()){
        _xspeed = 0;
    }

    //collide with ceilings
    if(_yspeed < 0 && _level.is_ceiling(bn::fixed_point(_hitbox.x(),_hitbox.top()))){
        _yspeed = 0;
    }


    //dont go off the edge of the map
    if(_hitbox.left() + _xspeed <= 0 ||
       _hitbox.right() + _xspeed >= _level.width()){

        _xspeed = 0;
    }

    fgrounded = on_flat_ground();

        
    if(sloped_ground_ytile){;
        //remember 7th grade algebra class? We're doing a slope equation today. 
        //y=mx+b
        bn::fixed b;
        bn::fixed m;
        if(_level.tile_has_flag(sloped_ground_type, 
                tile_flags::UP_SLOPE)){
            m =  -1;
            b = ((center_xtile + sloped_ground_ytile + 1) * 8);
            _xspeed *= INVSQRT2;
        }else if(_level.tile_has_flag(sloped_ground_type, 
                tile_flags::UP_HALFSLOPE_A)){
            m =  -0.5;
            b = ((bn::fixed(center_xtile) * bn::fixed(0.5)).floor_integer()
                 + sloped_ground_ytile + 1) * 8;
            _xspeed *= INVSQRT54THS;
        }else if(_level.tile_has_flag(sloped_ground_type, 
                tile_flags::UP_HALFSLOPE_B)){
            m =  -0.5;
            b = ((bn::fixed(center_xtile - 1) * bn::fixed(0.5)).floor_integer()
                 + sloped_ground_ytile + 1) * 8;
            _xspeed *= INVSQRT54THS;
        }else if(_level.tile_has_flag(sloped_ground_type, 
                tile_flags::DOWN_SLOPE)){
            m =  1;
            b = -((center_xtile - sloped_ground_ytile) * 8);
        }else if(_level.tile_has_flag(sloped_ground_type, 
                tile_flags::DOWN_HALFSLOPE_A)){
            m =  0.5;
            b = ((sloped_ground_ytile - bn::fixed(center_xtile)*bn::fixed(0.5)) * 8);
        }else if(_level.tile_has_flag(sloped_ground_type, 
                tile_flags::DOWN_HALFSLOPE_B)){
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

    }else if(fgrounded){
        //todo: it jitters when you go from flat to downward sloped ground
        // _idle.update();
        // _coyote_timer = 0;
        bn::fixed map_cell_bottom = (_hitbox.bottom() * bn::fixed(0.125)).floor_integer() * 8;
        _hitbox.set_y(map_cell_bottom - bn::fixed(0.5)*_hitbox.height());
        if(_yspeed > 0){
            land();
        }  
    }

    _grounded = fgrounded || sloped_ground_ytile;

    _hitbox.set_x(_hitbox.x() + _xspeed);
    _hitbox.set_y(_hitbox.y() + _yspeed);
    _sprite.set_x(_hitbox.x());
    _sprite.set_y(_hitbox.y());

}

bool physics_entity::facing_wall() const{
    //todo: refactor into level maybe?
    for(bn::fixed ycor = _hitbox.top(); ycor <= _hitbox.bottom(); ycor += 8){

        if(facing_right() && _level.is_leftfacing_wall(bn::fixed_point(_hitbox.right(),ycor))){
            return true;
        }
        else if(!facing_right() && _level.is_rightfacing_wall(bn::fixed_point(_hitbox.left(),ycor))){
            return true;
        }
    }  
    return false;  
}

void physics_entity::jump(){
    _yspeed = _MAX_UP_SPEED;
    _jump_timer = 0;
}

bool physics_entity::on_flat_ground() const{
    return on_thick_ground() || on_thin_ground();
}

//todo: Less copy/pasted code here
// refactor it into a check on all tiles below your feet
bool physics_entity::on_thick_ground() const{
    uint16_t current_foot_tile = bottom_tile();
    uint16_t current_right_tile = right_tile();
    for(uint16_t xtile = left_tile();
            xtile < current_right_tile; ++xtile){

        if(_level.tile_has_flag(bn::point(xtile, current_foot_tile), tile_flags::FLOOR)){
            return true;
        }
    }
    return false;
}

bool physics_entity::on_thin_ground() const{
    uint16_t current_foot_tile = bottom_tile();
    uint16_t current_right_tile = right_tile();
    for(uint16_t xtile = left_tile();
            xtile < current_right_tile; ++xtile){

        bn::point coords(xtile, current_foot_tile);
        if(_level.tile_has_flag(coords, tile_flags::THIN_FLOOR)){
            return true;
        }
    }
    return false;
}

void physics_entity::land(){
    _yspeed = 0;
    _sprite.set_rotation_angle(0);
}

bool physics_entity::apply_gravity() const{
   return  (_jump_timer > 4) && _yspeed < _MAX_DOWN_SPEED;
}


}