#pragma once
#include "box.h"

namespace aru { 
    class text_box : public box {
        private:
            bn::optional<bn::sprite_ptr> _portrait, _next_prompt;
            bn::optional<bn::sprite_item> _portrait_item;
            bn::vector<bn::string<64>, 24> _text;
            bool _top_box, _rf_portrait;
            uint8_t _current_line,_arrowtimer;
            void setup_text_sprites();
            void advance();
            virtual void set_visible(bool visible);

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text, 
                bool top_box);
            text_box(bn::sprite_text_generator &text_generator, const char *text, 
                const bn::sprite_item &portrait, bool top_box, bool rf_portrait, uint8_t anim_index = 0);
            void update();
            virtual bool input_required() const {return false;}
            uint8_t anim_index() const {return _anim_index;}
            virtual void init();

    };
}