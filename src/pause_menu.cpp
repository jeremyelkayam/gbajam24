#include <bn_blending.h>
#include <bn_keypad.h>
#include "pause_menu.h"
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
}

bool pause_menu::update()
{
    if(bn::keypad::b_pressed() || bn::keypad::start_pressed() || bn::keypad::select_pressed()){
        return true;
    }
    if(bn::keypad::up_pressed()){
        _selected 
    }

    return false;
}

}