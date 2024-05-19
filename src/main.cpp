#include <bn_core.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_bg_palettes.h>
#include <bn_log.h>
#include <bn_keypad.h>


#include "bn_regular_bg_items_testbg.h"
#include "player.h"


int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);
    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::camera_ptr cam = bn::camera_ptr::create(0,0);    bg.set_camera(cam);
    aru::player player(cam, 0, 0, 2, 0.5);


    while(true)
    {
        player.update();
        // bn::fixed_point target_cam_offset(40 * (player.facing_left() ? 1 : -1), -30);

        //todo: maybe add easing to this ?
        bn::fixed xoffset = cam.x() - player.x();
        bn::fixed target_xoffset = 50 * (player.facing_left() ? 1 : -1);
        bn::fixed camspeed = 8;

        if(xoffset < target_xoffset){
            xoffset += camspeed;
        }else if(xoffset > target_xoffset){
            xoffset -= camspeed;
        }

        if(target_xoffset - camspeed < xoffset && xoffset < target_xoffset + camspeed){
            xoffset = target_xoffset;
        }
        

        // BN_LOG("camera offset ", cam_offset);
        cam.set_position(player.x() + xoffset, player.y() - 30);
        bn::core::update();
    }
}