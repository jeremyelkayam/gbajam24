#include <bn_core.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_bg_palettes.h>
#include <bn_log.h>
#include <bn_keypad.h>

//copyrat90 demo imports
#include <bn_regular_bg_map_cell_info.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_format.h>


#include "bn_regular_bg_items_testbg.h"
#include "bn_regular_bg_items_testmap.h"
#include "player.h"


int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);
    bn::regular_bg_ptr map = bn::regular_bg_items::testmap.create_bg(
        bn::regular_bg_items::testmap.map_item().dimensions().width() * 4,
        bn::regular_bg_items::testmap.map_item().dimensions().height() * 4); 
    //multiply by 4 b/c we want the center of the map.
    // the dimensions are measured in 8x8 tiles, so 8*tiles = pixels. 8/2 = 4 
    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::camera_ptr cam = bn::camera_ptr::create(128,128);
    map.set_camera(cam);
    aru::player player(cam, 128, 128);
    


    //Tile background DEMO by copyrat90
    // bn::regular_bg_map_cell is the unique id number,
    // which is just a type alias of uint16_t.
    bn::span<const bn::regular_bg_map_cell> cells = map.map().cells_ref().value();
    // You can get unique id by indexing `cells` like this:
    int x = 1, y = 0;
    // COLUMNS: how many tiles in a row?
    constexpr int COLUMNS = bn::regular_bg_items::testmap.map_item().dimensions().width();
    // Get the unique id (which is not a tile index, yet)
    bn::regular_bg_map_cell unique_id_of_x_by_y = cells[x + y * COLUMNS];
    // Convert the unique id to a tile index
    bn::regular_bg_map_cell_info cell_info(unique_id_of_x_by_y);
    int tile_index = cell_info.tile_index();
    BN_LOG(bn::format<32>("({},{}) is {}", x, y, tile_index));



    while(true)
    {
        player.update();
        // bn::fixed_point target_cam_offset(40 * (player.facing_left() ? 1 : -1), -30);

        //todo: maybe add easing to this ?
        bn::fixed xoffset = cam.x() - player.x();
        bn::fixed target_xoffset = 50 * (player.facing_left() ? 1 : -1);
        bn::fixed camspeed = 4;

        unsigned int tile_x,tile_y;
        tile_x = (player.hitbox().bottom_right().x() / 8).floor_integer();
        tile_y = (player.hitbox().bottom_right().y() / 8).floor_integer();

        BN_LOG("bottom right tile :", tile_x, ",", tile_y);
        BN_LOG("bottom right tile cell type: ", cells[tile_x + tile_y * COLUMNS]);

        if(xoffset < target_xoffset){
            xoffset += camspeed;
        }else if(xoffset > target_xoffset){
            xoffset -= camspeed;
        }

        if(target_xoffset - camspeed < xoffset && xoffset < target_xoffset + camspeed){
            xoffset = target_xoffset;
        }
        

        // BN_LOG("camera offset ", cam_offset);
        cam.set_position(player.x() + xoffset, 128);
        bn::core::update();
    }
}