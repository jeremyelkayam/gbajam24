#pragma once

#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_item.h>
#include <bn_camera_ptr.h>


namespace aru
{
class level 
{
public: 
    level(const bn::camera_ptr &cam, const bn::regular_bg_item &bg);
    void update();

    bool is_ground(const bn::fixed_point &coords) const;
private:
    bn::regular_bg_ptr _bg;
    const int _COLUMNS;
    bn::span<const bn::regular_bg_map_cell> _cells;
    int tile_index_at(const bn::fixed_point &coords) const;
};

}