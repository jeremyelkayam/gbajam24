#include "menu_scene.h"
#include <bn_sprite_text_generator.h>
#include "common_variable_8x16_sprite_font.h"
#include "external_tool_file.h"
#include <bn_keypad.h>

namespace aru {


menu_scene::menu_scene() : 
    _selector(bn::sprite_items::common_variable_8x16_font.create_sprite(-45,25,29))
{
    bn::sprite_text_generator gen(variable_8x16_sprite_font);
    gen.set_center_alignment();
    gen.generate(0, -50, "GBA JAM 2024", _text_sprites);
    gen.generate(0, -40, "untitled bee robot post apoc game", _text_sprites);
    gen.generate(0, -25, "BUILD DATE: ", _text_sprites);
    gen.generate(0, -15, etf::current_date, _text_sprites);

    gen.generate(0, 10, "select stage", _text_sprites);


    gen.set_left_alignment();

    gen.generate(-40, 25, "LABORATORY", _text_sprites);
    gen.generate(-40, 35, "TEST MAP", _text_sprites);


}

bn::optional<scene_type> menu_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::a_pressed()){
        result = scene_type::LAB;
    }

    return result;
}
    
}