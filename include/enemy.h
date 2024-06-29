#pragma once
#include <bn_sprite_animate_actions.h>
#include "combat_entity.h"


namespace aru
{
class enemy : public combat_entity
{
public: 
    //todo: prob do not need to put spawnpoint in the constructor if it takes a level reference
    enemy(bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level);
    virtual void update() override;

private:
    bn::sprite_animate_action<6> _idle;
    uint8_t _jump_again_timer;
    
};

}