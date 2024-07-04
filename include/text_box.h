#pragma once
#include "box.h"

namespace aru { 
    class text_box : public box {
        private:
            bn::sprite_ptr _portrait, _next_prompt;
            bn::vector<bn::string<64>, 24> _text;
            bool _top_box;
            uint8_t _current_line,_arrowtimer;
            void setup_text_sprites();
            void advance();

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text, const bn::sprite_item &portrait, bool top_box, bool rf_portrait);
            void update();

    };
}