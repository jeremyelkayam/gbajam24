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
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_bg_priority(0);
    }
}

void pause_info_scene::set_transition_effects_enabled(bool enabled){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
    }
}

bn::optional<scene_type> pause_info_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::start_pressed() || bn::keypad::b_pressed()){
        result = scene_type::LEVEL;
    }
    
    return result;
}

}