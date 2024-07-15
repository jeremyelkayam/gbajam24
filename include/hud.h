#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sstream.h>
#include <bn_string.h>
#include <bn_sprite_ptr.h>
#include "common_stuff.h"

namespace aru {


class hud_element {
    protected:
        bn::sprite_text_generator &_generator;
        uint16_t _displayed, _target, _delta;

    public:
        hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator);
        virtual ~hud_element() {return;}
        virtual void update();
        void set_tracked_value(const uint16_t &tracked_value);
        virtual void set_visible(const bool &visible)=0;
        virtual void set_blending_enabled(const bool &enabled)=0;
};

class text_hud_element : public hud_element {
    private:
        bn::vector<bn::sprite_ptr, 8> _text_sprites;
        void int_to_text(bn::ivector<bn::sprite_ptr> &sprites, const uint16_t &integer, 
            const bn::fixed x, bn::fixed y);
    public:
        text_hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator);
        virtual void set_visible(const bool &visible) {common_stuff::set_sprite_arr_visible(
                _text_sprites, visible);}
        virtual void set_blending_enabled(const bool &enabled);
        virtual void update();
};

class health_hud_element : public hud_element { 
    protected:
        bn::sprite_ptr _health_bar;
        uint16_t _max;
    public:
        health_hud_element(const uint16_t &tracked_value, bn::sprite_text_generator &generator);
        virtual void set_visible(const bool &visible) {_health_bar.set_visible(visible);}
        virtual void set_blending_enabled(const bool &enabled) {_health_bar.set_blending_enabled(enabled);}
        virtual void update();
};

class hud { 
    private:
        bn::vector<bn::sprite_ptr, 8>   _player_hp_label_text_sprites,
                                        _enemy_hp_label_text_sprites;
                                        
        bn::sprite_text_generator _text_generator;
        bn::sprite_ptr _enemy_hp;
        
        uint8_t 
            _displayed_enemy_hp, _target_enemy_hp, _max_enemy_hp;

        text_hud_element _currency_meter;
        health_hud_element _player_hp;

        //todo: refactor the target values into const references to the actual values
        uint16_t _ehp_visible_timer;

    public:

        hud(const uint16_t &crcy);
        void update();
        void update_player_hp(const uint8_t &hp);
        void update_currency(const uint16_t &crcy);
        void update_enemy_hp(bn::string<16> enemy_name, const uint8_t &prev_hp, 
            const uint8_t &current_hp, const uint8_t &max_hp);
        void hide();
        void show();
        void set_blending_enabled(const bool &enabled);
};

}