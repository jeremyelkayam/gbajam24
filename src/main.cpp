#include <bn_core.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_camera_ptr.h>
#include <bn_bg_palettes.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_music.h>

#include "bn_regular_bg_items_testbg.h"
#include "bn_regular_bg_items_testmap.h"
#include "hud.h"
#include "lab_scene.h"
#include "logos_scene.h"
#include "menu_scene.h"
#include "level_scene.h"
#include "common_stuff.h"
#include "cute_prop_sprite_font.h"
#include "transition.h"


int main()
{
    bn::core::init();
    constexpr uint8_t transition_time = 20;

    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    
    bn::unique_ptr<aru::scene> scene;
    aru::Transition transition(aru::Transition::Types::FADE, 
        aru::Transition::Direction::IN, transition_time * 2);
    transition.Init();
    bn::optional<aru::scene_type> next_scene;
    bn::blending::set_white_fade_color();
    scene.reset(new aru::logos_scene());
    scene->set_transition_effects_enabled(true);

    aru::common_stuff cstuff;
    aru::Transition::Types fade_and_mosaic = aru::Transition::Types::FADE 
        | aru::Transition::Types::BG_MOSAIC | aru::Transition::Types::SPRITE_MOSAIC;
    
    while(true)
    {
        cstuff.rand.update();

        if(transition.GetState() != aru::Transition::State::DONE){
            transition.Update();

            //after finishing the transition out, clear out the scene
            if(transition.GetDirection() == aru::Transition::Direction::OUT && 
                transition.GetState() == aru::Transition::State::DONE){
                scene.reset();
                if(bn::music::playing()) bn::music::stop();
            }
        }else{
            
            if(scene){
                next_scene = scene->update();
            }

            if(next_scene){

                if(transition.GetDirection() == aru::Transition::Direction::OUT){
                    // bn::bg_palettes::set_transparent_color(bn::color(0,0,0));
                    //fading out is done, let's instantiate the transition and fade in
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
                        case aru::scene_type::LOGOS: { 
                            scene.reset(new aru::logos_scene());
                            break;
                        }
                        default: { 
                            BN_ERROR("the selected screen does not exist or is not yet implemented");
                            break;
                        }
                    }
                    scene->set_transition_effects_enabled(true);
                    transition.Set(fade_and_mosaic, aru::Transition::Direction::IN,
                         transition_time);
                    transition.Init();
                    BN_LOG("fade in on new scene");
                }else{
                    //we need to fade out
                    scene->set_transition_effects_enabled(true);
                    bn::blending::set_black_fade_color();
                    transition.Set(fade_and_mosaic, aru::Transition::Direction::OUT, 
                        transition_time);
                    transition.Init();                    
                }
            }
        
        }

        bn::core::update();

        if(bn::keypad::start_held() && bn::keypad::select_held() &&
            bn::keypad::a_held() && bn::keypad::b_held()){
            bn::core::reset();
        }
    }
}