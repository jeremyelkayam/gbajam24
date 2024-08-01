#pragma once
#include <bn_sprite_animate_actions.h>
#include <bn_fixed_rect.h>
#include <bn_deque.h>
#include "constants.h"
#include "physics_entity.h"
#include "rising_text.h"

namespace aru
{

class combat_entity : public physics_entity
{
public:
    combat_entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, 
        const bn::fixed &width, const bn::fixed &height, const bn::fixed &max_xspeed, 
        const bn::fixed &max_up_speed, const bn::fixed &max_down_speed, 
        const uint8_t &hp, const uint8_t &contact_damage, const uint8_t &iframes, 
        level &level, const bn::sprite_item &spritem, 
        bn::sprite_text_generator &rising_text_generator);
    virtual void update();

    void hit(uint8_t damage, bn::fixed x_push, bn::fixed y_push);
    void heal(const uint8_t &amount);
    uint8_t contact_damage() const {return _contact_damage;}
    uint8_t hp() const {return _hp;}
    uint8_t max_hp() const {return _max_hp;}
    bool in_iframes() const {return _iframes;}
    bool delete_me() const {return _explosion_anim && _explosion_anim->done() && _rising_text.empty();}

    virtual void set_blending_enabled(bool enabled);

    virtual void set_mosaic_enabled(bool enabled);
    virtual void set_visible(const bool &visible);
    void clear_rising_text() {_rising_text.clear();} 
    bool just_exploded() const;

protected:
    bn::optional<bn::sprite_animate_action<7>> _explosion_anim;
    bn::deque<rising_text, 4> _rising_text;
    bn::sprite_text_generator &_rising_text_generator;

    const uint8_t _HIT_IFRAMES;

    const uint8_t _max_hp;
    uint8_t _hp, _contact_damage,_iframes;

    virtual void die();
    static bool rising_text_deletable(const rising_text &r) {return r.delete_me();}
};

}