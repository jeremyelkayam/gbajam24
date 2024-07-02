#pragma once
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>
#include "entity.h"

namespace aru
{

class interactable_entity : public entity
{
public:
    interactable_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const bn::sprite_item &spritem);
    virtual ~interactable_entity() {return;}
    virtual void update();

protected:

};

}