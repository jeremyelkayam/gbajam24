#pragma once
#include "combat_entity.h"


namespace aru
{
class bullet : public combat_entity
{
public: 
    //todo: prob do not need to put spawnpoint in the constructor if it takes a level reference
    bullet(const bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level, bool facing_right);
    virtual void update() override;
    bool should_be_deleted() const;

private:

    virtual bool apply_gravity() const override {return false;}
    
};

}