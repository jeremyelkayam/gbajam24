#pragma once
#include "box.h"

namespace aru { 
    class selection_box : public box {
        protected:
            bn::optional<bn::sprite_ptr> _selector;
            bn::vector<bn::sprite_ptr, 4> _option_sprites;
            bn::vector<bn::string<8>, 4> _options;
            uint8_t _selected_option;

        public:
            selection_box(bn::sprite_text_generator &text_generator, 
                const bn::string<64> &text, const bn::vector<bn::string<8>,4> &options);
            virtual void update();
            virtual void set_visible(bool visible);
            bn::string<8> selected() {return _options.at(_selected_option);}
            virtual void init();
    };
}