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
#include "hud.h"
#include "enemy.h"


int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);

    //multiply by 4 b/c we want the center of the map.
    // the dimensions are measurecd in 8x8 tiles, so 8*tiles = pixels. 8/2 = 4 
    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::camera_ptr cam = bn::camera_ptr::create(128,128);
    aru::level level(cam, bn::regular_bg_items::testmap);
    aru::player player(cam, 128, 128, level);
    bn::optional<aru::enemy> enemy;
    enemy.emplace(cam, 328, 128, level);
    aru::hud hud;
    bg.set_camera(cam);

    uint8_t direction_timer = 0;



    while(true)
    {
        bool was_facing_right = player.facing_right();

        if(enemy){
            if(!player.in_iframes() && player.hitbox().intersects(enemy->hitbox())){
                bn::fixed hori_kb = 6 * (player.facing_right() ? 1 : -1); 
                uint8_t old_hp = enemy->hp();
                enemy->hit(player.contact_damage(),hori_kb,-3);
                hud.update_enemy_hp("GLOBLIN", old_hp, enemy->hp());
            }
            if(enemy->hitbox().intersects(player.hitbox())){
                bn::fixed hori_kb = 6 * (player.facing_right() ? -1 : 1); 
                player.hit(enemy->contact_damage(),hori_kb,-3);
            }
            enemy->update();
            if(enemy->hp() == 0){
                enemy.reset();
            }
        }

        hud.update_player_hp(player.hp());
        if(player.hp() == 0){
            //you died
            bn::core::reset();
        }
        player.update();
        hud.update();
        

        if(was_facing_right != player.facing_right()){
            //this means they changed direction
            direction_timer = 120;
        }
        // bn::fixed_point target_cam_offset(40 * (player.facing_left() ? 1 : -1), -30);

        //todo: maybe add easing to this ?
        bn::fixed xoffset = cam.x() - player.x();
        bn::fixed target_xoffset = 50 * (player.facing_right() ? 1 : -1);
        bn::fixed cam_xspeed = player.xspeed() * (player.facing_right() ? bn::fixed(1.5) : bn::fixed(-1.5));

        // // BN_LOG("grounded? ", player.grounded());
        if(direction_timer){
            --direction_timer;
        }
        if(bn::keypad::left_held() || bn::keypad::right_held()){
            if(xoffset < target_xoffset){
                xoffset += cam_xspeed;
            }else if(xoffset > target_xoffset){
                xoffset -= cam_xspeed;
            }
        }



        if(target_xoffset - cam_xspeed < xoffset && xoffset < target_xoffset + cam_xspeed){
            xoffset = target_xoffset;
        }

        bn::fixed yoffset = cam.y() - player.y();
        bn::fixed target_yoffset = cam.y() - player.y();
        bn::fixed cam_yspeed = 2;
        if(player.on_flat_ground()){
            // BN_LOG(cam.y() - player.hitbox().bottom());
            // yoffset = player.hitbox().bottom();
            // target_yoffset = cam.y() - player.last_solid_ground();
            // if(yoffset > target_yoffset){
            //     yoffset += cam_yspeed;
            // }else if(yoffset < target_yoffset){
            //     yoffset -= cam_yspeed;
            // }
        }else{
            if(yoffset > 60){
                yoffset = 60;
            }else if(yoffset < -40){
                yoffset = -40;
            }

        }

        

        bn::fixed new_x = player.x() + xoffset;
        bn::fixed new_y = player.y() + yoffset;
        if(new_x < 120) new_x = 120;
        if(new_x > (level.width() - 120)) new_x = level.width() - 120;
        if(new_y < 88) new_y = 88;
        if(new_y > (level.height() - 80)) new_y = level.height() - 80;
        cam.set_position(new_x, new_y);
        bn::core::update();

        if(bn::keypad::start_held() && bn::keypad::select_held() &&
            bn::keypad::a_held() && bn::keypad::b_held()){
            bn::core::reset();
        }
    }
}