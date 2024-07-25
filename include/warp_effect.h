#pragma once
#include <bn_value_template_actions.h>
#include <bn_sprite_position_hbe_ptr.h>
#include <bn_display.h>

namespace aru{

class warp_effect{ 
    private:
        bn::fixed _frequency, _amplitude, _speed, _base_degrees_angle;
        // bn::to_value_template_action _amplitude_to;
        bn::array<bn::fixed, bn::display::height()> _horizontal_deltas;
        bn::sprite_position_hbe_ptr _horizontal_position_hbe;

    public:
        warp_effect(bn::sprite_ptr &sprite);

        void update();

};

}