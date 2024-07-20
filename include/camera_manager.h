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

        class easer {
            private:
                const uint8_t _ease_time;
                uint8_t _timer;
                bn::fixed _start, _target;
                bn::fixed quadratic_ease_in_out(const bn::fixed &p);
                bn::fixed bezier_ease_in_out(const bn::fixed &p);

            public:
                easer(const bn::fixed &start, const bn::fixed &target, 
                    const uint8_t &ease_time);
                void set_target(const bn::fixed &target);
                void update();
                bn::fixed current();
        };

        easer _x_ease, _y_ease;

public:
    camera_manager(bn::camera_ptr cam, const level &lv, const player &pl);
    void update();    

};


}