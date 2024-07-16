#pragma once

#include <bn_regular_bg_ptr.h>
#include <bn_regular_bg_item.h>
#include <bn_camera_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_unordered_map.h>


namespace aru
{

enum class tile_flags
{
    EMPTY =                 0,
    CEILING =               1 << 0,
    FLOOR =                 1 << 1,
    LEFT_BLOCKING_WALL =    1 << 2,
    RIGHT_BLOCKING_WALL =   1 << 3,
    UP_SLOPE =              1 << 4,
    DOWN_SLOPE =            1 << 5,
    UP_HALFSLOPE_A =        1 << 6,
    UP_HALFSLOPE_B =        1 << 7,
    DOWN_HALFSLOPE_A =      1 << 8,
    DOWN_HALFSLOPE_B =      1 << 9,
    THIN_FLOOR =            1 << 10,
};

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
    bool tile_has_flag(const bn::point &coords, 
        const tile_flags &flags) const;
    bool tile_has_flag(const bn::fixed_point &coords, 
        const tile_flags &flags) const;
    void print_hitbox(const bn::fixed_rect &hitbox) const;
    void set_blending_enabled(const bool &enabled){_bg_ptr.set_blending_enabled(enabled);}
    void set_mosaic_enabled(const bool &enabled){_bg_ptr.set_mosaic_enabled(enabled);}


    bn::regular_bg_map_cell cell_at(const bn::fixed_point &coords) const;
    bn::regular_bg_map_cell cell_at(const unsigned short &xtile, const unsigned short &ytile) const;

    bool tile_has_flag(const bn::regular_bg_map_cell &tile_index, 
        const tile_flags &flags) const;

private:
    bn::regular_bg_item _bg;
    const uint16_t _ROWS, _COLUMNS;
    bn::regular_bg_ptr _bg_ptr;
    bn::span<const bn::regular_bg_map_cell> _cells;
    bn::point _spawnpoint;

    static constexpr int TILE_VARIATION_COUNT = 32;
    static constexpr int TILE_GROUP_COUNT = 15;

    bn::unordered_map<bn::regular_bg_map_cell, tile_flags, TILE_VARIATION_COUNT> _tile_flags;
};

[[nodiscard]] constexpr tile_flags operator|(tile_flags f1, tile_flags f2)
{
    return static_cast<tile_flags>(static_cast<int>(f1) | static_cast<int>(f2));
}

[[nodiscard]] constexpr tile_flags operator&(tile_flags f1, tile_flags f2)
{
    return static_cast<tile_flags>(static_cast<int>(f1) & static_cast<int>(f2));
}

[[nodiscard]] constexpr bool operator!(tile_flags flags)
{
    return flags == tile_flags::EMPTY;
}

}