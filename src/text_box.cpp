#include "text_box.h"
#include "constants.h"
#include "cute_prop_sprite_font.h"
#include <bn_log.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_downarrow.h"

namespace aru { 

text_box::text_box(bn::sprite_text_generator &text_generator, const char *text, const bn::sprite_item &portrait) : 
    _text_generator(text_generator),
    _portrait(portrait.create_sprite(-80,0)),
    _next_prompt(bn::sprite_items::downarrow.create_sprite(TB_ARROW_X,TB_ARROW_Y)),
    _box(bn::regular_bg_items::textbox.create_bg(0, 0)),
    _done(false),
    _current_line(0),
    _arrowtimer(0) {
    _box.set_priority(2);
    _portrait.set_bg_priority(0);
    _next_prompt.set_bg_priority(0);
    _next_prompt.set_visible(false);
    _text = split_into_lines(text);
    setup_text_sprites();
}

void text_box::setup_text_sprites(){
    _text_sprites.clear();
    _text_generator.set_bg_priority(0);
    _text_generator.set_left_alignment();
    _text_generator.set_one_sprite_per_character(true);
    for(uint8_t i = _current_line; (i < _current_line+3) && (i < _text.size()); i++ ){    
        _text_generator.generate(-112, 38 + (i - _current_line)*14, _text.at(i), _text_sprites);
    }
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_visible(false);
    }
}

void text_box::update(){
    for(bn::sprite_ptr &sprite : _text_sprites){
        if(!sprite.visible()){
            sprite.set_visible(true);
            return;
        }
    }
    if(_text_sprites.at(_text_sprites.size()-1).visible()){
        if(_arrowtimer >= TB_ARROW_OTIME || !_next_prompt.visible()){
            _arrowtimer = 0;
        }
        _next_prompt.set_visible(true);
        ++_arrowtimer;

        uint8_t multiplier = _arrowtimer;
        if(_arrowtimer >= TB_ARROW_OTIME * bn::fixed(0.5)){
            multiplier = TB_ARROW_OTIME - _arrowtimer;
        }

        _next_prompt.set_y(TB_ARROW_Y + TB_ARROW_INC_PER_FRAME * multiplier);     
    }
}

void text_box::advance(){
    if(!_next_prompt.visible()){
        for(bn::sprite_ptr &sprite : _text_sprites){
            sprite.set_visible(true);
        }
    }else if(!(_current_line + 3 >= _text.size())){
        _current_line += 3;
        _next_prompt.set_visible(false);
        setup_text_sprites();
    }else{
        _done = true;
    }
}


bn::vector<bn::string<64>, 24> text_box::split_into_lines(const char *text){
    const uint8_t max_line_width = 218;
    bn::vector<bn::string<64>, 24>result;
    
    uint8_t line_width = 0;
    uint8_t line_num = 0;
    uint16_t line_start = 0;
    uint16_t line_end = 0;
    for(uint16_t pos = 0; text[pos] != '\0'; ++pos){
        char c = text[pos];
        line_width += cute_prop_sprite_font_character_widths[c - ' '];
        if(line_width > max_line_width){
            uint16_t last_space = pos;
            while(text[last_space] != ' '){
                //Find the last space
                last_space--;
            }
            line_end = last_space;
        }

        //this case handles the end of the string (\0)
        //pos + 1 here because the for loop won't process the end-of-string character
        if(text[pos + 1] == '\0') line_end = pos + 1;

        //this case lets us handle manual newlines (\n)
        if(text[pos] == '\n') line_end = pos;

        //if this variable is set, we know where the end of the line is
        if(line_end){
            //copy the current line, up to the last space, into the result vector
            bn::string<64> line;
            bn::ostringstream line_stream(line);
            for(uint16_t i = line_start; i < line_end; i++){
                line_stream << text[i];
            }
            result.emplace_back(line);

            if(text[pos + 1] != '\0'){
                pos = line_end + 1;
            }
            line_start = pos;

            //we are effectively un-setting these variables by zeroing them
            line_width = 0;
            line_end = 0;

            ++line_num;
        }

    }

    return result;
}

}