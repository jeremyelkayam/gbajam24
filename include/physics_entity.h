#pragma once
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>
#include "constants.h"
#include "level.h"
#include "entity.h"

namespace aru
{

class physics_entity : public entity
{
public:
    physics_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, 
        const bn::fixed &width, const bn::fixed &height, const bn::fixed &max_xspeed, 
        const bn::fixed &max_up_speed, const bn::fixed &max_down_speed, 
        level &level, const bn::sprite_item &spritem);
    virtual void update();
    bn::fixed xspeed() const {return _xspeed;}

    virtual bool on_flat_ground() const;
    bool facing_wall() const;
    int left_tile() const {return (_hitbox.left() * bn::fixed(0.125)).floor_integer();}
    int right_tile() const {return (_hitbox.right() * bn::fixed(0.125)).floor_integer();}
    int top_tile() const {return (_hitbox.top() * bn::fixed(0.125)).floor_integer();}
    int bottom_tile() const {return (_hitbox.bottom() * bn::fixed(0.125)).floor_integer();}

protected:
    level &_level;

    const bn::fixed _MAX_XSPEED, _MAX_UP_SPEED, _MAX_DOWN_SPEED, _ACCEL, _G;
    bn::fixed _xspeed, _target_xspeed, _yspeed;
    
    bool _grounded;
    uint8_t _jump_timer;

    virtual void jump();
    virtual void land();

    virtual bool on_thin_ground() const;
    bool on_thick_ground() const;
    virtual bool apply_gravity() const;
};

}