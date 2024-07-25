#include "rising_text.h"
#include <bn_string.h>

namespace aru 
{
rising_text::rising_text(const bn::camera_ptr &cam, 
    bn::sprite_text_generator &text_generator, const int8_t &num, 
    const bn::fixed &x, const bn::fixed &y) : 
        _timer(255)
{
    text_generator.set_bg_priority(0);
    text_generator.set_left_alignment();
    text_generator.set_one_sprite_per_character(false);

    bn::string<4> text;
    bn::ostringstream stream(text);
    stream << num;

    text_generator.generate(x,y, text, _text_sprites);

    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_camera(cam);
    }

}

void rising_text::update(){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_y(sprite.y() - 1);
    }
    --_timer;
}

}