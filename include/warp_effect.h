#pragma once
#include <bn_value_template_actions.h>
#include <bn_sprite_position_hbe_ptr.h>
#include <bn_display.h>
#include "easer.h"
#include "player.h"

namespace aru{

class warp_effect{ 
    private:
        bool _reverse;
        bn::fixed _frequency, _speed, _base_degrees_angle;
        bn::array<bn::fixed, bn::display::height()> _horizontal_deltas;
        bn::sprite_position_hbe_ptr _horizontal_position_hbe;
        easer _amplitude_ease, _height_ease;
        player &_player;

    public:
        warp_effect(player &player, bool reverse);

        void update();

        bool done() {return _amplitude_ease.done() && _height_ease.done();}

};

}