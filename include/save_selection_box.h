#pragma once
#include "selection_box.h"
#include "common_stuff.h"

namespace aru { 
    class save_selection_box : public selection_box {
        private:
            common_stuff &_cstuff;
            bn::string<256> _next_text;

        public:
            save_selection_box(common_stuff &cstuff, uint8_t anim_index = 0);
            virtual void update();
            virtual bn::unique_ptr<box> next_box();
    };
}