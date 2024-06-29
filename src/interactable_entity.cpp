#include "interactable_entity.h"
#include "bn_sprite_items_a_button_prompt.h"

namespace aru 
{
//TODO: Refactor this to have the same superclass as combat entities. 
interactable_entity::interactable_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, level &level, const bn::sprite_item &spritem): 
    _level(level),
    _hitbox(x, y, width, height),
    _sprite(spritem.create_sprite(x,y)),
    _interact_icon(bn::sprite_items::a_button_prompt.create_sprite(x, _hitbox.top() - 8))
{
    _sprite.set_camera(cam);
}

void interactable_entity::update(){

}

}