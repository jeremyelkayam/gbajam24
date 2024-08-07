#include "common_stuff.h"
#include "cute_prop_sprite_font.h"
#include "small_numbers_sprite_font.h"
#include <bn_log.h>
#include <bn_bg_palette_item.h>
namespace aru { 


common_stuff::common_stuff() : 
    text_generator(cute_prop_sprite_font),
    rising_text_generator(small_numbers_sprite_font),
    _current_save_index(0) { 

    //DEFAULT format tag
    bn::array<char, 8> default_format_tag;
    bn::istring_base default_format_tag_istring(default_format_tag._data);
    bn::ostringstream default_format_tag_stream(default_format_tag_istring);
    default_format_tag_stream.append("ARUSAV0");

    bn::sram::read(userdata);
    //if the format tag is invalid, then we have to set up our default save file.
    if(userdata.format_tag != default_format_tag){
        userdata.format_tag = default_format_tag;

        for(save_file &savefile : userdata.saves){
            savefile.ultramatter = 0;
            savefile.total_donated = 0;
            savefile.hover_upgrade_lvl = 0;
            savefile.shoot_upgrade_lvl = 0;
            savefile.sword_upgrade_lvl = 0;
            savefile.current_level = 0;
            savefile.playtime = 0;
            savefile.slash_on_b = false;
        }
    }
}

common_stuff::save_file &common_stuff::current_save()
{
    BN_ASSERT(_current_save_index < userdata.saves.size());
    return userdata.saves[_current_save_index];
}

void common_stuff::set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible) {
    for(bn::sprite_ptr &sprite : sprites) {
        sprite.set_visible(visible);
        sprite.put_above();
    }
}

void common_stuff::set_sprite_arr_effects_enabled(bn::ivector<bn::sprite_ptr> &sprites, 
        const bool &enabled) {
    for(bn::sprite_ptr &sprite : sprites) {
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
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

uint16_t common_stuff::loop_addition(const int16_t &current, const int16_t &additive,  
    const int16_t &min, const int16_t &max){
    BN_ASSERT(additive < max);
    if(additive > 0){
        return min + ((current + additive) % (max - min));
    }else{
        int16_t result = current + additive;
        if(result < min){
            result += max;
        }
        return result;
    }
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

bn::string<256> common_stuff::append(const uint16_t &num, 
    const char *str)
{
    bn::string<256> text;
    bn::ostringstream stream(text);
    stream << num;
    stream << str;
    return text;
}

bn::string<256> common_stuff::int_str(const uint16_t &num)
{
    bn::string<256> text;
    bn::ostringstream stream(text);
    stream << num;
    return text;
}

bn::sprite_palette_ptr common_stuff::monochrome_sprite_palette(const bn::color &color){

    bn::color colors[16];
    for(bn::color &element : colors){
        element = color;
    }

    return bn::sprite_palette_ptr::create(bn::sprite_palette_item(bn::span(colors), bn::bpp_mode::BPP_4));
}

bn::bg_palette_ptr common_stuff::monochrome_bg_palette(const bn::color &color){

    bn::color colors[16];
    for(bn::color &element : colors){
        element = color;
    }

    return bn::bg_palette_ptr::create(bn::bg_palette_item(bn::span(colors), bn::bpp_mode::BPP_4));
}


}