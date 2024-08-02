#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sprite_text_generator.h>
#include "common_variable_8x16_sprite_font.h"
#include "bn_regular_bg_items_pause_info_bg.h"

#include "pause_info_scene.h"

namespace aru {

pause_info_scene::pause_info_scene(
    bn::sprite_text_generator &generator, 
    const common_stuff::save_file &savefile) : 
    _bg(bn::regular_bg_items::pause_info_bg.create_bg(0,0)) 
{
    _bg.set_priority(0);
    generator.set_center_alignment();
    generator.set_one_sprite_per_character(false);

    generator.generate(0, -65, "AUTOMATED RETRIEVAL UNIT", 
        _text_sprites);

    generator.generate(0, -55, "MODEL 133n", 
        _text_sprites);

    generator.generate(0, -35, 
            common_stuff::append("HOVER ABILITY LEVEL ", 
            savefile.hover_upgrade_lvl), 
        _text_sprites);

    generator.generate(0, -15, 
            common_stuff::append("SHOOT ABILITY LEVEL ", 
            savefile.shoot_upgrade_lvl), 
        _text_sprites);

    generator.generate(0, 5, 
            common_stuff::append("SLASH ABILITY LEVEL ", 
            savefile.sword_upgrade_lvl), 
        _text_sprites);

    generator.generate(0, 25, 
            common_stuff::append("ULTRAMATTER STORED: ", 
            savefile.ultramatter), 
        _text_sprites);

    generator.generate(0, 45, 
            common_stuff::append("DONATED TO VACCINE: ", 
            savefile.total_donated), 
        _text_sprites);

    uint32_t hours, minutes, seconds;
    seconds = savefile.playtime / 60 % 60;
    minutes = savefile.playtime / (60 * 60) % 60;
    hours = savefile.playtime / (60 * 60 * 60); 

    bn::string<256> playtime_text;
    bn::ostringstream stream(playtime_text);
    stream << "PLAY TIME :  ";
    stream << hours << ":";
    if(minutes < 10){
        stream << "0";
    }
    stream << minutes;

    generator.generate(0, 55, 
        playtime_text,
        _text_sprites);
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_bg_priority(0);
        sprite.set_blending_enabled(true);
    }
    bn::blending::set_transparency_alpha(0);
    _trans_alpha_action.emplace(5, 1);
    set_transition_effects_enabled(true);

}

void pause_info_scene::set_transition_effects_enabled(bool enabled){
    _bg.set_blending_enabled(enabled);
    _bg.set_mosaic_enabled(enabled);
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
    }
}

bn::optional<scene_type> pause_info_scene::update(){

    if(_trans_alpha_action){
        if(_trans_alpha_action->done())
        {
            _trans_alpha_action.reset();
        }else{
            _trans_alpha_action->update();
        }
    }

    bn::optional<scene_type> result;

    if(bn::keypad::start_pressed() || bn::keypad::b_pressed()){
        _trans_alpha_action.emplace(5, 0);
    }

    if(_trans_alpha_action && _trans_alpha_action->done() 
        && bn::blending::transparency_alpha() == 0)
    {
        result = scene_type::LEVEL;
    }
    
    return result;
}

}