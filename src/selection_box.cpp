#include "selection_box.h"
#include "constants.h"
#include <bn_log.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_cute_prop_font.h"
#include "bn_keypad.h"

namespace aru { 

selection_box::selection_box(bn::sprite_text_generator &text_generator, const char *text) : 
    _text_generator(text_generator),
    _selector(bn::sprite_items::cute_prop_font.create_sprite(0,0,29)),
    _box(bn::regular_bg_items::textbox.create_bg(0, -50)),
    _selected_option(0) {
    _box.set_priority(2);
    _selector.set_bg_priority(0);

    _text_generator.set_bg_priority(0);
    _text_generator.set_center_alignment();
    _text_generator.set_one_sprite_per_character(false);


    _text_generator.generate(0, -10, text, _text_sprites);

    _text_generator.generate(-20, 10, "YES", _option_sprites);
    _text_generator.generate(20, 10, "NO", _option_sprites);
    BN_LOG("number of sprites: ", _option_sprites.size());
}
void selection_box::update(){
    //will also need to do one for the donation box
    bn::fixed xcor = _option_sprites.at(_selected_option).x() - 20;
    bn::fixed ycor = _option_sprites.at(_selected_option).y();

    if(bn::keypad::left_pressed()) --_selected_option;
    if(bn::keypad::right_pressed()) ++_selected_option;

    if(bn::keypad::left_pressed() || bn::keypad::right_pressed()) {
        _selected_option = _selected_option % _option_sprites.size();
    }
    _selector.set_position(xcor, ycor);

    if(bn::keypad::a_pressed()) {
        BN_LOG("selected option: ", _selected_option);
    }
}

}