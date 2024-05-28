#pragma once

#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>
#include "level.h"


namespace aru
{
class player 
{
public: 
    //todo: prob do not need to put spawnpoint in the constructor if it takes a level reference
    player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level);
    void update();
    void take_button_input();
    bn::fixed_point position() const {return _hitbox.position();}
    bn::fixed x() const {return _hitbox.x();}
    bn::fixed y() const {return _hitbox.y();}
    bn::fixed_rect hitbox() const {return _hitbox;}
    bool facing_right() const {return _body.horizontal_flip();}
    bool grounded() const;

private:
    level &_level;
    bn::sprite_ptr _body, _jumpcloud;
    bn::sprite_animate_action<4> _idle;

    //todo: perhaps refactor this into an effects class
    bn::sprite_animate_action<6> _jumpcloud_anim;

    const bn::fixed _walk_xspeed, _sprint_xspeed, _max_yspeed, _accel, _g;
    bn::fixed _xspeed, _target_xspeed, _yspeed;
    bn::fixed_rect _hitbox;

    uint8_t _jump_timer, _jbuf_timer, _coyote_timer, _doubletap_timer;

    void jump();
};

}