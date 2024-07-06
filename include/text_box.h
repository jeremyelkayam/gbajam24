#pragma once
#include "box.h"

namespace aru { 
    class text_box : public box {
        private:
            bn::optional<bn::sprite_ptr> _portrait;
            bn::sprite_ptr _next_prompt;
            bn::vector<bn::string<64>, 24> _text;
            bool _top_box;
            uint8_t _current_line,_arrowtimer;
            void setup_text_sprites();
            void advance();
            virtual void set_visible(bool visible);

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text, 
                bool top_box);
            text_box(bn::sprite_text_generator &text_generator, const char *text, 
                const bn::sprite_item &portrait, bool top_box, bool rf_portrait);
            void update();
            virtual bool input_required() {return false;}

    };
}