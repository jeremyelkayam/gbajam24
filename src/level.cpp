#include <bn_log.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_regular_bg_map_cell_info.h>
#include <bn_sstream.h>
#include <bn_string.h>


#include "level.h"


namespace aru {

level::level(const bn::camera_ptr &cam, const bn::regular_bg_item &bg) : 
    _bg(bg),
    _bg_ptr(_bg.create_bg(bg.map_item().dimensions().width() * 4,
        _bg.map_item().dimensions().height() * 4)),
    _COLUMNS(_bg.map_item().dimensions().width()),
    _cells(_bg_ptr.map().cells_ref().value()),
    _THICK_GROUND(cell_at(1,0)),
    _THIN_GROUND(cell_at(3,0)),
    _RF_WALL(cell_at(5,0)),
    _LF_WALL(cell_at(7,0)),
    _CEILING(cell_at(9,0)),
    _RFT_CORNER(cell_at(11,0)),
    _LFT_CORNER(cell_at(13,0)),
    _RFB_CORNER(cell_at(15,0)),
    _LFB_CORNER(cell_at(17,0))
{
    _bg_ptr.set_camera(cam);
    BN_LOG("width: ", _bg.map_item().dimensions().width(), " tiles");
    BN_LOG("height: ", _bg.map_item().dimensions().height(), " tiles");
    // BN_LOG("cells: ", _cells.size());

    for(unsigned short tiley = 0; tiley < 32 ; ++tiley){
        bn::string<2048> logstr;
        bn::ostringstream logstream(logstr);
        for(unsigned short tilex = 0; tilex < (_COLUMNS / 2) ; ++tilex){
            logstream << cell_at(tilex, tiley);
            logstream << " ";
        }
        BN_LOG(logstr);
    }
    BN_LOG("thin ground: ", _THIN_GROUND);

}

bn::regular_bg_map_cell level::cell_at(const bn::fixed_point &coords) const{
    return cell_at((coords.x() / 8).floor_integer(),(coords.y() / 8).floor_integer());
}

bn::regular_bg_map_cell level::cell_at(const unsigned short &xtile, const unsigned short &ytile) const{
    return _bg.map_item().cell(xtile, ytile);
}

bool level::is_thick_ground(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == _THICK_GROUND 
        || cell_type == _RFT_CORNER || cell_type == _LFT_CORNER;
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


}