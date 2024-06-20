#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_sprite_actions.h>

namespace aru { 
    class text_box {
        private:
            bn::sprite_text_generator &_text_generator;
            bn::sprite_ptr _portrait, _next_prompt;
            bn::vector<bn::sprite_ptr, 256> _text_sprites;
            bn::regular_bg_ptr _box;
            bn::vector<bn::string<64>, 24> _text;
            bool _done;
            uint8_t _current_line,_arrowtimer;
            void setup_text_sprites();

            /*
             * Formats a longer string troginto up to 3 separate strings, separated
             * automatically based on width.
             * 
             * Throws an error if the string is too big to fit into 3 lines (1 text box).
             */
            bn::vector<bn::string<64>, 24> split_into_lines(const char *text);

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text, const bn::sprite_item &portrait);
            bool done() {return _done;}
            void update();
            void advance();

    };
}