#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_regular_bg_actions.h>
#include <bn_sprite_animate_actions.h>
#include "scene.h"

namespace aru{

class menu_scene : public scene{ 
private:
    bn::vector<bn::sprite_ptr, 64> _text_sprites;
    bn::sprite_ptr _selector;
    bn::sprite_animate_action<6> _selector_anim;
    uint8_t _index;
    bn::regular_bg_ptr _bg;
    bn::regular_bg_move_by_action _bg_move;
    


public:
    menu_scene();
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
    virtual void set_transition_effects_enabled(bool enabled);
};

}