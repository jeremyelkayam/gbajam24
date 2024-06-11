#include "entity.h"
#include <bn_log.h>

namespace aru 
{
entity::entity(bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const uint8_t &hp, const uint8_t &contact_damage, level &level, const bn::sprite_item &spritem) : 
    _level(level),
    _sprite(spritem.create_sprite(x,y)),
    _MAX_XSPEED(3),
    _MAX_YSPEED(8),
    _ACCEL(0.75),
    _G(1),
    _hitbox(x, y, width, height),
    _grounded(false),
    _max_hp(hp),
    _jump_timer(0),
    _hp(hp),
    _contact_damage(contact_damage)
{
    _sprite.set_camera(cam);

}

void entity::update(){
    uint16_t sloped_ground_ytile = 0;
    bn::regular_bg_map_cell sloped_ground_type = 0;
    uint16_t center_xtile = (_hitbox.x() * bn::fixed(0.125)).floor_integer();
    uint16_t current_foot_tile = bottom_tile();
    

    for(uint16_t ytile = (_hitbox.top() * bn::fixed(0.125)).floor_integer(); 
            ytile < current_foot_tile + 1; ++ytile){
        
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

    bool fgrounded = on_flat_ground();    
    
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
        BN_LOG("HIT WALL!");
        BN_LOG("facing right: ", facing_right());
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
        if(sloped_ground_type == _level._UP_SLOPE){
            m =  -1;
            b = ((center_xtile + sloped_ground_ytile + 1) * 8);
            _xspeed *= INVSQRT2;
        }else if(sloped_ground_type == _level._UP_HALFSLOPE_1){
            m =  -0.5;
            b = ((bn::fixed(center_xtile) * bn::fixed(0.5)).floor_integer()
                 + sloped_ground_ytile + 1) * 8;
            _xspeed *= INVSQRT54THS;
        }else if(sloped_ground_type == _level._UP_HALFSLOPE_2){
            m =  -0.5;
            b = ((bn::fixed(center_xtile - 1) * bn::fixed(0.5)).floor_integer()
                 + sloped_ground_ytile + 1) * 8;
            _xspeed *= INVSQRT54THS;
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
}

bool entity::facing_wall() const{
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

void entity::jump(){
    _yspeed = -_MAX_YSPEED;
    _jump_timer = 0;
}

bool entity::on_flat_ground() const{
    return on_thick_ground() || on_thin_ground();
}

//todo: Less copy/pasted code here
// refactor it into a check on all tiles below your feet
bool entity::on_thick_ground() const{
    uint16_t current_foot_tile = bottom_tile();
    uint16_t current_right_tile = right_tile();
    for(uint16_t xtile = left_tile();
            xtile < current_right_tile; ++xtile){

        bn::regular_bg_map_cell tile_type = _level.cell_at(xtile, current_foot_tile);
        if(tile_type == _level._THICK_GROUND 
        || tile_type == _level._LFT_CORNER 
        || tile_type == _level._RFT_CORNER){
            return true;
        }
    }
    return false;
}

bool entity::on_thin_ground() const{
    uint16_t current_foot_tile = bottom_tile();
    uint16_t current_right_tile = right_tile();
    for(uint16_t xtile = left_tile();
            xtile < current_right_tile; ++xtile){

        bn::regular_bg_map_cell tile_type = _level.cell_at(xtile, current_foot_tile);
        if(tile_type == _level._THIN_GROUND){
            return true;
        }
    }
    return false;
}

void entity::land(){
    _yspeed = 0;
}

bool entity::apply_gravity() const{
   return  (_jump_timer > 4) && _yspeed < _MAX_YSPEED;
}

void entity::hit(uint8_t damage, bn::fixed x_push, bn::fixed y_push){
    _xspeed += x_push;
    _yspeed += y_push;
    //TODO: death check
    _hp -= damage;

    //TODO ALSO: maybe add a bit of punchiness to the attack like freeze frames or shake.
}


}