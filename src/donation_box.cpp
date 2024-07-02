#include "donation_box.h"
#include "constants.h"
#include <bn_log.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_cute_prop_font.h"
#include "bn_keypad.h"
#include "common_stuff.h"

namespace aru { 

donation_box::donation_box(bn::sprite_text_generator &text_generator, const uint16_t &ultramatter) : 
    _text_generator(text_generator),
    _box(bn::regular_bg_items::textbox.create_bg(0, -104)),
    _place(0),
    _donation_amount(0),
    _ultramatter(ultramatter) {
    _box.set_priority(2);

    _text_generator.set_bg_priority(0);
    _text_generator.set_center_alignment();
    _text_generator.set_one_sprite_per_character(false);


    _text_generator.generate(0, _box.y() + 38, "Donate how much?", _text_sprites);

    BN_LOG("donation amount: ", _donation_amount);
    BN_LOG("fixedwidth str : ", to_string_fixed_width(_donation_amount));
    update_text_sprites();
    
}

void donation_box::update(){
    if(bn::keypad::left_pressed() && _place < 5){
        ++_place;
    }
    if(bn::keypad::right_pressed() && _place > 0){
        --_place;
    }    

    if(bn::keypad::up_pressed()) _donation_amount += ten_to_the(_place);
    if(bn::keypad::down_pressed()) _donation_amount -= ten_to_the(_place);


    if(bn::keypad::up_pressed() || bn::keypad::down_pressed()){
        common_stuff::bound(_donation_amount, 0, _ultramatter);

        update_text_sprites();
        BN_LOG("updating...");

    }    
}

uint16_t donation_box::ten_to_the(uint8_t pow){
    uint16_t result = 1;
    for(uint8_t z = 0; z < pow; ++z){
        result*=10;
    }
    return result;
}

// https://cplusplus.com/forum/beginner/276241/
bn::string<6> donation_box::to_string_fixed_width(uint16_t n)
{   // this version will truncate any digits past the width
    bn::string<5> result = "00000";
    for (int i = 5; i--; n /= 10) result[i] = char('0' + n % 10);
    return result;
}

}