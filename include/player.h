#pragma once
#include <bn_sprite_animate_actions.h>
#include "effect.h"
#include "entity.h"


namespace aru
{
class player : public entity
{
public: 
    //todo: prob do not need to put spawnpoint in the constructor if it takes a level reference
    player(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level);
    virtual void update() override;
    virtual void jump() override;

private:
    effect _jumpcloud, _sprintcloud;
    bn::sprite_animate_action<4> _idle;

    const bn::fixed _DUSTCLOUD_OFFSET;

    uint8_t _jbuf_timer, _coyote_timer;

    virtual bool on_thin_ground() const override;
    virtual bool apply_gravity() const override;
    
};

}