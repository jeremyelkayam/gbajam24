#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>
#include "bn_regular_bg_items_lab.h"
#include "bn_regular_bg_items_testbg.h"
#include "bn_sprite_items_a_button_prompt.h"

#include "lab_scene.h"

namespace aru {

lab_scene::lab_scene(common_stuff &cstuff) :
    play_scene(cstuff, bn::regular_bg_items::lab, bn::regular_bg_items::testbg),
    _interact_icon(bn::sprite_items::a_button_prompt.create_sprite(0, 0)),
    _interact_icon_anim(bn::create_sprite_animate_action_forever(_interact_icon, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1))
{
    _interact_icon.set_visible(false);
    _interact_icon.set_camera(_cam);
    _interactables.emplace_front(bn::unique_ptr<slung>(new slung(_cam, 350, 208, _cstuff)));
    _interactables.emplace_front(bn::unique_ptr<vax_mchn>(new vax_mchn(_cam, 450, 190, _cstuff)));
    _interactables.emplace_front(bn::unique_ptr<hover_upgrader>(new hover_upgrader(_cam, 100, 202, _cstuff)));
    
}

bn::optional<scene_type> lab_scene::update()
{
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

        bool can_interact = false;

        for(bn::unique_ptr<interactable_entity> &ent : _interactables){
            ent->update();

            if(_player.hitbox().intersects(ent->hitbox())) {
                _interact_icon.set_position(ent->x(), ent->hitbox().top() - 16);
                can_interact = true;

                if(bn::keypad::a_pressed()){
                    _text_boxes = ent->interact_boxes();
                }


                break;
            }
        }
        _interact_icon.set_visible(can_interact);
        _interact_icon_anim.update();

        if(!result && !(can_interact && bn::keypad::a_pressed())) result = play_scene::update();
    }

    _hud.update_currency(_cstuff.savefile.ultramatter);


    return result;
}

}