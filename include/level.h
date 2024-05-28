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
    bn::fixed width() {return _bg_ptr.dimensions().width();}
    bn::fixed height() {return _bg_ptr.dimensions().height();}

    bool is_ground(const bn::fixed_point &coords) const;
    bool is_thin_ground(const bn::fixed_point &coords) const;
private:
    const bn::regular_bg_item &_bg;
    bn::regular_bg_ptr _bg_ptr;
    const int _COLUMNS;
    bn::span<const bn::regular_bg_map_cell> _cells;
    bn::regular_bg_map_cell cell_at(const bn::fixed_point &coords) const;
    bn::regular_bg_map_cell cell_at(const unsigned short &xtile, const unsigned short &ytile) const;
};

}