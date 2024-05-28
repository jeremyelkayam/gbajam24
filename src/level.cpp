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
    _cells(_bg_ptr.map().cells_ref().value())
{
    _bg_ptr.set_camera(cam);
    BN_LOG("width: ", _bg.map_item().dimensions().width(), " tiles");
    BN_LOG("height: ", _bg.map_item().dimensions().height(), " tiles");
    BN_LOG("cells: ", _cells.size());

    // for(unsigned short tiley = 0; tiley < 32 ; ++tiley){
    //     bn::string<1024> logstr;
    //     bn::ostringstream logstream(logstr);
    //     for(unsigned short tilex = 0; tilex < _COLUMNS ; ++tilex){
    //         logstream << cell_at(tilex, tiley);
    //         logstream << " ";
    //     }
    //     BN_LOG(logstr);
    // }

}

bn::regular_bg_map_cell level::cell_at(const bn::fixed_point &coords) const{
    return cell_at((coords.x() / 8).floor_integer(),(coords.y() / 8).floor_integer());
}

bn::regular_bg_map_cell level::cell_at(const unsigned short &xtile, const unsigned short &ytile) const{
    return _bg.map_item().cell(xtile, ytile);
}

bool level::is_ground(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == 1 || cell_type == 2;
}

bool level::is_thin_ground(const bn::fixed_point &coords) const{
    bn::regular_bg_map_cell cell_type = cell_at(coords);
    return cell_type == 2;
}

}