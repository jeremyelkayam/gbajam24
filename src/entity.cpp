
#include "entity.h"

namespace aru 
{
entity::entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const bn::sprite_item &spritem): 
    _hitbox(x, y, width, height),
    _sprite(spritem.create_sprite(x,y))
{
    _sprite.set_z_order(1);
    _sprite.set_camera(cam);
}

}