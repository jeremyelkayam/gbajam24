#include "physics_entity.h"

namespace aru
{

class pickup : public physics_entity
{
public:
    pickup(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, 
        const bn::fixed &width, const bn::fixed &height, 
        const bn::fixed &max_up_speed, const bn::fixed &max_down_speed, 
        level &level, const bn::sprite_item &spritem, const uint16_t &value);

    uint16_t collect() {_collected = true; return _value;}
    bool collected() const {return _collected;}

protected:
    const uint16_t _value;
    bool _collected;

};

class health_pickup : public pickup
{
public:
    health_pickup(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, 
        level &level, const uint16_t &value);
};

}