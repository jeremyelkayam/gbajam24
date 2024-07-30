#pragma once
#include "player.h"
#include <bn_blending_actions.h>

namespace aru{



class animated_sprite
{
    public: 
        animated_sprite(bn::camera_ptr &cam, const bn::sprite_item &spritem, const bn::fixed &x, const bn::fixed &y);
        bool done(){return _anim.done();}
        void reset(const bn::fixed &x, const bn::fixed &y);
        void update(){_anim.update();}

    private:
        bn::sprite_ptr _sprite;
        bn::sprite_animate_action<7> _anim;
};

class death_anim{ 

    public:

        death_anim(player &player);

        void update();

        bool done() const;

    private:
        player &_player;

        bn::vector<animated_sprite, 3> _explosions;
        bn::optional<bn::blending_fade_alpha_to_action> _fade_action;

};

}