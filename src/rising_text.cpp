#include "rising_text.h"
#include <bn_string.h>

namespace aru 
{
rising_text::rising_text(const bn::camera_ptr &cam, 
    bn::sprite_text_generator &text_generator, const int8_t &num, 
    const bn::fixed &x, const bn::fixed &y) : 
        _timer(120)
{
    //todo: maybe color code this based on player vs enemy
    text_generator.set_bg_priority(0);
    text_generator.set_left_alignment();
    text_generator.set_one_sprite_per_character(false);

    bn::string<4> text;
    bn::ostringstream stream(text);
    stream << num;

    text_generator.generate(x,y, text, _text_sprites);

    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_camera(cam);
        // sprite.set_scale(2);
    }

}

void rising_text::update(){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_y(sprite.y() - 1);
        if(_timer < 110){
            // sprite.set_scale(bn::max(bn::fixed(1), 
                // sprite.horizontal_scale() - bn::fixed(0.03333333333)));
        }
    }
    --_timer;
}

}