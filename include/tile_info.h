#pragma once

// adapted from helper_tilemap from Symbol*Merged by copyrat90

#include <bn_affine_bg_ptr.h>
#include <bn_assert.h>
#include <bn_camera_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_point.h>
#include <bn_size.h>
#include <bn_vector.h>

namespace aru
{

/**
 * @brief bg tile(8x8) index rect.

 *
 */

/**
 * @brief TopLeft-Anchored bg tile(8x8) index rect.
 *
 * Anchor position {0,0} is bg center.
 * (e.g. index_rect can be topLeft:{-15,-21}, bottomRight:{15,0},
 * then the center of the index_rect will be {0,-10.5})
 */
struct index_rect
{
    int x, y, w, h;

    /**
     * @brief Constructor.
     * Automatically moves anchor to match the center to the bg.
     * (i.e. Converts {0,0} to {-bgTileCountDimentions.width()/2,-bgTileCountDimentions.height()/2})
     *
     * @param bgTileCountSize bg_item.map_item().dimensions()
     */
    constexpr index_rect(int x_a, int y_a, int w_a, int h_a, bn::size bgTileCountSize)
        : x(x_a - bgTileCountSize.width() / 2), y(y_a - bgTileCountSize.height() / 2), w(w_a), h(h_a)
    {
        BN_ASSERT(w_a >= 30, "Invalid index_rect width: ", w);
        BN_ASSERT(h_a >= 20, "Invalid index_rect height: ", h);
    }

    /**
     * @brief Constructor.
     * Automatically moves anchor to match the center to the bg.
     * (i.e. Converts {0,0} to {-bgTileCountDimentions.width()/2,-bgTileCountDimentions.height()/2})
     *
     * @param bgTileCountSize bg_item.map_item().dimensions()
     */
    constexpr index_rect(bn::point topLeft, bn::size size, bn::size bgTileCountSize)
        : index_rect(topLeft.x(), topLeft.y(), size.width(), size.height(), bgTileCountSize)
    {
    }

    /**
     * @brief Constructor.
     * Automatically moves anchor to match the center to the bg.
     * (i.e. Converts {0,0} to {-bgTileCountDimentions.width()/2,-bgTileCountDimentions.height()/2})
     *
     * @param bgTileCountSize bg_item.map_item().dimensions()
     */
    constexpr index_rect(bn::point topLeft, bn::point bottomRight, bn::size bgTileCountSize)
        : index_rect(topLeft, bn::size{bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y()}, bgTileCountSize)
    {
    }

    constexpr bn::point TopLeftPosition() const
    {
        return {x, y};
    }

    /**
     * @brief Returns center position.
     *
     * type `bn::fixed_point` is used.
     * Because center position can hold *.5 element.
     * (e.g. index_rect can be topLeft:{-15,-21}, bottomRight:{15,0},
     * then the center of the index_rect will be {0,-10.5})
     *
     * @return `constexpr bn::fixed_point` center position
     */
    constexpr bn::fixed_point CenterPosition() const
    {
        return {x + bn::fixed(w) / 2, y + bn::fixed(h) / 2};
    }

    constexpr bn::size Dimensions() const
    {
        return {w, h};
    }
};

[[nodiscard]] constexpr bn::fixed_rect ConvertIndexRectToPositionRect(const index_rect& zoneBoundary)
{
    const bn::fixed_point centerPos = zoneBoundary.CenterPosition() * 8;
    const bn::fixed_size size = zoneBoundary.Dimensions() * 8;
    return {centerPos, size};
}

/**
 * @brief Snap camera to zone boundary.
 * This function receives absolute position `zoneBoundary`,
 * so, `ConvertIndexRectToPositionRect()` must be called before.
 *
 * @param cam
 * @param zoneBoundary zoneBoundary in absolute position
 */
void SnapCameraToZoneBoundary(bn::camera_ptr& cam, const bn::fixed_rect& zoneBoundary);

class tile_info
{
public:
    tile_info(const bn::affine_bg_ptr& bg);
    void reset(const bn::affine_bg_ptr& bg);

    tile_info(const tile_info&) = delete;
    tile_info& operator=(const tile_info&) = delete;

    tile_info(tile_info&& other) : bg_(other.bg_), tileFlags_(bn::move(other.tileFlags_))
    {
    }
    tile_info& operator=(tile_info&& other)
    {
        bg_ = other.bg_;
        tileFlags_ = bn::move(other.tileFlags_);
        return *this;
    }

    enum class flags
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

    flags GetTileFlagsByPosition(bn::fixed_point position) const;

private:
    static constexpr int TILE_VARIATION_COUNT = 32;
    static constexpr int TILE_GROUP_COUNT = 15;

    const bn::affine_bg_ptr* bg_;
    bn::vector<flags, TILE_VARIATION_COUNT> tileFlags_;

    flags At_(int tileVariationId) const;

    /**
     * @brief Get map index which can be used to index bg.map().cells_ref().value()
     *
     */
    int MapIndex_(bn::fixed_point point) const;
};

[[nodiscard]] constexpr tile_info::flags operator|(tile_info::flags f1, tile_info::flags f2)
{
    return static_cast<tile_info::flags>(static_cast<int>(f1) | static_cast<int>(f2));
}

[[nodiscard]] constexpr tile_info::flags operator&(tile_info::flags f1, tile_info::flags f2)
{
    return static_cast<tile_info::flags>(static_cast<int>(f1) & static_cast<int>(f2));
}

[[nodiscard]] constexpr bool operator!(tile_info::flags flags)
{
    return flags == tile_info::flags::EMPTY;
}

} // namespace sym::helper::tilemap