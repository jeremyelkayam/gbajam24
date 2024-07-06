#pragma once
#include "box.h"

namespace aru { 
    class selection_box : public box {
        private:
            bn::sprite_ptr _selector;
            bn::vector<bn::sprite_ptr, 4> _option_sprites;
            bn::vector<bn::string<8>, 4> _options;
            uint8_t _selected_option;

        public:
            selection_box(bn::sprite_text_generator &text_generator, const char *text, const bn::vector<bn::string<8>,4> &options);
            void update();
            virtual void set_visible(bool visible);
    };
}