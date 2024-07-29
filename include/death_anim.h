#pragma once
#include "player.h"
#include <bn_blending_actions.h>

namespace aru{

class death_anim{ 

    public:

        death_anim(player &player);

        void update();

        bool done() const;

    private:
        player &_player;

        bn::vector<bn::sprite_ptr, 3> _explosions;
        bn::vector<bn::sprite_animate_action<7>, 3> _explosion_anims;
        bn::blending_fade_alpha_to_action _fade_action;



};

}