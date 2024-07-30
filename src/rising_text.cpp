#include "rising_text.h"
#include <bn_string.h>
#include <bn_log.h>
#include <bn_size.h>
#include "small_numbers_sprite_font.h"

namespace aru 
{
rising_text::rising_text(const bn::camera_ptr &cam, 
    bn::sprite_text_generator &text_generator, const int8_t &num, 
    const bn::fixed &x, const bn::fixed &y) : 
        _xcor(x),
        _timer(TOTAL_TIME_VISIBLE),
        _string_width(0)
{
    //todo: maybe color code this based on player vs enemy or heal vs hurt
    text_generator.set_center_alignment();
    text_generator.set_one_sprite_per_character(false);

    bn::string<4> text;
    bn::ostringstream stream(text);
    if(num > 0){
        stream << '+';
    }
    stream << num;
    stream << "|";

    _string_width = text_width(text, small_numbers_sprite_font_character_widths);

    text_generator.generate(x,y, text, _text_sprites);

    _sprite_width = _text_sprites.at(0).dimensions().width();

    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_camera(cam);
        sprite.set_scale(2);
    }

}

void rising_text::update(){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_y(sprite.y() - 1);
        if(_timer < TOTAL_TIME_VISIBLE - 20){
            sprite.set_scale(bn::max(bn::fixed(1), 
                sprite.horizontal_scale() - bn::fixed(0.03333333333)));
        }
        //due to a bit of jank in the butano text generator, we have to
        //adjust the position of the text to keep it from shifting.
        sprite.set_x(_xcor + ((_sprite_width - _string_width) * 
            sprite.horizontal_scale() * bn::fixed(0.5)));

    }
    --_timer;
}

uint16_t rising_text::text_width(const bn::string<256> &text, const int8_t *char_widths) const 
{
    uint16_t width = 0;

    for(uint8_t z = 0; z < text.size(); ++z)
    {
        char c = text.at(z);
        width += char_widths[c - ' '];
    }
    return width;
}

}