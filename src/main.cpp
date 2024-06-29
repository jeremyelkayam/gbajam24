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
#include "bn_sprite_items_portrait.h"
#include "player.h"
#include "level.h"
#include "text_box.h"
#include "hud.h"
#include "enemy.h"
#include "lab_scene.h"
#include "cute_prop_sprite_font.h"


int main()
{
    bn::core::init();
    bn::regular_bg_ptr bg = bn::regular_bg_items::testbg.create_bg(0,0);
    // bn::sprite_text_generator boxgen(aru::cute_prop_sprite_font);

    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    bn::camera_ptr cam = bn::camera_ptr::create(128,128);

    bn::unique_ptr<aru::scene> scene;
    bn::optional<aru::scene_type> next_scene = aru::scene_type::LAB;

    // bn::optional<aru::enemy> enemy;
    // bn::optional<aru::text_box> box;

    // aru::level level(cam, bn::regular_bg_items::testmap);
    // aru::player player(cam, 128, 128, level);
    // enemy.emplace(cam, 328, 128, level);
    // bg.set_camera(cam);

   while(true)
    {
        if(scene){
            next_scene = scene->update();
        }
        if(next_scene){
            switch(*next_scene){
                case aru::scene_type::LAB: { 
                    scene.reset(new aru::lab_scene());
                    break;
                }
                default: { 
                    BN_ERROR("the selected screen does not exist or is not yet implemented");
                    break;
                }
            }
        }


    // while(true)
    // {
    //     if(!box){
    //         bool was_facing_right = player.facing_right();

    //         if(enemy){
    //             uint8_t old_hp = enemy->hp();
    //             if(!player.in_iframes() && player.hitbox().intersects(enemy->hitbox())){
    //                 bn::fixed hori_kb = 6 * (player.facing_right() ? 1 : -1); 
    //                 enemy->hit(player.contact_damage(),hori_kb,-3);
    //                 hud.update_enemy_hp("GLOBLIN", old_hp, enemy->hp(), enemy->max_hp());
    //             }
    //             if(enemy->hitbox().intersects(player.hitbox())){
    //                 bn::fixed hori_kb = 6 * (player.facing_right() ? -1 : 1); 
    //                 player.hit(enemy->contact_damage(),hori_kb,-3);
    //             }
    //             if(player.check_bullet_collision(*enemy.get())){
    //                 hud.update_enemy_hp("GLOBLIN", old_hp, enemy->hp(), enemy->max_hp());
    //             }
    //             enemy->update();
    //             if(enemy->delete_me()){
    //                 enemy.reset();
    //             }
    //         }

    //         hud.update_player_hp(player.hp());
    //         if(player.hp() == 0){
    //             //you died
    //             bn::core::reset();
    //         }
    //         player.update();
    //         hud.update();
            

    //         if(was_facing_right != player.facing_right()){
    //             //this means they changed direction
    //             direction_timer = 120;
    //         }
    //         // bn::fixed_point target_cam_offset(40 * (player.facing_left() ? 1 : -1), -30);

    //         //todo: maybe add easing to this ?
    //         bn::fixed xoffset = cam.x() - player.x();
    //         bn::fixed target_xoffset = 50 * (player.facing_right() ? 1 : -1);
    //         bn::fixed cam_xspeed = player.xspeed() * (player.facing_right() ? bn::fixed(1.5) : bn::fixed(-1.5));

    //         // // BN_LOG("grounded? ", player.grounded());
    //         if(direction_timer){
    //             --direction_timer;
    //         }
    //         if(bn::keypad::left_held() || bn::keypad::right_held()){
    //             if(xoffset < target_xoffset){
    //                 xoffset += cam_xspeed;
    //             }else if(xoffset > target_xoffset){
    //                 xoffset -= cam_xspeed;
    //             }
    //         }

    //         if(bn::keypad::start_pressed()){
    //             box.emplace(boxgen, "This is a text box test. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", bn::sprite_items::portrait);
    //         }



    //         if(target_xoffset - cam_xspeed < xoffset && xoffset < target_xoffset + cam_xspeed){
    //             xoffset = target_xoffset;
    //         }

    //         bn::fixed yoffset = cam.y() - player.y();
    //         bn::fixed target_yoffset = cam.y() - player.y();
    //         bn::fixed cam_yspeed = 2;
    //         if(player.on_flat_ground()){
    //             // BN_LOG(cam.y() - player.hitbox().bottom());
    //             // yoffset = player.hitbox().bottom();
    //             // target_yoffset = cam.y() - player.last_solid_ground();
    //             // if(yoffset > target_yoffset){
    //             //     yoffset += cam_yspeed;
    //             // }else if(yoffset < target_yoffset){
    //             //     yoffset -= cam_yspeed;
    //             // }
    //         }else{
    //             if(yoffset > 60){
    //                 yoffset = 60;
    //             }else if(yoffset < -40){
    //                 yoffset = -40;
    //             }

    //         }

    //         bn::fixed new_x = player.x() + xoffset;
    //         bn::fixed new_y = player.y() + yoffset;
    //         if(new_x < 120) new_x = 120;
    //         if(new_x > (level.width() - 120)) new_x = level.width() - 120;
    //         if(new_y < 88) new_y = 88;
    //         if(new_y > (level.height() - 80)) new_y = level.height() - 80;
    //         cam.set_position(new_x, new_y);
    //     }else{
    //         box->update();
    //         if(box->done()){
    //             box.reset();
    //         }
    //         if(bn::keypad::a_pressed()){
    //             box->advance();
    //         }
    //     }

        bn::core::update();

        if(bn::keypad::start_held() && bn::keypad::select_held() &&
            bn::keypad::a_held() && bn::keypad::b_held()){
            bn::core::reset();
        }
    }
}