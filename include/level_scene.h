#pragma once
#include <bn_forward_list.h>
#include <bn_unique_ptr.h>
#include "play_scene.h"

namespace aru{

class level_scene : public play_scene{ 
private:
    bn::forward_list<bn::unique_ptr<combat_entity>, 6> _enemies;
 
public:
    level_scene(common_stuff &cstuff);
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
};

}