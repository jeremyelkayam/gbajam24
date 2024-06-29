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

class combat_entity
{
public:
    combat_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const bn::fixed &max_xspeed, const bn::fixed &max_yspeed, const uint8_t &hp, const uint8_t &contact_damage, const uint8_t &iframes, level &level, const bn::sprite_item &spritem);
    virtual ~combat_entity() {return;}
    virtual void update();
    bn::fixed_point position() const {return _hitbox.position();}
    bn::fixed x() const {return _hitbox.x();}
    bn::fixed y() const {return _hitbox.y();}
    bn::fixed_rect hitbox() const {return _hitbox;}
    bn::fixed xspeed() const {return _xspeed;}

    bool facing_right() const {return _sprite.horizontal_flip();}
    bool on_flat_ground() const;
    bool facing_wall() const;
    int left_tile() const {return (_hitbox.left() * bn::fixed(0.125)).floor_integer();}
    int right_tile() const {return (_hitbox.right() * bn::fixed(0.125)).floor_integer();}
    int top_tile() const {return (_hitbox.top() * bn::fixed(0.125)).floor_integer();}
    int bottom_tile() const {return (_hitbox.bottom() * bn::fixed(0.125)).floor_integer();}
    void hit(uint8_t damage, bn::fixed x_push, bn::fixed y_push);
    uint8_t contact_damage() const {return _contact_damage;}
    uint8_t hp() const {return _hp;}
    uint8_t max_hp() const {return _max_hp;}
    bool in_iframes() const {return _iframes;}
    bool delete_me() const {return _explosion_anim && _explosion_anim->done();}


protected:
    level &_level;
    bn::sprite_ptr _sprite;
    bn::optional<bn::sprite_animate_action<6>> _explosion_anim;
    const bn::fixed _MAX_XSPEED, _MAX_YSPEED, _ACCEL, _G;
    const uint8_t _HIT_IFRAMES;
    bn::fixed _xspeed, _target_xspeed, _yspeed;
    bn::fixed_rect _hitbox;

    bool _grounded;
    const uint8_t _max_hp;
    uint8_t _jump_timer, _hp, _contact_damage,_iframes;

    virtual void jump();
    virtual void land();

    virtual bool on_thin_ground() const;
    bool on_thick_ground() const;
    virtual bool apply_gravity() const;
    void die();

};

}