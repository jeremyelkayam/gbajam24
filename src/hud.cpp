#include "hud.h"
#include "common_fixed_8x8_sprite_font.h"
#include "bn_sprite_items_bar_3px.h"
#include "bn_sprite_items_bar_5px.h"
#include "bn_sprite_items_bar_32x3.h"
#include <bn_log.h>
#include <bn_size.h>
#include "common_stuff.h"
#include "constants.h"

namespace aru {

hud::hud(common_stuff &cstuff) : 
    _text_generator(cstuff.rising_text_generator),
    _enemy_hp(bn::sprite_items::bar_3px.create_sprite(90,72)),
    _displayed_enemy_hp(10),
    _target_enemy_hp(10),
    _max_enemy_hp(10),
    _currency_meter(cstuff.current_save().ultramatter, _text_generator),
    _player_hp_meter(PLAYER_HP, _text_generator, 
        bn::sprite_items::bar_5px, bn::fixed_point(-110,-28)),
    _hover_meter(PLAYER_HOVER_TIME[cstuff.current_save().hover_upgrade_lvl], _text_generator),
    _cstuff(cstuff)
{
    _enemy_hp.set_visible(false);
    _text_generator.set_center_alignment();
    _text_generator.generate(-110, -72, "HP", _player_hp_label_text_sprites);  
    update_hover_level();
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

    _player_hp_meter.update();
    _currency_meter.update();
    _hover_meter.update();

}

void hud::update_player_hp(const uint8_t &hp){
    _player_hp_meter.set_tracked_value(hp);
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

void hud::update_hover_time(const uint8_t &hover_time){
    _hover_meter.set_tracked_value(hover_time);
}

void hud::update_hover_level(){
    _hover_meter.set_max(PLAYER_HOVER_TIME[_cstuff.current_save().hover_upgrade_lvl]);
    _hover_meter.set_palette(PLAYER_PALETTE[_cstuff.current_save().hover_upgrade_lvl]);
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
    _player_hp_meter.set_blending_enabled(enabled);
    _hover_meter.set_blending_enabled(enabled);
}

void hud::set_mosaic_enabled(const bool &enabled){
    for(bn::sprite_ptr &sprite : _player_hp_label_text_sprites){
        sprite.set_mosaic_enabled(enabled);
    }
    for(bn::sprite_ptr &sprite : _enemy_hp_label_text_sprites){
        sprite.set_mosaic_enabled(enabled);
    }    
    _enemy_hp.set_mosaic_enabled(enabled);
    _currency_meter.set_mosaic_enabled(enabled);
    _player_hp_meter.set_mosaic_enabled(enabled);
    _hover_meter.set_mosaic_enabled(enabled);
}

void hud::set_visible(const bool &visible){
    for(bn::sprite_ptr &sprite : _player_hp_label_text_sprites){
        sprite.set_visible(visible);
    }
    for(bn::sprite_ptr &sprite : _enemy_hp_label_text_sprites){
        sprite.set_visible(visible);
    }    
    _enemy_hp.set_visible(visible);
    _currency_meter.set_visible(visible);
    _player_hp_meter.set_visible(visible);
    _hover_meter.set_visible(visible);
}

void text_hud_element::int_to_text(bn::ivector<bn::sprite_ptr> &sprites, const uint16_t &integer, bn::fixed x, bn::fixed y){
    bn::string<8> text_str;
    bn::ostringstream text_stream(text_str);
    text_stream << integer;
    text_stream << "|";
    _generator.set_one_sprite_per_character(false);
    _generator.set_left_alignment();
    _generator.generate(x, y, text_str, sprites); 
}

hud_element::hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator) : 
    _generator(generator),
    _displayed(tracked_value),
    _target(tracked_value),
    _delta(0),
    _time_since_updated(MAX_UINT8),
    _delta_factor(.01666666666) {    
}

void hud_element::update(){
    if(_displayed != _target){
        if(_displayed > _target){
            _displayed = common_stuff::bounded_subtraction(_displayed, _delta, _target);
        }else if(_displayed < _target){
            _displayed = common_stuff::bounded_addition(_displayed, _delta, _target);
        }
    }
    if(_time_since_updated < MAX_UINT8){
        ++_time_since_updated;
    }
    
}

void hud_element::set_tracked_value(const uint16_t &tracked_value) {
    if(_target != tracked_value) _time_since_updated = 0;

    _target = tracked_value;
    uint16_t diff = 0;

    if(_displayed > _target){
        diff = _displayed - _target;
    }else if(_displayed < _target){
        diff = _target - _displayed;
    }
    //should take 2 seconds to change max.
    //if the delta is smaller than 1 don't bother with fractions, just go 1 per frame.
    _delta = (diff * _delta_factor).ceil_integer();
    

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

void text_hud_element::set_mosaic_enabled(const bool &enabled){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_mosaic_enabled(enabled);
    }
}

meter_hud_element::meter_hud_element(const uint16_t &tracked_value, 
        bn::sprite_text_generator &generator, const bn::sprite_item &bar, 
        const bn::fixed_point &pos) : 
    hud_element(tracked_value, generator),
    _bar(bar.create_sprite(pos)),
    _meter_pos(pos),
    _max(tracked_value) {

}

void meter_hud_element::update(){

    if(_displayed < _target)
    {
        //we are healing
    }

    hud_element::update();
    bn::fixed scale_factor = bn::fixed(_displayed) / bn::fixed(_max);
    bn::fixed half_height = _bar.dimensions().height() * bn::fixed(0.5);

    

    //GBA refuses to do anything with a scale factor of 0, so let's just make the bar
    // invisible in this edge case
    if(scale_factor != 0){
        _bar.set_visible(true);
        _bar.set_vertical_scale(scale_factor);
    }else{
        _bar.set_visible(false);
    }
    _bar.set_y(_meter_pos.y() + half_height - 
        (half_height * scale_factor).floor_integer());
}

void hover_meter_hud_element::update()
{
    if(_time_since_updated < 60){
        meter_hud_element::update();
    }else{
        _bar.set_visible(false);
    }
}

hover_meter_hud_element::hover_meter_hud_element(const uint16_t &tracked_value, 
        bn::sprite_text_generator &generator) : 
    meter_hud_element(tracked_value, generator, bn::sprite_items::bar_32x3, 
        bn::fixed_point(-116, 62))

{
    _delta_factor *= 4;
    _bar.set_visible(false);
}

}