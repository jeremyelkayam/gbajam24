#include "warp_effect.h"
#include "common_stuff.h"
#include <bn_math.h>
#include <bn_blending.h>
#include <bn_log.h>
#include "bn_sprite_items_aru.h"

namespace aru {

warp_effect::warp_effect(bn::sprite_ptr &sprite, bool reverse) : 
    _reverse(reverse),
    _frequency(16),
    _speed(4),
    _base_degrees_angle(0),
    _horizontal_position_hbe(bn::sprite_position_hbe_ptr::create_horizontal(sprite, _horizontal_deltas)),
    _amplitude_ease(reverse ? 100 : 0, reverse ? 0 : 100, reverse ? 120 : 240, reverse ? easer::sine_ease_out : easer::sine_ease_in),
    _sprite(sprite)
{
    sprite.set_blending_enabled(true);
    if(reverse){
        sprite.set_palette(common_stuff::monochrome_palette(bn::color(31,31,31)));
    }
}

void warp_effect::update()
{
    _amplitude_ease.update();
    _base_degrees_angle += _speed;

    if(_base_degrees_angle >= 360)
    {
        _base_degrees_angle -= 360;
    }

    bn::fixed degrees_angle = _base_degrees_angle;

    for(int index = 0, limit = bn::display::height() / 2; index < limit; ++index)
    {
        //degrees_angle increment controls wave frequency
        degrees_angle += _frequency;

        if(degrees_angle >= 360)
        {
            degrees_angle -= 360;
        }
        
        bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * _amplitude_ease.current();
        _horizontal_deltas[(bn::display::height() / 2) + index] = desp;
        _horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
    }

    bn::fixed percent = _amplitude_ease.ease_pct();
    if(_reverse){
        percent = 1 - percent;
    }else{

    }
    if(_amplitude_ease.time_elapsed() == 2){
        bn::blending::set_white_fade_color();
        //todo: this will not necessarily be the right palette
        _sprite.set_palette(bn::sprite_items::aru.palette_item());
        _sprite.set_blending_enabled(true);
    }    

    if(percent == 1){
        _sprite.set_palette(common_stuff::monochrome_palette(bn::color(31,31,31)));
    }else{
        BN_LOG("setting fade alpha for aru sprite", percent);
        bn::blending::set_fade_alpha(percent);
    }

    _horizontal_position_hbe.reload_deltas_ref();
    
}

}