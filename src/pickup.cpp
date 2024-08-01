#include "pickup.h"
#include "bn_sprite_items_pink_thing.h"

namespace aru
{
pickup::pickup(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, const bn::fixed &width, const bn::fixed &height,
        const bn::fixed &max_up_speed, 
        const bn::fixed &max_down_speed, level &level, 
        const bn::sprite_item &spritem, 
        const uint16_t &value): 
    physics_entity(cam, x, y, width, height, 0, 
        max_up_speed, max_down_speed, level, spritem),
    _value(value)
{

}

health_pickup::health_pickup(const bn::camera_ptr &cam, const bn::fixed &x, 
    const bn::fixed &y, level &level, const uint16_t &value) : 
    pickup(cam, x, y, 8, 8, PLAYER_JUMP_YSPEED, PLAYER_FALL_YSPEED, level, 
        bn::sprite_items::pink_thing, value)
{

}
}