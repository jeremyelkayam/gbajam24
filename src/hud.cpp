#include "hud.h"
#include "common_fixed_8x8_sprite_font.h"
#include "bn_sprite_items_bar_3px.h"
#include "bn_sprite_items_bar_5px.h"
#include <bn_log.h>
#include "common_stuff.h"
#include "constants.h"

namespace aru {

hud::hud(const uint16_t &crcy) : 
    _text_generator(fixed_8x8_sprite_font),
    _enemy_hp(bn::sprite_items::bar_3px.create_sprite(90,72)),
    _displayed_enemy_hp(10),
    _target_enemy_hp(10),
    _max_enemy_hp(10),
    _currency_meter(crcy, _text_generator),
    _player_hp(PLAYER_HP, _text_generator)
{
    _enemy_hp.set_visible(false);
    _text_generator.set_center_alignment();
    _text_generator.generate(-110, -72, "HP", _player_hp_label_text_sprites);  
    
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

    _player_hp.update();
    _currency_meter.update();

}

void hud::update_player_hp(const uint8_t &hp){
    _player_hp.set_tracked_value(hp);
}

void hud::update_enemy_hp(bn::string<16> enemy_name, const uint8_t &prev_hp, const uint8_t &current_hp, const uint8_t &max_hp){
    _enemy_hp_label_text_sprites.clear();
    _ehp_visible_timer = 300;

    _displayed_enemy_hp = prev_hp;
    _target_enemy_hp = current_hp;
    _max_enemy_hp = max_hp;

    _text_generator.set_center_alignment();
    _text_generator.generate(90, 64, enemy_name, _enemy_hp_label_text_sprites);
}

void hud::update_currency(const uint16_t &crcy){
    _currency_meter.set_tracked_value(crcy);
}

void hud::hide(){

}

void hud::show(){

}

void hud::set_blending_enabled(const bool &enabled){
    for(bn::sprite_ptr &sprite : _player_hp_label_text_sprites){
        sprite.set_blending_enabled(enabled);
    }
    for(bn::sprite_ptr &sprite : _enemy_hp_label_text_sprites){
        sprite.set_blending_enabled(enabled);
    }    
    _enemy_hp.set_blending_enabled(enabled);
    _currency_meter.set_blending_enabled(enabled);
    _player_hp.set_blending_enabled(enabled);
}

void text_hud_element::int_to_text(bn::ivector<bn::sprite_ptr> &sprites, const uint16_t &integer, bn::fixed x, bn::fixed y){
    bn::string<8> text_str;
    bn::ostringstream text_stream(text_str);
    text_stream << integer;
    _generator.set_one_sprite_per_character(false);
    _generator.set_left_alignment();
    _generator.generate(x, y, text_str, sprites); 
}

hud_element::hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator) : 
    _generator(generator),
    _displayed(tracked_value),
    _target(tracked_value),
    _delta(0) {    
}

void hud_element::update(){
    if(_displayed != _target){
        if(_displayed > _target){
            _displayed = common_stuff::bounded_subtraction(_displayed, _delta, _target);
        }else if(_displayed < _target){
            _displayed = common_stuff::bounded_addition(_displayed, _delta, _target);
        }
    }
}

void hud_element::set_tracked_value(const uint16_t &tracked_value) {
    _target = tracked_value;
    uint16_t diff = 0;

    if(_displayed > _target){
        diff = _displayed - _target;
    }else if(_displayed < _target){
        diff = _target - _displayed;
    }
    //should take 2 seconds to change.
    _delta = (diff * bn::fixed(.01666666666)).ceil_integer();
}


text_hud_element::text_hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator) : 
    hud_element(tracked_value, generator) {

    int_to_text(_text_sprites, _displayed, 80, -72);
}

void text_hud_element::update(){
    if(_displayed != _target){
        hud_element::update();
        _text_sprites.clear();
        int_to_text(_text_sprites, _displayed, 80, -72);
    }
}

void text_hud_element::set_blending_enabled(const bool &enabled){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_blending_enabled(enabled);
    }
}

health_hud_element::health_hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator) : 
    hud_element(tracked_value, generator),
    _health_bar(bn::sprite_items::bar_5px.create_sprite(-110,-28)),
    _max(tracked_value) {

}

void health_hud_element::update(){
    hud_element::update();
    bn::fixed scale_factor = bn::fixed(_displayed) / bn::fixed(_max);
    _health_bar.set_vertical_scale(scale_factor);
    _health_bar.set_y(4 + (bn::fixed(-32)*scale_factor).floor_integer());
}

}