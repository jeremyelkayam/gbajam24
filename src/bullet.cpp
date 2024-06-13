#include "bullet.h"
#include "bn_sprite_items_bullet.h"
namespace aru { 

bullet::bullet(const bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level, bool facing_right) : 
    entity(cam, x,y,6,4,1,5,0,level,bn::sprite_items::bullet)
{
    //todo: probably refactor this b/c it's not necessary for bullet to have a target xspeed or really any gravity logic
    _xspeed = _MAX_XSPEED * (facing_right ? 1 : -1);
    _target_xspeed = _xspeed;
    _sprite.set_horizontal_flip(facing_right);

}

void bullet::update(){
    entity::update();
}

bool bullet::should_be_deleted() const{
    //is this terrible??? YES! Let's refactor it at some point lol bc bugs are going to happen
    return (_hp == 0) || (_level.cell_at(_hitbox.position()) != 0) || _xspeed == 0;
}

}
