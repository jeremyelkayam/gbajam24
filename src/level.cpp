#include <bn_log.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_regular_bg_map_cell_info.h>
#include <bn_sstream.h>
#include <bn_string.h>


#include "level.h"


namespace aru {

level::level(const bn::camera_ptr &cam, const bn::regular_bg_item &bg) : 
    _bg(bg),
    _ROWS(_bg.map_item().dimensions().height()),
    _COLUMNS(_bg.map_item().dimensions().width()),
    _bg_ptr(_bg.create_bg(_COLUMNS * 4,
        _ROWS * 4)),
    _cells(_bg_ptr.map().cells_ref().value()),
    _THIN_GROUND(cell_at(3,0)),
    _RF_WALL(cell_at(5,0)),
    _LF_WALL(cell_at(7,0)),
    _CEILING(cell_at(9,0)),
    _RFT_CORNER(cell_at(11,0)),
    _LFT_CORNER(cell_at(13,0)),
    _RFB_CORNER(cell_at(15,0)),
    _LFB_CORNER(cell_at(17,0)),
    _UP_SLOPE(cell_at(19,0)),
    _DOWN_SLOPE(cell_at(21,0)),
    _UP_HALFSLOPE_1(cell_at(23,0)),
    _UP_HALFSLOPE_2(cell_at(25,0)),
    _DOWN_HALFSLOPE_1(cell_at(27,0)),
    _DOWN_HALFSLOPE_2(cell_at(29,0))
{
    _tile_flags.clear();
    _tile_flags.insert(0, tile_flags::EMPTY);
    int group = 0;
    BN_ASSERT(_cells[0] == 0, "First cell is not empty. check transparent color.");
    for (int i = 1; group < TILE_GROUP_COUNT; ++i)
    {
        if (_cells[i] == !!tile_flags::EMPTY)
        {
            ++group;
            continue;
        }
        switch (group)
        {
        case 0:
            _tile_flags.insert(_cells[i], tile_flags::FLOOR);
            break;
        case 1:
            _tile_flags.insert(_cells[i], tile_flags::THIN_FLOOR);
            break;
        case 2:
            _tile_flags.insert(_cells[i], tile_flags::LEFT_BLOCKING_WALL);
            break;
        case 3:
            _tile_flags.insert(_cells[i], tile_flags::RIGHT_BLOCKING_WALL);
            break;
        case 4:
            _tile_flags.insert(_cells[i], tile_flags::CEILING);
            break;
        case 5:
            _tile_flags.insert(_cells[i], tile_flags::FLOOR | tile_flags::LEFT_BLOCKING_WALL);
            break;
        case 6:
            _tile_flags.insert(_cells[i], tile_flags::FLOOR | tile_flags::RIGHT_BLOCKING_WALL);
            break;
        case 7:
            _tile_flags.insert(_cells[i], tile_flags::CEILING | tile_flags::LEFT_BLOCKING_WALL);
            break;
        case 8:
            _tile_flags.insert(_cells[i], tile_flags::CEILING | tile_flags::RIGHT_BLOCKING_WALL);
            break;
        case 9:
            _tile_flags.insert(_cells[i], tile_flags::UP_SLOPE);
            break;
        case 10:
            _tile_flags.insert(_cells[i], tile_flags::DOWN_SLOPE);
            break;
        case 11:
            _tile_flags.insert(_cells[i], tile_flags::UP_HALFSLOPE_A);
            break;
        case 12:
            _tile_flags.insert(_cells[i], tile_flags::UP_HALFSLOPE_B);
            break;
        case 13:
            _tile_flags.insert(_cells[i], tile_flags::DOWN_HALFSLOPE_A);
            break;
        case 14:
            _tile_flags.insert(_cells[i], tile_flags::DOWN_HALFSLOPE_B);
            break;
        default:
            BN_ERROR("Invalid tile group number");
            break;
        }
    }

    _bg_ptr.set_camera(cam);

    bn::regular_bg_map_cell spawn_tile = cell_at(31,0);

    for(uint16_t xtile = 0; xtile < _COLUMNS; ++xtile){
        for(uint16_t ytile = 1; ytile < _ROWS; ++ytile){
            if(cell_at(xtile, ytile) == spawn_tile){
                
            }
        }
    }

}

bn::regular_bg_map_cell level::cell_at(const bn::fixed_point &coords) const{
    //FLOOR INTEGER IS NOT ADEQUATE FOR THIS. 
    //NEEDS TO BE CEILING INTEGER FOR TOP LEFT, FLOOR INTEGER FOR BOTTOM RIGHT.
    return cell_at((coords.x() * bn::fixed(0.125)).floor_integer(),(coords.y() * bn::fixed(0.125)).floor_integer());
}

bn::regular_bg_map_cell level::cell_at(const unsigned short &xtile, const unsigned short &ytile) const{
    if(xtile <= _COLUMNS && ytile <= _ROWS){
        return _bg.map_item().cell(xtile, ytile);
    } else return _bg.map_item().cell(0,0);
}

bool level::is_thick_ground(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return !!(tile_flags::FLOOR & _tile_flags.at(cell_type));
}

bool level::tile_has_flag(const bn::point &coords, const tile_flags &flags){
    return !!(flags & _tile_flags[cell_at(coords.x(), coords.y())]);
}

bool level::tile_has_flag(const bn::fixed_point &coords, const tile_flags &flags){
    return !!(flags & _tile_flags[cell_at(coords)]);
}

bool level::is_thin_ground(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _THIN_GROUND;
}

bool level::is_leftfacing_wall(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _LF_WALL
        || cell_type == _LFT_CORNER
        || cell_type == _LFB_CORNER;
}

bool level::is_rightfacing_wall(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _RF_WALL
        || cell_type == _RFT_CORNER 
        || cell_type == _RFB_CORNER;
}

bool level::is_up_slope(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _UP_SLOPE;
}

bool level::is_down_slope(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _DOWN_SLOPE;
}

bool level::is_ceiling(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _CEILING;
}

void level::print_hitbox(const bn::fixed_rect &hitbox) const{
    for(bn::fixed ycor = hitbox.top(); ycor <= hitbox.bottom(); ycor += 8){
        bn::string<2048> logstr;
        bn::ostringstream logstream(logstr);

        for(bn::fixed xcor = hitbox.left(); xcor <= hitbox.right(); xcor += 8){
            logstream << cell_at(bn::fixed_point(xcor, ycor));
            logstream << " ";

        }
        BN_LOG(logstr);

    }
}


}