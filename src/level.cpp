#include <bn_log.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_regular_bg_map_cell_info.h>


#include "level.h"

namespace aru {

level::level(const bn::camera_ptr &cam, const bn::regular_bg_item &bg) : 
    _bg(bg.create_bg(bg.map_item().dimensions().width() * 4,
        bg.map_item().dimensions().height() * 4)),
    _COLUMNS(bg.map_item().dimensions().width()),
    _cells(_bg.map().cells_ref().value())
{
    _bg.set_camera(cam);

}

int level::tile_index_at(const bn::fixed_point &coords) const{
    //todo: maybe do uint16_t for x and uint8_t for ytile
    unsigned short xtile = (coords.x() / 8).floor_integer();
    unsigned short ytile = (coords.y() / 8).floor_integer();
    bn::regular_bg_map_cell_info cell_info(_cells[xtile + ytile * _COLUMNS]);
    return cell_info.tile_index();
}

}