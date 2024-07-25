#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sprite_text_generator.h>
#include <bn_version.h>
#include "common_variable_8x16_sprite_font.h"

#include "bn_regular_bg_items_logos_test.h"
#include "logos_scene.h"

namespace aru {

logos_scene::logos_scene() :
    _bg(bn::regular_bg_items::logos_test.create_bg(0,0)),
    _timer(255) {
    _bg.set_z_order(1);
    _bg.set_blending_enabled(true);
    _bg.set_mosaic_enabled(true);
    bn::sprite_text_generator gen(variable_8x16_sprite_font);

    gen.set_center_alignment();

    gen.generate(0, -65, "Created using", _text_sprites);
    gen.generate(-5, -30, "Butano", _text_sprites);
    gen.generate(-5, -20, bn::string<32>("version ") + BN_VERSION_STRING, _text_sprites);

    gen.generate(0, 12, "for", _text_sprites);
}

void logos_scene::set_transition_effects_enabled(bool enabled){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_blending_enabled(enabled);
        sprite.set_mosaic_enabled(enabled);
    }    
}

bn::optional<scene_type> logos_scene::update(){
    bn::optional<scene_type> result;
    --_timer;

    if(_timer == 0 || bn::keypad::any_pressed()){
        result = scene_type::MENU;
    }
    
    return result;
}

}