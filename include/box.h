#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_unique_ptr.h>
#include <bn_string.h>
#include <bn_sprite_actions.h>

namespace aru { 
    class box {
        protected:
            bn::sprite_text_generator &_text_generator;
            bn::vector<bn::sprite_ptr, 256> _text_sprites;
            bn::optional<bn::regular_bg_ptr> _box;
            bool _done;
            uint8_t _anim_index;

            /*
             * Formats a longer string into up to 24 separate strings, separated
             * automatically based on width.
             * 
             * Throws an error if the string is too big to fit into 24 lines 
             * (8 text boxes).
             */
            bn::vector<bn::string<64>, 24> split_into_lines(const char *text);


        public:
            box(bn::sprite_text_generator &text_generator, uint8_t anim_index = 0);
            virtual ~box() {return;} //ayy, oracle vm where u at
            virtual bool done() {return _done;}
            virtual void update()=0;
            virtual void set_visible(bool visible);
            virtual bn::unique_ptr<box> next_box();
            uint8_t anim_index() const {return _anim_index;}

            virtual bool input_required() const {return true;}

            virtual void init();

    };
}