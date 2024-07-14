#include "selection_box.h"
#include "constants.h"
#include "common_stuff.h"
#include <bn_log.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_cute_prop_font.h"
#include "bn_keypad.h"

namespace aru { 

selection_box::selection_box(bn::sprite_text_generator &text_generator, const bn::string<64> &text, const bn::vector<bn::string<8>,4> &options) : 
    box(text_generator),
    _options(options),
    _text(text),
    _selected_option(0) {
}
void selection_box::update(){
    if(!_box){
        init();
    }

    bn::fixed xcor = _option_sprites.at(_selected_option).x() - 20;
    bn::fixed ycor = _option_sprites.at(_selected_option).y();

    if(bn::keypad::left_pressed()) --_selected_option;
    if(bn::keypad::right_pressed()) ++_selected_option;

    if(bn::keypad::left_pressed() || bn::keypad::right_pressed()) {
        _selected_option = _selected_option % _option_sprites.size();
    }
    _selector->set_position(xcor, ycor);

    if(bn::keypad::a_pressed()) {
        BN_LOG("selected option: ", _options.at(_selected_option));
    }
}

void selection_box::set_visible(bool visible){
    box::set_visible(visible);
    common_stuff::set_sprite_arr_visible(_option_sprites, visible);
    if(_selector) _selector->set_visible(visible);
}

void selection_box::init(){
    BN_LOG("run init function");
    box::init();
    _box->set_y(-50);
    _selector = bn::sprite_items::cute_prop_font.create_sprite_optional(0,0,29);
    _selector->set_bg_priority(0);


    _text_generator.set_bg_priority(0);
    _text_generator.set_center_alignment();
    _text_generator.set_one_sprite_per_character(false);

    _text_generator.generate(0, -10, _text, _text_sprites);
    bn::fixed spacing = 30;
    bn::fixed start_x = -spacing * bn::fixed(0.5) * (_options.size() - 1);

    for(int z = 0; z < _options.size(); ++z){
        _text_generator.generate(start_x + spacing * z, 10, _options.at(z), _option_sprites);
    }
}

}