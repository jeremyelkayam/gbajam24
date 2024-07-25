#pragma once
#include <bn_camera_ptr.h>
#include "level.h"
#include "player.h"
#include "easer.h"

namespace aru {

class camera_manager {

private:
        bn::camera_ptr _cam;
        bn::fixed_rect _view;
        const level &_level;
        const player &_player;

        easer _x_ease, _y_ease;

public:
    camera_manager(bn::camera_ptr cam, const level &lv, const player &pl);
    bool on_screen(bn::fixed_rect &hitbox);
    void update();    

};


}