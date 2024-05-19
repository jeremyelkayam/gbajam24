#include <bn_core.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_bg_palettes.h>
#include <bn_keypad.h>
#include <bn_log.h>

#include "bn_sprite_items_arutest.h"
#include "bn_regular_bg_items_testbg.h"

int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);
    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::sprite_ptr player = bn::sprite_items::arutest.create_sprite(0,0);
    bn::camera_ptr cam = bn::camera_ptr::create(0,0);
    player.set_camera(cam);
    bg.set_camera(cam);


    while(true)
    {
        if(bn::keypad::left_held()){
            player.set_x(player.x() - 1);
            player.set_horizontal_flip(false);
        }
        if(bn::keypad::right_held()){
            player.set_x(player.x() + 1);
            player.set_horizontal_flip(true);
        }        
        bn::fixed cam_offset = 40 * (player.horizontal_flip() ? 1 : -1);
        bn::fixed_point cam_pos(player.x() + cam_offset, player.y());
        BN_LOG("camera offset ", cam_offset);
        cam.set_position(cam_pos);
        bn::core::update();
    }
}
