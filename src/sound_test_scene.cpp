#include "sound_test_scene.h"
#include "common_variable_8x16_sprite_font.h"
#include "external_tool_file.h"
#include "bn_regular_bg_items_honeycomb.h"
#include <bn_keypad.h>
#include <bn_string.h>
#include <bn_music_items_info.h>
#include "common_stuff.h"

namespace aru {

sound_test_scene::sound_test_scene() : 
    _gen(variable_8x16_sprite_font),
    _index(0),
    _bg(bn::regular_bg_items::honeycomb.create_bg(0,5)),
    _bg_move(_bg, -0.5, 0.5)
{
    _gen.set_center_alignment();

    _gen.generate(0, -50, "SOUND TEST", _title_text_sprites);
    _gen.generate(0, -30, "select music using up and down keys", _title_text_sprites);

    _bg.set_blending_enabled(true);
    _bg.set_mosaic_enabled(true);
    update_track_number();
}

void sound_test_scene::set_transition_effects_enabled(bool enabled){
    for(bn::sprite_ptr &sprite : _title_text_sprites){
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
    }
    for(bn::sprite_ptr &sprite : _track_number_text_sprites){
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
    }

    _bg.set_blending_enabled(enabled);
    _bg.set_mosaic_enabled(enabled);
}

bn::optional<scene_type> sound_test_scene::update(){
    bn::optional<scene_type> result;

    _bg_move.update();
    if(bn::keypad::up_pressed() || bn::keypad::down_pressed()){
        if(bn::keypad::down_pressed()){
            _index = common_stuff::bounded_addition(_index, 1, bn::music_items_info::span.size() - 1);
        }else if(bn::keypad::up_pressed()){
            _index = common_stuff::bounded_subtraction(_index, 1, 0);
        }
        update_track_number();
    }


    if(bn::keypad::a_pressed()){
        bn::music_item(_index).play(1);
    }    

    if(bn::keypad::b_pressed()){
        result = scene_type::MENU;
    }

    return result;
}

void sound_test_scene::update_track_number()
{
    _gen.set_left_alignment();
    _track_number_text_sprites.clear();
    bn::string<256> text;
    bn::ostringstream stream(text);
    stream << _index;
    stream << ". ";
    stream << bn::music_items_info::span.at(_index).second;
    _gen.generate(-15, 0, text, _track_number_text_sprites);

}
 
}