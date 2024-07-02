#include "interactable_entity.h"

namespace aru 
{
interactable_entity::interactable_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const bn::sprite_item &spritem): 
    entity(cam, x, y, width, height, spritem)
{
    
}

void interactable_entity::update(){

}

}