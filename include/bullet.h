#pragma once
#include "combat_entity.h"


namespace aru
{
class bullet : public combat_entity
{
public: 
    bullet(const bn::camera_ptr &cam, bn::fixed x, bn::fixed y, level &level,
        bool facing_right, bn::sprite_text_generator &rising_text_generator);
    virtual void update() override;
    bool should_be_deleted() const;

private:

    virtual bool apply_gravity() const override {return false;}
    
};

}