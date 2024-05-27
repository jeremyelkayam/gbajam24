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
#include "level.h"


int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);

    //multiply by 4 b/c we want the center of the map.
    // the dimensions are measured in 8x8 tiles, so 8*tiles = pixels. 8/2 = 4 
    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::camera_ptr cam = bn::camera_ptr::create(128,128);
    aru::level level(cam, bn::regular_bg_items::testmap);
    aru::player player(cam, 128, 128, level);
    bg.set_camera(cam);



    while(true)
    {
        player.update();
        // bn::fixed_point target_cam_offset(40 * (player.facing_left() ? 1 : -1), -30);

        //todo: maybe add easing to this ?
        bn::fixed xoffset = cam.x() - player.x();
        bn::fixed target_xoffset = 50 * (player.facing_left() ? 1 : -1);
        bn::fixed camspeed = 4;

        // BN_LOG("grounded? ", player.grounded());

        if(xoffset < target_xoffset){
            xoffset += camspeed;
        }else if(xoffset > target_xoffset){
            xoffset -= camspeed;
        }

        if(target_xoffset - camspeed < xoffset && xoffset < target_xoffset + camspeed){
            xoffset = target_xoffset;
        }
        

        bn::fixed new_x = player.x() + xoffset;
        if(new_x < 120) new_x = 120;
        if(new_x > (level.width() - 120)) new_x = level.width() - 120;
        cam.set_position(new_x, 128);
        bn::core::update();
    }
}