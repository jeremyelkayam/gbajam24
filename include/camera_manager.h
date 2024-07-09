#pragma once
#include <bn_camera_ptr.h>
#include "level.h"
#include "player.h"

namespace aru {

class camera_manager {

private:
        bn::camera_ptr _cam;
        const level &_level;
        const player &_player;
public:
    camera_manager(bn::camera_ptr cam, const level &lv, const player &pl);
    void update();    

};


}