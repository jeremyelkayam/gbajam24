#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_string.h>

namespace aru {

    struct saved_data {
        bn::array<char, 8> format_tag;
        uint16_t ultramatter, donated_amount;
        uint8_t hover_upgrade_lvl, shoot_upgrade_lvl, sword_upgrade_lvl, current_level;
    };

    class common_stuff { 
        public:
            bn::sprite_text_generator text_generator;
            bn::random rand;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}

            common_stuff();
            static void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);

            static uint16_t bound(const uint16_t &num, const uint16_t &min, const uint16_t &max);
            static uint16_t bounded_subtraction(const uint16_t &subtractee, const uint16_t &subtractor,  const uint16_t &min);
            static uint16_t bounded_addition(const uint16_t &first, const uint16_t &second,  const uint16_t &max);

        private:

    };
}