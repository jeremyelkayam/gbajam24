#include "common_stuff.h"
#include "cute_prop_sprite_font.h"
#include "small_numbers_sprite_font.h"
#include "bn_log.h"
namespace aru { 


common_stuff::common_stuff() : 
    text_generator(cute_prop_sprite_font),
    rising_text_generator(small_numbers_sprite_font) { 

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
        savefile.total_donated = 0;
        savefile.hover_upgrade_lvl = 0;
        savefile.shoot_upgrade_lvl = 0;
        savefile.sword_upgrade_lvl = 0;
        savefile.current_level = 0;
    }
}

void common_stuff::set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible) {
    for(bn::sprite_ptr &sprite : sprites) {
        sprite.set_visible(visible);
        sprite.put_above();
    }
}

uint16_t common_stuff::bound(const uint16_t &num, const uint16_t &min, const uint16_t &max){
    BN_ASSERT(min < max);
    if(num < min){
        return min;
    }
    if(max < num){
        return max;
    }
    return num;
}

uint16_t common_stuff::bounded_subtraction(const uint16_t &subtractee, const uint16_t &subtractor, const uint16_t &min){
    if(subtractee < subtractor){
        return min;
    }else{
        uint16_t sub_result = subtractee - subtractor;
        if(sub_result < min) return min;
        return sub_result;
    }
}

uint16_t common_stuff::bounded_addition(const uint16_t &first, const uint16_t &second,  const uint16_t &max){
    if(65535 - first < second) return max;
    uint16_t addition_result = first + second;
    if(addition_result > max) return max;
    return addition_result;
}

//todo: full implementation of this 
uint16_t common_stuff::loop_subtraction(const uint16_t &subtractee, 
    const uint16_t &subtractor, const uint16_t &min, const uint16_t &max){
    if(subtractee < subtractor){
        return min;
    }else{
        uint16_t sub_result = subtractee - subtractor;
        if(sub_result < min) return min;
        return sub_result;
    }
}

uint16_t common_stuff::loop_addition(const uint16_t &first, const uint16_t &second,  
    const uint16_t &min, const uint16_t &max){
    if(65535 - first < second) return max;
    uint16_t addition_result = first + second;
    if(addition_result > max) return max;
    return addition_result;
}

bn::string<256> common_stuff::append(const char *str, 
    const uint16_t &num)
{
    bn::string<256> text;
    bn::ostringstream stream(text);
    stream << str;
    stream << num;
    return text;
}

bn::sprite_palette_ptr common_stuff::monochrome_palette(const bn::color &color){

    bn::color colors[16];
    for(bn::color &element : colors){
        element = color;
    }

    return bn::sprite_palette_ptr::create(bn::sprite_palette_item(bn::span(colors), bn::bpp_mode::BPP_4));
}


}