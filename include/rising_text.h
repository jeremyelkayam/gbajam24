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

    void set_blending_enabled(bool enabled)
        { _text_sprites.at(0).set_blending_enabled(enabled); }

    void set_mosaic_enabled(bool enabled)
        { _text_sprites.at(0).set_mosaic_enabled(enabled); }

    void set_visible(const bool &visible)
        { _text_sprites.at(0).set_visible(visible); }

private:
    uint16_t text_width(const bn::string<256> &text, const int8_t *char_widths) const;

    bn::fixed _xcor;

    bn::vector<bn::sprite_ptr, 1> _text_sprites;
    uint8_t _timer, _sprite_width, _string_width;

    static constexpr const uint8_t TOTAL_TIME_VISIBLE = 90;

};

}