#include "enemy.h"
#include "bn_sprite_items_globtest.h"
namespace aru { 

enemy::enemy(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level) : 
    combat_entity(cam, x,y,42,36,3,8,20,5,10,level,bn::sprite_items::globtest),
    _idle(bn::create_sprite_animate_action_forever(_sprite, 8, bn::sprite_items::globtest.tiles_item(), 0, 1, 2, 3, 4, 5)),
    _jump_again_timer(0)
{

}

void enemy::update(){
    if(!_explosion_anim){
        _idle.update();
        if(_jump_again_timer > 240){
            jump();
            _jump_again_timer = 0;
        }
        ++_jump_again_timer;
    }

    combat_entity::update();
}

}
