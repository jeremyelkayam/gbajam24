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
    // virtual ~player() override {return;}
    virtual void update() override;

private:
    effect _jumpcloud, _sprintcloud;
    bn::sprite_animate_action<4> _idle;

    const bn::fixed _DUSTCLOUD_OFFSET;

    uint8_t _jump_timer, _jbuf_timer, _coyote_timer;
    
};

}