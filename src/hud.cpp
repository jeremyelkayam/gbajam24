#include "hud.h"
#include "common_fixed_8x8_sprite_font.h"
#include <bn_log.h>

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
    _text_generator.set_center_alignment();
    _text_generator.generate(-110, -72, "HP", _player_hp_label_text_sprites);  
    int_to_text(_player_hp_text_sprites, _displayed_player_hp, -110, -64);
    
    _text_generator.generate(100, -72, "E:0", _crcy_text_sprites);  
    
}

void hud::update(){
    if(_ehp_visible_timer){
        --_ehp_visible_timer;
        if(_ehp_visible_timer != 0){
            if(_displayed_enemy_hp != _target_enemy_hp){
                _enemy_hp_text_sprites.clear();
                if(_displayed_enemy_hp > _target_enemy_hp){
                    --_displayed_enemy_hp;
                }else if(_displayed_enemy_hp < _target_enemy_hp){
                    ++_displayed_enemy_hp;
                }
                int_to_text(_enemy_hp_text_sprites, _displayed_enemy_hp, 90, 72);
            }
        }else{
            _enemy_hp_label_text_sprites.clear();
            _enemy_hp_text_sprites.clear();
        }
    }

    if(_displayed_player_hp != _target_player_hp){
        _player_hp_text_sprites.clear();

        if(_displayed_player_hp > _target_player_hp){
            --_displayed_player_hp;
        }else if(_displayed_player_hp < _target_player_hp){
            ++_displayed_player_hp;
        } 
        int_to_text(_player_hp_text_sprites, _displayed_player_hp, -110, -64);
    }

}

void hud::update_player_hp(const uint8_t &hp){
    _target_player_hp = hp;
}

void hud::update_enemy_hp(bn::string<16> enemy_name, const uint8_t &prev_hp, const uint8_t &current_hp){
    _enemy_hp_label_text_sprites.clear();
    _enemy_hp_text_sprites.clear();
    _ehp_visible_timer = 300;

    _displayed_enemy_hp = prev_hp;
    _target_enemy_hp = current_hp;
    BN_LOG("displayed health: ", _displayed_enemy_hp);
    BN_LOG("target health: ", _target_enemy_hp);

    _text_generator.set_center_alignment();
    _text_generator.generate(90, 64, enemy_name, _enemy_hp_label_text_sprites);
    int_to_text(_enemy_hp_text_sprites, _displayed_enemy_hp, 90, 72);
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