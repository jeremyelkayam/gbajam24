#pragma once

#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_item.h>
#include <bn_camera_ptr.h>
#include <bn_fixed_rect.h>
#include "tile_info.h"


namespace aru
{
class level 
{
public: 
    level(const bn::camera_ptr &cam, const bn::regular_bg_item &bg);
    void update();
    bn::fixed width() const {return _bg_ptr.dimensions().width();}
    bn::fixed height() const {return _bg_ptr.dimensions().height();}

    bool is_thick_ground(const bn::fixed_point &coords) const;
    bool is_thin_ground(const bn::fixed_point &coords) const;
    bool is_leftfacing_wall(const bn::fixed_point &coords) const;
    bool is_rightfacing_wall(const bn::fixed_point &coords) const;
    bool is_up_slope(const bn::fixed_point &coords) const;
    bool is_down_slope(const bn::fixed_point &coords) const;
    bool is_ceiling(const bn::fixed_point &coords) const;
    void print_hitbox(const bn::fixed_rect &hitbox) const;

private:
    bn::regular_bg_item _bg;
    const uint16_t _ROWS, _COLUMNS;
    bn::regular_bg_ptr _bg_ptr;
    bn::span<const bn::regular_bg_map_cell> _cells;
    bn::point _spawnpoint;

public: //temp. later we will do flags
    const bn::regular_bg_map_cell _THICK_GROUND,_THIN_GROUND, _RF_WALL, _LF_WALL, _CEILING,
        _RFT_CORNER, _LFT_CORNER, _RFB_CORNER, _LFB_CORNER,
        _UP_SLOPE, _DOWN_SLOPE, _UP_HALFSLOPE_1, _UP_HALFSLOPE_2,
        _DOWN_HALFSLOPE_1, _DOWN_HALFSLOPE_2;
    bn::regular_bg_map_cell cell_at(const bn::fixed_point &coords) const;
    bn::regular_bg_map_cell cell_at(const unsigned short &xtile, const unsigned short &ytile) const;

};

}