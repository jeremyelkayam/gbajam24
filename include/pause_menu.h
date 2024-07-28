#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_text_generator.h>

namespace aru {

class pause_menu {
    private:
        bn::regular_bg_ptr _box;
        bn::vector<bn::sprite_ptr, 4> _text_sprites;
        bn::vector<bn::vector<bn::sprite_ptr, 2>, 4> _options;
        uint8_t _selected;
    public:
        pause_menu(bn::sprite_text_generator &text_generator);
        bool update();

};

}