#pragma once
#include <bn_fixed_point.h>
#include "bn_regular_bg_items_testmap.h"
#include "bn_regular_bg_items_testbg.h"
#include "bn_regular_bg_items_lab.h"

namespace aru 
{   
    //Enemy type ID
    struct enemy_data {
        uint8_t enemy_type;
        bn::fixed_point coords;
    };
    struct level_data {
        bn::fixed_point spawnpoint;
        bn::regular_bg_item map, backdrop;
        enemy_data enemies[32];
    };

    constexpr level_data lab_data {
        bn::fixed_point(256,208),
        bn::regular_bg_items::lab, 
        bn::regular_bg_items::testbg,
        {}
    };

    constexpr level_data test_level_data {
        bn::fixed_point(256,80),
        bn::regular_bg_items::testmap, 
        bn::regular_bg_items::testbg,
        {{1, bn::fixed_point(512, 80)},
         {1, bn::fixed_point(768, 80)}}
    };

}