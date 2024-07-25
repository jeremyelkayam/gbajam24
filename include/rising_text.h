#pragma once
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>

namespace aru
{

class rising_text
{
public:
    rising_text(const bn::camera_ptr &cam, 
        bn::sprite_text_generator &text_generator, const int8_t &num, 
        const bn::fixed &x, const bn::fixed &y);
    void update();
    bool delete_me() const {return _timer == 0;}

private:
    bn::vector<bn::sprite_ptr, 1> _text_sprites;
    uint8_t _timer;

};

}