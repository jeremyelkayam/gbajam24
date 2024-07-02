#include "common_stuff.h"
#include "cute_prop_sprite_font.h"
namespace aru { 


common_stuff::common_stuff() : 
    text_generator(cute_prop_sprite_font) { 

    //DEFAULT format tag
    bn::array<char, 8> default_format_tag;
    bn::istring_base default_format_tag_istring(default_format_tag._data);
    bn::ostringstream default_format_tag_stream(default_format_tag_istring);
    default_format_tag_stream.append("ARUSAV0");

    bn::sram::read(savefile);
    //if the format tag is invalid, then we have to set up our default save file.
    if(savefile.format_tag != default_format_tag){
        savefile.format_tag = default_format_tag;
        savefile.ultramatter = 10000;
        savefile.hover_upgrade_lvl = 0;
        savefile.shoot_upgrade_lvl = 0;
        savefile.sword_upgrade_lvl = 0;
    }
}

void common_stuff::set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible) {
    for(bn::sprite_ptr &sprite : sprites) {
        sprite.set_visible(visible);
        sprite.put_above();
    }
}

void common_stuff::bound(uint16_t &num, const uint16_t &min, const uint16_t &max){
    BN_ASSERT(min < max);
    if(num < min){
        num = min;
    }
    if(max < num){
        num = max;
    }
}

}