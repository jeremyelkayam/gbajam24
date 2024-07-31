#pragma once
#include "player.h"
#include <bn_blending_actions.h>
#define NUM_EXPLOSIONS 16

namespace aru{



class animated_sprite
{
    public: 
        animated_sprite(bn::camera_ptr &cam, const bn::sprite_item &spritem, 
            const bn::fixed_point &pos);
        bool done() const {return _anim.done();}
        void reset(const bn::fixed_point &pos);
        void update(){_anim.update();}
        uint8_t current_index() const {return _anim.current_index();}

    private:
        bn::sprite_ptr _sprite;
        bn::sprite_animate_action<7> _anim;
};

class death_anim{ 

    public:

        death_anim(player &player, bn::camera_ptr &cam);

        void update();

        bool done() const;
        bool initial_fade_done() const {return _initial_fade_done;}

    private:
        player &_player;
        bn::camera_ptr &_cam;
        bool _initial_fade_done;

        static constexpr const bn::fixed_point EXPLOSION_COORDS[NUM_EXPLOSIONS] = 
        {
            bn::fixed_point(-10, 10),
            bn::fixed_point(10, -10),
            bn::fixed_point(10, 10),
            bn::fixed_point(-10, -10),
            bn::fixed_point(-10, 10),
            bn::fixed_point(10, -10),
            bn::fixed_point(10, 10),
            bn::fixed_point(-10, -10),
            bn::fixed_point(-10, 10),
            bn::fixed_point(10, -10),
            bn::fixed_point(10, 10),
            bn::fixed_point(-10, -10),
            bn::fixed_point(-10, 10),
            bn::fixed_point(10, -10),
            bn::fixed_point(10, 10),
            bn::fixed_point(-10, -10)
        };

        bn::forward_list<animated_sprite, 3> _explosions;
        bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
        bn::deque<bn::fixed_point, NUM_EXPLOSIONS> _explosion_coords;

        static bool anim_spr_done(const animated_sprite &as) { return as.done();}


        
};

}