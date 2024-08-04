#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_regular_bg_actions.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"

namespace aru{

class sound_test_scene : public scene{ 
private:
    bn::vector<bn::sprite_ptr, 16> _title_text_sprites;
    bn::vector<bn::sprite_ptr, 4> _track_number_text_sprites;
    bn::sprite_text_generator _gen;
    uint8_t _index;
    bn::regular_bg_ptr _bg;
    bn::regular_bg_move_by_action _bg_move;
    void update_track_number();

public:
    sound_test_scene();
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
    virtual void set_transition_effects_enabled(bool enabled);

};

}