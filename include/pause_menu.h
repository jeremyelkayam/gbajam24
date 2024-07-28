#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"

namespace aru {

class pause_menu {
    private:
        struct menu_option {
            const char* text;
            scene_type next_scene;
        };
        static constexpr const menu_option _options[3] {
            {"QUIT TO TITLE", scene_type::MENU},
            {"EXIT TO LAB", scene_type::LAB},
            {"RETURN TO GAME", scene_type::BACK},
        };

        bn::regular_bg_ptr _box;
        bn::vector<bn::sprite_ptr, 4> _text_sprites;
        bn::vector<bn::vector<bn::sprite_ptr, 4>, 4> _option_text_sprites;
        uint8_t _selected;
        void update_colors();
    public:
        pause_menu(bn::sprite_text_generator &text_generator);
        bn::optional<scene_type> update();

};

}