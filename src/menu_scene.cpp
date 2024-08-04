#include "menu_scene.h"
#include <bn_sprite_text_generator.h>
#include "common_variable_8x16_sprite_font.h"
#include "bn_sprite_items_cursor.h"
#include "external_tool_file.h"
#include "bn_regular_bg_items_honeycomb.h"
#include <bn_keypad.h>
#include <bn_string.h>
#include "common_stuff.h"

namespace aru {


menu_scene::menu_scene() : 
    _selector(bn::sprite_items::cursor.create_sprite(-47,25)),
    _selector_anim(bn::create_sprite_animate_action_forever(
        _selector, 4, bn::sprite_items::cursor.tiles_item(), 0, 1, 2, 3, 4, 5)),
    _index(0),
    _bg(bn::regular_bg_items::honeycomb.create_bg(0,5)),
    _bg_move(_bg, -0.5, 0.5)
{
    bn::sprite_text_generator gen(variable_8x16_sprite_font);
    gen.set_center_alignment();
    gen.generate(0, 75, "BUILT " + bn::string<32>(etf::current_date), _text_sprites);

    gen.generate(0, -50, "GBA JAM 2024", _text_sprites);
    gen.generate(0, -40, "Globlins!", _text_sprites);

    gen.generate(0, 10, "select stage", _text_sprites);


    gen.set_left_alignment();

    gen.generate(-40, 25, "LABORATORY", _text_sprites);
    gen.generate(-40, 35, "TEST MAP", _text_sprites);
    gen.generate(-40, 45, "SOUND TEST", _text_sprites);

    _selector.set_blending_enabled(true);
    _selector.set_mosaic_enabled(true);
    _bg.set_blending_enabled(true);
    _bg.set_mosaic_enabled(true);
}

void menu_scene::set_transition_effects_enabled(bool enabled){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
    }
}

bn::optional<scene_type> menu_scene::update(){
    bn::optional<scene_type> result;

    _bg_move.update();
    _selector_anim.update();

    if(bn::keypad::down_pressed()){
        _index = common_stuff::bounded_addition(_index, 1, 2);
    }else if(bn::keypad::up_pressed()){
        _index = common_stuff::bounded_subtraction(_index, -1, 0);
    }
    _selector.set_y(25 + 10 * _index);

    if(bn::keypad::a_pressed()){
        if(_index == 0){
            result = scene_type::LAB;
        }else if(_index == 1){
            result = scene_type::LEVEL;
        }else if(_index == 2){
            result = scene_type::SOUND_TEST;
        }
    }

    return result;
}
    
}