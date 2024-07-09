#include "menu_scene.h"
#include <bn_sprite_text_generator.h>
#include "common_variable_8x16_sprite_font.h"
#include "external_tool_file.h"
#include <bn_keypad.h>
#include <bn_string.h>
#include "common_stuff.h"

namespace aru {


menu_scene::menu_scene() : 
    _selector(bn::sprite_items::common_variable_8x16_font.create_sprite(-45,25,29)),
    _index(0)
{
    bn::sprite_text_generator gen(variable_8x16_sprite_font);
    gen.set_center_alignment();
    gen.generate(0, 75, "BUILT " + bn::string<32>(etf::current_date), _text_sprites);

    gen.generate(0, -50, "GBA JAM 2024", _text_sprites);
    gen.generate(0, -40, "untitled bee robot post apoc game", _text_sprites);

    gen.generate(0, 10, "select stage", _text_sprites);


    gen.set_left_alignment();

    gen.generate(-40, 25, "LABORATORY", _text_sprites);
    gen.generate(-40, 35, "TEST MAP", _text_sprites);


}

bn::optional<scene_type> menu_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::down_pressed()){
        _index = common_stuff::bounded_addition(_index, 1, 1);
    }else if(bn::keypad::up_pressed()){
        _index = common_stuff::bounded_subtraction(_index, -1, 0);
    }
    _selector.set_y(25 + 10 * _index);

    if(bn::keypad::a_pressed()){
        if(_index == 0){
            result = scene_type::LAB;
        }else if(_index == 1){
            result = scene_type::LEVEL;
        }
    }

    return result;
}
    
}