#pragma once
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>
#include "constants.h"
#include "level.h"

namespace aru
{

class interactable_entity
{
public:
    interactable_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, level &level, const bn::sprite_item &spritem);
    virtual ~interactable_entity() {return;}
    virtual void update();
    bn::fixed_point position() const {return _hitbox.position();}
    bn::fixed x() const {return _hitbox.x();}
    bn::fixed y() const {return _hitbox.y();}
    bn::fixed_rect hitbox() const {return _hitbox;}

    bool facing_right() const {return _sprite.horizontal_flip();}
    
protected:
    level &_level;
    bn::sprite_ptr _sprite;
    bn::fixed_rect _hitbox;

};

}