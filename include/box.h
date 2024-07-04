#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_sprite_actions.h>

namespace aru { 
    class box {
        private:
            bn::sprite_text_generator &_text_generator;
            bn::vector<bn::sprite_ptr, 256> _text_sprites;
            bn::regular_bg_ptr _box;
            bool _done;

            void setup_text_sprites();

        public:
            box(bn::sprite_text_generator &text_generator);
            virtual ~box() {return;} //ayy, oracle vm where u at
            virtual bool done() {return _done;}
            virtual void update()=0;

    };
}