#include "interactable_entity.h"

namespace aru 
{
interactable_entity::interactable_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, level &level, const bn::sprite_item &spritem): 
    _level(level),
    _sprite(spritem.create_sprite(x,y)),
    _hitbox(x, y, width, height)
{
    _sprite.set_camera(cam);
}

void interactable_entity::update(){
}
}