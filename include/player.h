#pragma once

#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_camera_ptr.h>


namespace aru
{
class player 
{
public: 
    player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y);
    void update();
    void take_button_input();
    bn::fixed_point position() const {return _body.position();}
    bn::fixed x() const {return _body.x();}
    bn::fixed y() const {return _body.y();}
    bool facing_left() const {return _body.horizontal_flip();}
    bool grounded() const;

private:
    bn::sprite_ptr _body, _jumpcloud;
    bn::sprite_animate_action<4> _idle;

    //todo: perhaps refactor this into an effects class
    bn::sprite_animate_action<6> _jumpcloud_anim;

    const bn::fixed _max_xspeed, _max_yspeed, _accel, _g;
    bn::fixed _xspeed, _target_xspeed, _yspeed;

    uint8_t _jump_timer, _jbuf_timer;

    bn::fixed get_new_xspeed(bn::fixed speed, bn::fixed max_xspeed, bn::fixed accel) const;
    void jump();
};

}