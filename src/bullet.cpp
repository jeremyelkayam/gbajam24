#include <bn_log.h>
#include "bullet.h"
#include "bn_sprite_items_bullet.h"
namespace aru { 

bullet::bullet(const bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level, bool facing_right) : 
    combat_entity(cam, x, y, BULLET_WIDTH, BULLET_HEIGHT, BULLET_MAX_XSPEED, 
        BULLET_MAX_YSPEED, BULLET_MAX_YSPEED, BULLET_HP, BULLET_CONTACT_DAMAGE, 
        BULLET_IFRAMES, level,bn::sprite_items::bullet)
{
    //todo: probably refactor this b/c it's not necessary for bullet to have a target xspeed or really any gravity logic
    _xspeed = _MAX_XSPEED * (facing_right ? 1 : -1);
    _target_xspeed = _xspeed;
    _sprite.set_horizontal_flip(facing_right);

}

void bullet::update(){
    combat_entity::update();
}

bool bullet::should_be_deleted() const{
    BN_LOG("bullet hitbox: ", _hitbox.position().x(), ", ", _hitbox.position().y());
    BN_LOG("bullet cell", (int)_level.cell_at(_hitbox.position()));
    BN_LOG("bullet cell empty?", _level.tile_has_flag(_hitbox.position(), tile_flags::EMPTY));
    //is this terrible??? YES! Let's refactor it at some point lol bc bugs are going to happen
    return _hp == 0
        || _xspeed == 0
        || !(_level.tile_has_flag(_hitbox.position(), tile_flags::EMPTY) 
            || _level.tile_has_flag(_hitbox.position(), tile_flags::THIN_FLOOR));
}

}
