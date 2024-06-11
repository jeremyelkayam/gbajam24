#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sstream.h>
#include <bn_string.h>
#include <bn_sprite_ptr.h>

namespace aru {
class hud { 
    private:
        bn::vector<bn::sprite_ptr, 8>   _player_hp_text_sprites, 
                                        _player_hp_label_text_sprites,
                                        _enemy_hp_text_sprites,
                                        _enemy_hp_label_text_sprites,
                                        _crcy_text_sprites;

        // void set_all_visible(bool visible);
        bn::sprite_text_generator _text_generator;
        uint8_t _displayed_player_hp, _target_player_hp, _displayed_enemy_hp, _target_enemy_hp;
        uint16_t _displayed_currency, _target_currency, _ehp_visible_timer;

    public:

        hud();
        void update();
        void update_player_hp(const uint8_t &hp);
        void update_enemy_hp(bn::string<16> enemy_name, const uint8_t &prev_hp, const uint8_t &current_hp);

        void set_all_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);
        void int_to_text(bn::ivector<bn::sprite_ptr> &sprites, const uint16_t &integer, bn::fixed x, bn::fixed y);

        void hide();
        void show();
};
}