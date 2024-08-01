#pragma once
#include <bn_forward_list.h>
#include <bn_unique_ptr.h>
#include "play_scene.h"
#include "death_anim.h"
#include "pickup.h"

namespace aru{

class level_scene : public play_scene{ 
private:
    //enemies must store their ID as an array index ... 
    // or also we could just make a map as well and index them based on that idk
    bn::forward_list<bn::unique_ptr<combat_entity>, 6> _enemies;
    bn::forward_list<health_pickup, 6> _health_pickups;
    bn::unordered_map<uint8_t, enemy_data, 32> _enemy_data; 
    bn::optional<death_anim> _dying;

    static bool enemy_deletable(bn::unique_ptr<combat_entity> &e) {return e->delete_me();}
    static bool pickup_deletable(health_pickup &p) {return p.collected();}
 
public:
    level_scene(common_stuff &cstuff);
    [[nodiscard]] virtual bn::optional<scene_type> update_scene_components() override final;
    virtual void set_transition_effects_enabled(bool enabled);
    virtual void set_visible(bool visible);
};

}