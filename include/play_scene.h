#pragma once
#include <bn_vector.h>
#include <bn_unique_ptr.h>
#include "scene.h"
#include "level.h"
#include "player.h"
#include "common_stuff.h"
#include <bn_forward_list.h>

namespace aru{

class play_scene : public scene{ 
protected:
    common_stuff &_cstuff;
    bn::camera_ptr _cam;
    level _level;
    player _player;

    bn::regular_bg_ptr _bg;
 
public:
    play_scene(common_stuff &cstuff, const bn::regular_bg_item &map, const bn::regular_bg_item &backdrop);
    [[nodiscard]] virtual bn::optional<scene_type> update();
};

}