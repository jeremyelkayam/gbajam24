#pragma once

#include <bn_camera_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_item.h>
#include <bn_unique_ptr.h>
#include <bn_sprite_animate_actions.h>

namespace aru
{
class effect
{
public: 
    effect(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::sprite_item &spritem, const uint8_t &indices);
    void update();
    void start(const bn::fixed &x, const bn::fixed &y);
    void set_horizontal_flip(const bool &flip) {_sprite.set_horizontal_flip(flip);}
    bool visible(){return _sprite.visible();}
private:
    uint16_t gfxindices_arr[10];
    bn::sprite_ptr _sprite;
    bn::sprite_animate_action<10> _anim;
    static bn::span<uint16_t> indices_span(const uint8_t &indices);
};

}