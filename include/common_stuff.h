#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_string.h>
#include "constants.h"

namespace aru {

    struct saved_data {
        bn::array<char, 8> format_tag;
        uint16_t ultramatter;
    };

    class common_stuff { 
        public:
            bn::sprite_text_generator text_generator;
            bn::random rand;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}

            common_stuff();
            void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);

        private:

    };
}