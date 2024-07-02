#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_sprite_actions.h>

namespace aru { 
    class donation_box {
        private:
            bn::sprite_text_generator &_text_generator;
            bn::vector<bn::sprite_ptr, 16> _text_sprites;
            bn::vector<bn::sprite_ptr, 8> _donation_amount_sprites;
            bn::regular_bg_ptr _box;
            bn::vector<bn::sprite_ptr, 2> _selectors;
            uint8_t _place, _hold_timer, _blink_timer;
            uint16_t _donation_amount;
            const uint16_t &_ultramatter;
        
            void update_text_sprites() {_donation_amount_sprites.clear(); _text_generator.generate(0, _box.y() + 50, to_string_fixed_width(_donation_amount), _donation_amount_sprites);}

            uint16_t ten_to_the(uint8_t pow);
            char to_char(uint8_t digit) {BN_ASSERT(digit < 10); return digit + 48;}

            bn::string<6> to_string_fixed_width(uint16_t n);
        public:
            donation_box(bn::sprite_text_generator &text_generator, const uint16_t &ultramatter);
            void update();

    };
}