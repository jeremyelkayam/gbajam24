#pragma once

#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>


namespace aru
{
class player 
{
public: 
    player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, bn::fixed max_speed, bn::fixed accel);
    void update();
    void take_button_input();
    bn::fixed_point position() const {return _body.position();}
    bn::fixed x() const {return _body.x();}
    bn::fixed y() const {return _body.y();}
    bool facing_left() const {return _body.horizontal_flip();}

private:
    bn::sprite_ptr _body;
    const bn::fixed _max_speed, _accel;
    bn::fixed _speed, _target_speed;

    bn::fixed get_new_speed(bn::fixed speed, bn::fixed max_speed, bn::fixed accel) const;
};

}