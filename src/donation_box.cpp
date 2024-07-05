#include "donation_box.h"
#include "constants.h"
#include <bn_log.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_cute_prop_font.h"
#include "bn_keypad.h"
#include "common_stuff.h"

namespace aru { 

donation_box::donation_box(bn::sprite_text_generator &text_generator, uint16_t &ultramatter) : 
    box(text_generator),
    _place(0),
    _hold_timer(0),
    _blink_timer(0),
    _donation_amount(0),
    _ultramatter(ultramatter) {

    _box.set_y(-104);
    update_text_sprites();
    

    _selectors.emplace_back(bn::sprite_items::cute_prop_font.create_sprite(0,_box.y() + 46,61)),
    _selectors.emplace_back(bn::sprite_items::cute_prop_font.create_sprite(0,_box.y() + 55,61)),
    _selectors.at(1).set_vertical_flip(true);

    _text_generator.generate(0, _box.y() + 38, "Donate how much?", _text_sprites);
    _text_generator.generate(0, _box.y() + 64, "Press A to confirm or B to cancel", _text_sprites);

    set_visible(false);
    
}

void donation_box::update(){
    if(bn::keypad::left_pressed() && _place < 4){
        ++_place;
    }
    if(bn::keypad::right_pressed() && _place > 0){
        --_place;
    }    

    for(bn::sprite_ptr &selector : _selectors){
        selector.set_bg_priority(0);
        selector.set_x(13 - (6 * _place));
        if(_blink_timer == 30){
            selector.set_visible(true);
        }else if(_blink_timer == 0){
            selector.set_visible(false);
        }
    }


    if(bn::keypad::down_held() || bn::keypad::up_held()){
        ++_hold_timer;

        if(_hold_timer >=210) _hold_timer = 30;
    }
    if(bn::keypad::down_released() || bn::keypad::up_released()) _hold_timer = 0;


    if(bn::keypad::up_pressed() || 
      (bn::keypad::up_held() && _hold_timer >= 30 && _hold_timer % 3 == 0)){

        _donation_amount = common_stuff::bounded_addition(_donation_amount, ten_to_the(_place), _ultramatter);

        _selectors.at(0).set_y(DB_USEL_Y - 1);

    }else{

        _selectors.at(0).set_y(DB_USEL_Y);

    }
    if(bn::keypad::down_pressed() || 
      (bn::keypad::down_held() && _hold_timer >= 30 && _hold_timer % 3 == 0)){

        _donation_amount = common_stuff::bounded_subtraction(_donation_amount, ten_to_the(_place), 0);

        _selectors.at(1).set_y(DB_DSEL_Y + 1);

    }else{
        //todo: maybe make it last more than one frame idk
        _selectors.at(1).set_y(DB_DSEL_Y);
    }

    update_text_sprites();    

    ++_blink_timer;

    if(_blink_timer > 60){
        _blink_timer = 0;
    }

    if(bn::keypad::a_pressed()){
        _done = true;
        //cannot donate more than what you've got on hand...
        BN_ASSERT(_donation_amount < _ultramatter);
        _ultramatter -= _donation_amount;
    }

    if(bn::keypad::a_pressed() || bn::keypad::b_pressed()){
        _done = true;
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

void donation_box::update_text_sprites(){
    _text_generator.set_bg_priority(0);
    _text_generator.set_center_alignment();
    _text_generator.set_one_sprite_per_character(false);

    _donation_amount_sprites.clear();
    _text_generator.generate(0, _box.y() + 50, to_string_fixed_width(_donation_amount), _donation_amount_sprites);

}

void donation_box::set_visible(bool visible){
    box::set_visible(visible);
    common_stuff::set_sprite_arr_visible(_selectors, visible);
    common_stuff::set_sprite_arr_visible(_donation_amount_sprites, visible);
}


}