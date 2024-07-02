#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>

namespace aru { 
    class selection_box {
        private:
            bn::sprite_text_generator &_text_generator;
            bn::sprite_ptr _selector;
            bn::vector<bn::sprite_ptr, 256> _text_sprites;
            bn::vector<bn::sprite_ptr, 4> _option_sprites;
            bn::regular_bg_ptr _box;
            uint8_t _selected_option;

        public:
            selection_box(bn::sprite_text_generator &text_generator, const char *text);
            void update();
    };
}