#include <bn_blending.h>
#include "pause_menu.h"
#include "bn_regular_bg_items_textbox.h"

namespace aru {

pause_menu::pause_menu(bn::sprite_text_generator& text_generator) : 
    _box(bn::regular_bg_items::textbox.create_bg(0,0)),
    _selected(0)
{
    bn::blending::set_black_fade_color();
    bn::blending::set_fade_alpha(0.5);
}

void pause_menu::update()
{

}

}