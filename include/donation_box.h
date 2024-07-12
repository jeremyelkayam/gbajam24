#pragma once
#include "box.h"
#include "common_stuff.h"

namespace aru { 
    class donation_box : public box {
        private:
            bn::vector<bn::sprite_ptr, 8> _donation_amount_sprites;
            bn::vector<bn::sprite_ptr, 2> _selectors;
            uint8_t _place, _hold_timer, _blink_timer;
            uint16_t _donation_amount;
            uint16_t &_ultramatter, &_total_donated;
        
            void update_text_sprites();

            uint16_t ten_to_the(uint8_t pow);
            char to_char(uint8_t digit) {BN_ASSERT(digit < 10); return digit + 48;}

            bn::string<6> to_string_fixed_width(uint16_t n);
        public:
            donation_box(common_stuff &cstuff);
            virtual void update();

            virtual void set_visible(bool visible);

    };
}