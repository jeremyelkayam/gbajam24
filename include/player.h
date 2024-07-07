#pragma once
#include <bn_forward_list.h>
#include "effect.h"
#include "combat_entity.h"
#include "enemy.h"
#include "bullet.h"

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
    player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level);
    virtual void update() override;
    virtual void jump() override;
    bool check_bullet_collision(enemy &enemy);

private:
    effect _jumpcloud, _sprintcloud;
    PSTATE _current_state;
    bn::sprite_animate_action<20> _idle;
    bn::sprite_animate_action<2> _jump;
    bn::sprite_animate_action<2> _jumpsquat;
    bn::sprite_animate_action<2> _fall;
    bn::sprite_animate_action<2> _hover;
    bn::sprite_animate_action<8> _run;
    bn::forward_list<bullet, 8> _bullets;

    static bool bullet_deletable(bullet &b) {return b.should_be_deleted();}

    const bn::fixed _DUSTCLOUD_OFFSET;

    uint8_t _jbuf_timer, _coyote_timer, _shoot_timer, _hover_timer;

    virtual bool on_thin_ground() const override;
    virtual bool apply_gravity() const override;
    void shoot();
    virtual void land();
    
};

}