#include "hud.h"
#include "common_fixed_8x8_sprite_font.h"

namespace aru {

hud::hud() : 
    _text_generator(fixed_8x8_sprite_font),
    _displayed_player_hp(50),
    _target_player_hp(50),
    _displayed_enemy_hp(10),
    _target_enemy_hp(10),
    _displayed_currency(10),
    _target_currency(10) 
{
    _text_generator.set_left_alignment();
    _text_generator.generate(-120, -72, "HP", _player_hp_label_text_sprites);  
    int_to_text(_player_hp_text_sprites, _displayed_player_hp, -120, -64);
}

void hud::update(){
    if(_ehp_visible_timer){
        --_ehp_visible_timer;
        if(_ehp_visible_timer != 0){
            
        }
    }

    if(_displayed_player_hp != _target_player_hp){
        _player_hp_text_sprites.clear();

        if(_displayed_player_hp > _target_player_hp){
            --_displayed_player_hp;
        }else if(_displayed_player_hp < _target_player_hp){
            ++_displayed_player_hp;
        } 
        int_to_text(_player_hp_text_sprites, _displayed_player_hp, -120, -64);
    }

}

void hud::update_player_hp(const uint8_t &hp){
    _target_player_hp = hp;
}

void hud::update_enemy_hp(bn::string<16> enemy_name, const uint8_t &hp){
    _ehp_visible_timer = 300;

    _text_generator.set_center_alignment();
    _text_generator.generate(100, 64, enemy_name, _enemy_hp_label_text_sprites);
}

void hud::hide(){

}

void hud::show(){

}

void hud::set_all_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible){
    for(bn::sprite_ptr &sprite : sprites){
        sprite.set_visible(visible);
    }
}

void hud::int_to_text(bn::ivector<bn::sprite_ptr> &sprites, const uint16_t &integer, bn::fixed x, bn::fixed y){
    bn::string<8> text_str;
    bn::ostringstream text_stream(text_str);
    text_stream << integer;
    _text_generator.generate(x, y, text_str, sprites); 
}

}