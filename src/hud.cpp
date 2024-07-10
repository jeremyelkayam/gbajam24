#include "hud.h"
#include "common_fixed_8x8_sprite_font.h"
#include "bn_sprite_items_bar_3px.h"
#include "bn_sprite_items_bar_5px.h"
#include <bn_log.h>
#include "common_stuff.h"

namespace aru {

hud::hud() : 
    _text_generator(fixed_8x8_sprite_font),
    _player_hp(bn::sprite_items::bar_5px.create_sprite(-110,-28)),
    _enemy_hp(bn::sprite_items::bar_3px.create_sprite(90,72)),
    _displayed_player_hp(50),
    _target_player_hp(50),
    _max_player_hp(50),
    _displayed_enemy_hp(10),
    _target_enemy_hp(10),
    _max_enemy_hp(10),
    _displayed_currency(10000),
    _target_currency(10000) 
{
    _enemy_hp.set_visible(false);
    _text_generator.set_center_alignment();
    _text_generator.generate(-110, -72, "HP", _player_hp_label_text_sprites);  
    int_to_text(_crcy_text_sprites, _displayed_currency, 90, -72);
    
    
}

void hud::update(){
    //todo: refactor this into a health bar class
    if(_ehp_visible_timer){
        --_ehp_visible_timer;
        if(_ehp_visible_timer != 0){
            if(_displayed_enemy_hp != _target_enemy_hp){
                // _enemy_hp_text_sprites.clear();
                if(_displayed_enemy_hp > _target_enemy_hp){
                    --_displayed_enemy_hp;
                }else if(_displayed_enemy_hp < _target_enemy_hp){
                    ++_displayed_enemy_hp;
                }

                bn::fixed scale_factor = bn::fixed(_displayed_enemy_hp) / bn::fixed(_max_enemy_hp);
                if(scale_factor > 0){
                _enemy_hp.set_visible(true);
                    _enemy_hp.set_horizontal_scale(scale_factor);
                    _enemy_hp.set_x((90 - 32) + (bn::fixed(32)*scale_factor).floor_integer());
                }else{
                    _enemy_hp.set_visible(false);
                }
            }
        }else{
            _enemy_hp_label_text_sprites.clear();
            _enemy_hp.set_visible(false);
        }
    }

    //update player HP gauge
    if(_displayed_player_hp != _target_player_hp){
        // _player_hp_text_sprites.clear();

        if(_displayed_player_hp > _target_player_hp){
            --_displayed_player_hp;
        }else if(_displayed_player_hp < _target_player_hp){
            ++_displayed_player_hp;
        } 
        bn::fixed scale_factor = bn::fixed(_displayed_player_hp) / bn::fixed(_max_player_hp);
        _player_hp.set_vertical_scale(scale_factor);
        _player_hp.set_y(4 + (bn::fixed(-32)*scale_factor).floor_integer());
    }
    //todo: refactor this...
    if(_displayed_currency != _target_currency){
        if(_displayed_currency > _target_currency){
            //todo: calculate difference based on time (e.g. difference / 120 for 2 secs) 
            _displayed_currency = common_stuff::bounded_subtraction(_displayed_currency, 10, _target_currency);
        }else if(_displayed_currency < _target_currency){
            _displayed_currency = common_stuff::bounded_addition(_displayed_currency, 10, _target_currency);
        }         
        _crcy_text_sprites.clear();
        int_to_text(_crcy_text_sprites, _displayed_currency, 90, -72);
    }

}

void hud::update_player_hp(const uint8_t &hp){
    _target_player_hp = hp;
}

void hud::update_enemy_hp(bn::string<16> enemy_name, const uint8_t &prev_hp, const uint8_t &current_hp, const uint8_t &max_hp){
    _enemy_hp_label_text_sprites.clear();
    // _enemy_hp_text_sprites.clear();
    _ehp_visible_timer = 300;

    _displayed_enemy_hp = prev_hp;
    _target_enemy_hp = current_hp;
    _max_enemy_hp = max_hp;

    _text_generator.set_center_alignment();
    _text_generator.generate(90, 64, enemy_name, _enemy_hp_label_text_sprites);
    // int_to_text(_enemy_hp_text_sprites, _displayed_enemy_hp, 90, 72);
}

void hud::update_currency(const uint16_t &crcy){
    _target_currency = crcy;
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
    _text_generator.set_one_sprite_per_character(false);
    _text_generator.generate(x, y, text_str, sprites); 
}

}