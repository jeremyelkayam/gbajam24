#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>
#include "bn_regular_bg_items_lab.h"
#include "bn_regular_bg_items_testbg.h"
#include "bn_sprite_items_dummy_sprite.h"
#include "bn_sprite_items_slung.h"
#include "bn_sprite_items_a_button_prompt.h"
#include "bn_sprite_items_portrait.h"

#include "lab_scene.h"

namespace aru {

lab_scene::lab_scene(common_stuff &cstuff) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(128,128)),
    _level(_cam, bn::regular_bg_items::lab),
    _player(_cam,128,128,_level),
    // _slung(_cam, 168,180),
    // _vax_mchn(_cam, 210,172,50,50,bn::sprite_items::dummy_sprite),
    _interact_icon(bn::sprite_items::a_button_prompt.create_sprite(0, 0)),
    _interact_icon_anim(bn::create_sprite_animate_action_forever(_interact_icon, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1)),
    _bg(bn::regular_bg_items::testbg.create_bg(0,0)){
    _bg.set_z_order(1);
    _interact_icon.set_visible(false);
    _interact_icon.set_camera(_cam);
    _interactables.emplace_front(bn::unique_ptr<slung>(new slung(_cam, 168, 180, _cstuff)));
    
}

bn::optional<scene_type> lab_scene::update(){
    bn::optional<scene_type> result;
    if(!_text_boxes.empty()){
        
        _interact_icon.set_visible(false);
        _text_boxes.front()->update();
        _text_boxes.front()->set_visible(true);        

        if(_text_boxes.front()->done() || (bn::keypad::select_pressed() && !_text_boxes.front()->input_required())){
            bn::unique_ptr<box> next_box = _text_boxes.front()->next_box();
            _text_boxes.pop_front();
            if(next_box) _text_boxes.push_front(bn::move(next_box));            

            BN_LOG("ending current box");
            if(!_text_boxes.empty()){
                _text_boxes.front()->set_visible(true);
            }
        }
        else if(bn::keypad::start_pressed())
        {
            while(!_text_boxes.front()->input_required()){
                _text_boxes.pop_front();
            }
        }
    }else{
        // _slung.update();
        // _vax_mchn.update();
        _player.update();

        bool can_interact = false;

        for(bn::unique_ptr<interactable_entity> &ent : _interactables){
            if(_player.hitbox().intersects(ent->hitbox())) {
                _interact_icon.set_position(ent->x(), ent->hitbox().top() - 16);
                can_interact = true;

                if(bn::keypad::a_pressed()){
                    _text_boxes = ent->interact_boxes();
                }

                break; //shouldn't be able to interact w/ multiple entities
                //so just break at the first one in the list
            }
        }
        if(can_interact){
            _interact_icon.set_visible(true);
            _interact_icon_anim.update();
        }else{
            _interact_icon.set_visible(false);

        }

        // if(_player.hitbox().intersects(_slung.hitbox())){
            
            
        //     _interact_icon.set_visible(true);
        //     _interact_icon_anim.update();

        //     //todo: maybe make the interactable entities each return their own deque of boxes
        //     if(bn::keypad::a_pressed()){
        //     }

        // }else if(_player.hitbox().intersects(_vax_mchn.hitbox())){

        //     if(bn::keypad::a_pressed()){
        //         _text_boxes.push_back(bn::unique_ptr<donation_box>(new donation_box(_cstuff.text_generator, _cstuff.savefile.ultramatter)));
        //     }
        // }else{
        // }
    }


    return result;
}

}