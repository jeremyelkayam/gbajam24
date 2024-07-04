#include "box.h"
#include "bn_regular_bg_items_textbox.h"

namespace aru {

box::box(bn::sprite_text_generator &text_generator) : 
    _text_generator(text_generator),
    _box(bn::regular_bg_items::textbox.create_bg(0, 0)),
    _done(false) {
    _box.set_priority(2);
}

}