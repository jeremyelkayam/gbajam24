#pragma once
#include <bn_fixed_point.h>
#include "bn_regular_bg_items_testmap.h"
#include "bn_regular_bg_items_testbg.h"
#include "bn_regular_bg_items_lab.h"

namespace aru 
{
    struct level_data {
        bn::fixed_point spawnpoint;
        bn::regular_bg_item map, backdrop;
    };

    constexpr level_data lab_data {
        bn::fixed_point(256,208),
        bn::regular_bg_items::lab, 
        bn::regular_bg_items::testbg
    };

    constexpr level_data test_level_data {
        bn::fixed_point(256,20),
        bn::regular_bg_items::testmap, 
        bn::regular_bg_items::testbg
    };

}