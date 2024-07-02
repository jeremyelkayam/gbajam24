#pragma once
#include <bn_sprite_animate_actions.h>
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
    entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const bn::sprite_item &spritem);
    virtual ~entity() {return;}
    virtual void update()=0;
    bn::fixed_point position() const {return _hitbox.position();}
    bn::fixed x() const {return _hitbox.x();}
    bn::fixed y() const {return _hitbox.y();}
    bn::fixed_rect hitbox() const {return _hitbox;}

    bool facing_right() const {return _sprite.horizontal_flip();}

protected:
    bn::fixed_rect _hitbox;
    bn::sprite_ptr _sprite;

};

}