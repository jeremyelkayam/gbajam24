#include "box.h"
#include "common_stuff.h"
#include "bn_regular_bg_items_textbox.h"
#include "cute_prop_sprite_font.h"
#include <bn_log.h>

namespace aru {

box::box(bn::sprite_text_generator &text_generator, uint8_t anim_index) : 
    _text_generator(text_generator),
    _done(false),
    _anim_index(anim_index)  {
    set_visible(false);
}

bn::vector<bn::string<64>, 24> box::split_into_lines(const char *text){
    
    const uint8_t max_line_width = 218;
    bn::vector<bn::string<64>, 24>result;
    
    uint8_t line_width = 0;
    uint8_t line_num = 0;
    uint16_t line_start = 0;
    uint16_t line_end = 0;
    for(uint16_t pos = 0; text[pos] != '\0'; ++pos){
        if(text[pos] == '$'){
            ++pos;
            BN_ASSERT(text[pos] != '\0');
            ++pos;
        }
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

void box::set_visible(bool visible){
    common_stuff::set_sprite_arr_visible(_text_sprites, visible);
    if(_box){
        _box->set_visible(visible);
    }
}

bn::unique_ptr<box> box::next_box(){
    // BN_ASSERT(_done);
    return bn::unique_ptr<box>();
}

void box::init(){
    _box = bn::regular_bg_items::textbox.create_bg_optional(0, 0);
    _box->set_priority(2);
    _anim.emplace(bn::create_regular_bg_cached_animate_action_forever(
        *_box, 6, bn::regular_bg_items::textbox.map_item(), 0, 1, 2, 3));
}

}