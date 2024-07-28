#include <bn_blending.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include "pause_menu.h"
#include "common_stuff.h"
#include "bn_regular_bg_items_pause_bg.h"

namespace aru {

pause_menu::pause_menu(bn::sprite_text_generator& text_generator) : 
    _box(bn::regular_bg_items::pause_bg.create_bg(0,0)),
    _selected(0)
{
    _box.set_priority(0);
    bn::blending::set_black_fade_color();
    bn::blending::set_fade_alpha(0.5);
    text_generator.set_bg_priority(0);
    text_generator.set_center_alignment();
    text_generator.set_one_sprite_per_character(false);
    text_generator.generate(0, -30, "- PAUSED -", _text_sprites);

    for(uint8_t z = 0; z < 3; ++z){
        bn::vector<bn::sprite_ptr, 4> vec;
        text_generator.generate(0, -10 + 15*z, _options[z].text, vec);
        _option_text_sprites.emplace_back(vec);
    }
    update_colors();
}

bn::optional<scene_type> pause_menu::update()
{
    if(bn::keypad::b_pressed() || bn::keypad::start_pressed() || bn::keypad::select_pressed()){
        return scene_type::BACK;
    }
    if(bn::keypad::up_pressed()){
        _selected = common_stuff::loop_addition(_selected, -1, 0, _option_text_sprites.size());
    }else if(bn::keypad::down_pressed()){
        _selected = common_stuff::loop_addition(_selected, 1, 0, _option_text_sprites.size());
    }
    if(bn::keypad::any_pressed()){
        update_colors();
    }

    if(bn::keypad::a_pressed()){
        //todo: add a warning saying that your progress in this level will be lost
        _box.set_blending_enabled(true);
        _box.set_mosaic_enabled(true);

        common_stuff::set_sprite_arr_effects_enabled(_text_sprites, true);
        for(bn::vector<bn::sprite_ptr, 4> &sprites : _option_text_sprites){
            common_stuff::set_sprite_arr_effects_enabled(sprites, true);
        }

        bn::blending::set_fade_alpha(0);
        return _options[_selected].next_scene;
    }

    return bn::optional<scene_type>();
}

void pause_menu::update_colors(){
    for(uint8_t z = 0; z < _option_text_sprites.size(); ++z){
        bn::sprite_palette_ptr pal = _text_sprites.at(0).palette();
        if(z == _selected){
            pal = common_stuff::monochrome_palette(bn::color(10, 31, 10));
        }
        for(bn::sprite_ptr &sprite : _option_text_sprites.at(z)){
            sprite.set_palette(pal);
        }
    }
}

}