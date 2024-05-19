#include <bn_core.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_bg_palettes.h>
#include <bn_log.h>
#include <bn_keypad.h>


#include "bn_regular_bg_items_testbg.h"
#include "player.h"

//p is the progress of the ease, represented as a fraction of total time
bn::fixed ease(bn::fixed p)
{
    //linear ease
    // return p;

	// if(p < 0.5)
	// {
	// 	return 2 * p * p;
	// }
	// else
	// {

    //quadratic ease out
	return -(p * (p - 2));
	// }
}

int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);
    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::camera_ptr cam = bn::camera_ptr::create(0,0);    bg.set_camera(cam);
    aru::player player(cam, 0, 0, 2, 0.5);
    bn::fixed_point cam_offset(40 * (player.facing_left() ? 1 : -1), -30);
    unsigned short timer,ease_time;
    timer = -1;
    ease_time = 20;

    bn::fixed xoffset_start;
    bn::fixed xoffset_target;


    while(true)
    {
        bool was_facing_left = player.facing_left();
        player.update();
        bn::fixed xoffset;

        // i hate this. but it's a camera ease
        if(!was_facing_left && player.facing_left()){
            xoffset_start = -40;
            xoffset_target = 40;
            timer = 0;
        }

        if(was_facing_left && !player.facing_left()){
            xoffset_start = 40;
            xoffset_target = -40;
            timer = 0;
        }

        if(timer < ease_time){
            ++timer;
            xoffset = (xoffset_start - xoffset_target) * ease(bn::fixed(timer) / bn::fixed(ease_time)) - xoffset_start;
            cam_offset.set_x(xoffset);
        }


        // BN_LOG("camera offset ", cam_offset);
        cam.set_position(player.position() + cam_offset);
        bn::core::update();
    }
}