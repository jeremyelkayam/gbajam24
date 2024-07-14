#include <bn_core.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_bg_palettes.h>
#include <bn_log.h>
#include <bn_keypad.h>

//copyrat90 demo imports

#include "bn_regular_bg_items_testbg.h"
#include "bn_regular_bg_items_testmap.h"
#include "bn_sprite_items_portrait.h"
#include "hud.h"
#include "lab_scene.h"
#include "menu_scene.h"
#include "level_scene.h"
#include "common_stuff.h"
#include "cute_prop_sprite_font.h"


int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(bn::color(25, 25, 25));
    // bn::camera_ptr cam = bn::camera_ptr::create(128,128);

    bn::unique_ptr<aru::scene> scene;
    bn::optional<aru::scene_type> next_scene = aru::scene_type::MENU;

    aru::common_stuff cstuff;
    
    while(true)
    {
        cstuff.rand.update();
        if(scene){
            next_scene = scene->update();
        }
        if(next_scene){
            //yeah kinda shitty but w/e
            //todo: scene transitions
            scene.reset();
            switch(*next_scene){
                case aru::scene_type::LAB: { 
                    scene.reset(new aru::lab_scene(cstuff));
                    break;
                }
                case aru::scene_type::MENU: { 
                    scene.reset(new aru::menu_scene());
                    break;
                }
                case aru::scene_type::LEVEL: { 
                    scene.reset(new aru::level_scene(cstuff));
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
    //     }
    //     }

        bn::core::update();

        if(bn::keypad::start_held() && bn::keypad::select_held() &&
            bn::keypad::a_held() && bn::keypad::b_held()){
            bn::core::reset();
        }
    }
}