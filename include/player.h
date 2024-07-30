#pragma once
#include <bn_forward_list.h>
#include "effect.h"
#include "combat_entity.h"
#include "common_stuff.h"
#include "enemy.h"
#include "bullet.h"
#include "bn_sprite_items_aru.h"

namespace aru
{

    enum class PSTATE {
        JUMP,
        FALL,
        STAND,
        RUN,
        HOVER,
        JUMPSQUAT
        // SHOOT,
        // SLASH
    };

class player : public combat_entity
{
public: 
    //todo: prob do not need to put spawnpoint in the constructor if it takes a level reference
    player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level, 
        const common_stuff::saved_data &savefile, 
        bn::sprite_text_generator &rising_text_generator);
    virtual void update() override;
    virtual void jump() override;
    bool check_bullet_collision(combat_entity &enemy);
    void move_to(const bn::fixed &xcor, const bool &face_right);
    void clear_target();
    uint8_t hover_time() const {return _hover_timer;}
    PSTATE state() const {return _current_state;}
    uint16_t time_since_state_change() const {return _state_change_timer;}
    virtual bool on_flat_ground() const;
    void squat() {_sprite.set_item(bn::sprite_items::aru, 6);}
    void update_palette() {_sprite.set_palette(PLAYER_PALETTE[_savefile.hover_upgrade_lvl]);}

    virtual void set_blending_enabled(bool enabled);
    virtual void set_mosaic_enabled(bool enabled);

private:
    effect _jumpcloud, _sprintcloud;
    const common_stuff::saved_data &_savefile;
    PSTATE _current_state;
    bn::sprite_animate_action<20> _idle;
    bn::sprite_animate_action<2> _jump;
    bn::sprite_animate_action<2> _jumpsquat;
    bn::sprite_animate_action<2> _fall;
    bn::sprite_animate_action<2> _hover;
    bn::sprite_animate_action<9> _run;
    bn::sprite_animate_action<9> _shoot;
    bn::sprite_animate_action<9> _shoot_run;
    bn::forward_list<bullet, 8> _bullets;
    bn::optional<bn::fixed> _target_xcor;
    bool _face_right_after_moving;

    static bool bullet_deletable(bullet &b) {return b.should_be_deleted();}

    const bn::fixed _DUSTCLOUD_OFFSET;

    uint8_t _jbuf_timer, _coyote_timer, _shoot_timer, _hover_timer;
    uint16_t _state_change_timer;

    virtual bool on_thin_ground() const override;
    virtual bool apply_gravity() const override;
    void shoot();
    virtual void land();
    void start_anim(bn::isprite_animate_action &anim);
    void set_state(const PSTATE &state);
    virtual void die();
 
};

}