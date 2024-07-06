#pragma once
#include "selection_box.h"
#include "common_stuff.h"

namespace aru { 
    class upgrade_selection_box : public selection_box {
        private:
            uint8_t &_upgrade_lvl;
            uint16_t &_ultramatter;
            const uint16_t _price; 
            const bn::string<16> _upgrade_type;
            bool _upgraded, _not_enough_money;
            
        public:
            upgrade_selection_box(bn::sprite_text_generator &text_generator,
                uint8_t &upgrade_lvl, uint16_t &ultramatter, const uint16_t &price,
                const bn::string<16> &upgrade_type);

            virtual void update();
            virtual bn::unique_ptr<box> next_box();
    };
}