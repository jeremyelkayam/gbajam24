#pragma once
#include <bn_unique_ptr.h>
#include "scene.h"
#include "level.h"
#include "player.h"
#include "common_stuff.h"
#include "camera_manager.h"
#include "hud.h"
#include "level_data.h"

namespace aru{

class play_scene : public scene{ 
protected:
    common_stuff &_cstuff;
    bn::camera_ptr _cam;
    level _level;
    player _player;
    camera_manager _cam_mgr;
    hud _hud;


    bn::regular_bg_ptr _bg;
 
public:
    play_scene(common_stuff &cstuff, const level_data &ld);
    [[nodiscard]] virtual bn::optional<scene_type> update();
};

}