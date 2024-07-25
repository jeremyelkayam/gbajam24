#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sprite_text_generator.h>
#include "common_variable_8x16_sprite_font.h"

#include "pause_info_scene.h"

namespace aru {

pause_info_scene::pause_info_scene(
    bn::sprite_text_generator &generator, 
    const common_stuff::saved_data &savefile) 
{
    generator.set_center_alignment();

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


    // for(bn::sprite_ptr &sprite : _text_sprites){
    //     sprite.set_blending_enabled(true);
    //     sprite.set_mosaic_enabled(true);
    // }
}

bn::optional<scene_type> pause_info_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::start_pressed() || bn::keypad::b_pressed()){
        result = scene_type::LEVEL;
    }
    
    return result;
}

}