#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_string.h>
#include <bn_math.h>
#include <bn_sprite_palette_ptr.h>
#include <bn_bg_palette_ptr.h>

namespace aru {

    class common_stuff { 
        public:
            struct saved_data {
                bn::array<char, 8> format_tag;
                uint16_t ultramatter, total_donated;
                uint8_t hover_upgrade_lvl, shoot_upgrade_lvl, sword_upgrade_lvl, current_level;
            }; 
                   
            bn::sprite_text_generator text_generator;
            bn::sprite_text_generator rising_text_generator;
            bn::random rand;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}

            common_stuff();
            static void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, 
                const bool &visible);
            
            static void set_sprite_arr_effects_enabled(bn::ivector<bn::sprite_ptr> &sprites, 
                const bool &enabled);

            [[nodiscard]] static uint16_t bound(const uint16_t &num, const uint16_t &min, 
                const uint16_t &max);
            [[nodiscard]] static uint16_t bounded_subtraction(const uint16_t &subtractee, 
                const uint16_t &subtractor,  const uint16_t &min);
            [[nodiscard]] static uint16_t bounded_addition(const uint16_t &first, 
                const uint16_t &second,  const uint16_t &max);

            [[nodiscard]] static uint16_t loop_addition(const int16_t &current, 
                const int16_t &additive,  const int16_t &min, const int16_t &max);

            static bn::string<256> append(const char *str, 
                const uint16_t &num);
            
            static bn::sprite_palette_ptr monochrome_sprite_palette(const bn::color &color);

            static bn::bg_palette_ptr monochrome_bg_palette(const bn::color &color);

        private:

    };
}