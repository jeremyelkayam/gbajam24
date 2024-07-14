#include "combat_entity.h"
#include "bn_sprite_items_explosion.h"
#include <bn_log.h>

namespace aru 
{
combat_entity::combat_entity(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, 
        const bn::fixed &max_xspeed, const bn::fixed &max_up_speed, 
        const bn::fixed &max_down_speed, const uint8_t &hp, 
        const uint8_t &contact_damage, const uint8_t &iframes, level &level, 
        const bn::sprite_item &spritem): 
    entity(cam, x, y, width, height, spritem),
    _level(level),
    _MAX_XSPEED(max_xspeed),
    _MAX_UP_SPEED(max_up_speed),
    _MAX_DOWN_SPEED(max_down_speed),
    _ACCEL(0.75),
    _G(GRAVITY_ACCELERATION),
    _HIT_IFRAMES(iframes),
    _grounded(false),
    _max_hp(hp),
    _jump_timer(0),
    _hp(hp),
    _contact_damage(contact_damage),
    _iframes(0)
{

}

void combat_entity::update(){
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
        _yspeed = -_MAX_UP_SPEED;
    }

    for(uint16_t ytile = (_hitbox.top() * bn::fixed(0.125)).floor_integer(); 
            ytile < current_foot_tile + 1; ++ytile){
        
        bn::regular_bg_map_cell tile_type = _level.cell_at(center_xtile, ytile);

        if(_level.tile_has_flag(tile_type, 
            tile_flags::UP_SLOPE | tile_flags:: DOWN_SLOPE | 
            tile_flags::UP_HALFSLOPE_A | tile_flags:: UP_HALFSLOPE_B | 
            tile_flags::DOWN_HALFSLOPE_A | tile_flags:: DOWN_HALFSLOPE_B)){
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

    if(_iframes){
        _sprite.set_visible(_iframes % 2);
        --_iframes;
    }

    _hitbox.set_x(_hitbox.x() + _xspeed);
    _hitbox.set_y(_hitbox.y() + _yspeed);
    _sprite.set_x(_hitbox.x());
    _sprite.set_y(_hitbox.y());

    if(_explosion_anim) _explosion_anim->update();

}

bool combat_entity::facing_wall() const{
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

void combat_entity::jump(){
    _yspeed = _MAX_UP_SPEED;
    _jump_timer = 0;
}

bool combat_entity::on_flat_ground() const{
    return on_thick_ground() || on_thin_ground();
}

//todo: Less copy/pasted code here
// refactor it into a check on all tiles below your feet
bool combat_entity::on_thick_ground() const{
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

bool combat_entity::on_thin_ground() const{
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

void combat_entity::land(){
    _yspeed = 0;
    _sprite.set_rotation_angle(0);
}

bool combat_entity::apply_gravity() const{
   return  (_jump_timer > 4) && _yspeed < _MAX_DOWN_SPEED;
}

void combat_entity::hit(uint8_t damage, bn::fixed x_push, bn::fixed y_push){
    if(!_iframes){

        if(_hp > damage){
            _hp -= damage;

            bn::fixed new_angle = _sprite.rotation_angle() - 2*x_push;
            if(new_angle < 0){
                new_angle += 360;
            }else if(360 <= new_angle){
                new_angle -= 360;
            }
            _sprite.set_rotation_angle(new_angle);

            _xspeed += x_push;

            _yspeed += y_push;
        }else{
            _hp = 0;
            die();
        }
        _iframes = _HIT_IFRAMES;
    }

    //TODO ALSO: maybe add a bit of punchiness to the attack like freeze frames or shake.
}

void combat_entity::die(){
    _sprite.set_item(bn::sprite_items::explosion);
    _sprite.set_scale(2);
    _explosion_anim.emplace(bn::create_sprite_animate_action_once(_sprite, 4, bn::sprite_items::explosion.tiles_item(), 0, 1, 2, 3, 4, 5));
}


}