#pragma once
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>
#include "level.h"

namespace aru
{

class entity
{
public:
    entity(bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, level &level, const bn::sprite_item &spritem);
    virtual ~entity() {return;}
    virtual void update();
    bn::fixed_point position() const {return _hitbox.position();}
    bn::fixed x() const {return _hitbox.x();}
    bn::fixed y() const {return _hitbox.y();}
    bn::fixed_rect hitbox() const {return _hitbox;}
    bn::fixed xspeed() const {return _xspeed;}

    bool facing_right() const {return _sprite.horizontal_flip();}
    bool on_flat_ground() const;
    bool facing_wall() const;
    int left_tile() const {return (_hitbox.left() * bn::fixed(0.125)).floor_integer();}
    int right_tile() const {return (_hitbox.right() * bn::fixed(0.125)).floor_integer();}
    int top_tile() const {return (_hitbox.top() * bn::fixed(0.125)).floor_integer();}
    int bottom_tile() const {return (_hitbox.bottom() * bn::fixed(0.125)).floor_integer();}


protected:
    level &_level;
    bn::sprite_ptr _sprite;
    const bn::fixed _MAX_XSPEED, _MAX_YSPEED, _ACCEL, _G;
    bn::fixed _xspeed, _target_xspeed, _yspeed;
    bn::fixed_rect _hitbox;

    bool grounded;

    void jump();
    void land();

};

}