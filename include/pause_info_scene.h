#pragma once
#include "common_stuff.h"
#include "scene.h"
#include <bn_regular_bg_ptr.h>
#include <bn_blending_actions.h>

namespace aru{

class pause_info_scene : public scene{ 
protected:

    bn::vector<bn::sprite_ptr, 64> _text_sprites;
    bn::optional<bn::blending_transparency_alpha_to_action> _trans_alpha_action;
    bn::regular_bg_ptr _bg;
 
public:
    pause_info_scene(bn::sprite_text_generator &generator,
        const common_stuff::save_file &savefile);
    [[nodiscard]] virtual bn::optional<scene_type> update();
    virtual void set_transition_effects_enabled(bool enabled);
};

}