#include "text_box.h"
#include "constants.h"
#include <bn_log.h>
#include <bn_keypad.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_downarrow.h"

namespace aru { 

text_box::text_box(bn::sprite_text_generator &text_generator, const char *text, 
    bool top_box) : 
        box(text_generator),
        _top_box(top_box),
        _current_line(0),
        _arrowtimer(0) {
    _text = split_into_lines(text);
}

text_box::text_box(bn::sprite_text_generator &text_generator, const char *text, 
    const bn::sprite_item &portrait, bool top_box, bool rf_portrait, uint8_t anim_index) : 
        text_box(text_generator, text, top_box) {
    _anim_index = anim_index;
    _portrait.emplace(portrait.create_sprite(-76,0));

    _portrait->set_bg_priority(0);

    if(rf_portrait){
        _portrait->set_horizontal_flip(true);
        _portrait->set_x(-_portrait->x());
    }
}

void text_box::setup_text_sprites(){
    _text_sprites.clear();
    _text_generator.set_bg_priority(0);
    _text_generator.set_left_alignment();
    _text_generator.set_one_sprite_per_character(true);
    for(uint8_t i = _current_line; (i < _current_line+3) && (i < _text.size()); i++ ){    

        _text_generator.generate(_box->x() - 112,_box->y() + 38 + (i - _current_line)*14 - (_top_box ? 3 : 0), _text.at(i), _text_sprites);
    }
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_visible(false);
    }
}

void text_box::update(){
    if(!_box){
        init();
    }

    if(bn::keypad::a_pressed()){
        advance();
    }
    
    for(bn::sprite_ptr &sprite : _text_sprites){
        if(!sprite.visible()){
            sprite.set_visible(true);
            return;
        }
    }
    if(_text_sprites.at(_text_sprites.size()-1).visible()){
        if(_arrowtimer >= TB_ARROW_OTIME || !_next_prompt->visible()){
            _arrowtimer = 0;
        }
        _next_prompt->set_visible(true);
        ++_arrowtimer;

        uint8_t multiplier = _arrowtimer;
        if(_arrowtimer >= TB_ARROW_OTIME * bn::fixed(0.5)){
            multiplier = TB_ARROW_OTIME - _arrowtimer;
        }

        _next_prompt->set_y(_box->y() + TB_ARROW_Y + TB_ARROW_INC_PER_FRAME * multiplier);     
    }
}

void text_box::advance(){
    if(!_next_prompt->visible()){
        for(bn::sprite_ptr &sprite : _text_sprites){
            sprite.set_visible(true);
        }
    }else if(!(_current_line + 3 >= _text.size())){
        _current_line += 3;
        _next_prompt->set_visible(false);
        setup_text_sprites();
    }else{
        _done = true;
    }
}

void text_box::set_visible(bool visible){    
    if(_portrait) _portrait->set_visible(visible);
}

//todo: CHANGE ALL GRAPHICAL RESOURCES TO OPTIONAL AND SET THEM UP IN THE INIT FUNCTION

void text_box::init(){
    _box = bn::regular_bg_items::textbox.create_bg_optional(0, _top_box ? -104 : 0);
    _box->set_priority(2);
    setup_text_sprites();
    _next_prompt = bn::sprite_items::downarrow.create_sprite_optional(TB_ARROW_X,TB_ARROW_Y);
    _next_prompt->set_bg_priority(0);
    _next_prompt->set_visible(false);
}

}