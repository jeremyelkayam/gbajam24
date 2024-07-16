#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>
#include "bn_sprite_items_a_button_prompt.h"
#include "bn_sprite_items_portrait.h"
#include "selection_box.h"
#include "lab_scene.h"

namespace aru {

lab_scene::lab_scene(common_stuff &cstuff) :
    play_scene(cstuff, lab_data),
    _interact_icon(bn::sprite_items::a_button_prompt.create_sprite(0, 0)),
    _interact_icon_anim(bn::create_sprite_animate_action_forever(_interact_icon, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1))
{
    _interact_icon.set_visible(false);
    _interact_icon.set_camera(_cam);
    _interacting_with = new slung(_cam, 350, 208, _cstuff);
    _interactables.emplace_front(bn::unique_ptr<slung>((slung*)_interacting_with));
    _interactables.emplace_front(bn::unique_ptr<vax_mchn>(new vax_mchn(_cam, 450, 190, _cstuff)));
    _interactables.emplace_front(bn::unique_ptr<hover_upgrader>(new hover_upgrader(_cam, 400, 112, _cstuff)));
    _interactables.emplace_front(bn::unique_ptr<shoot_upgrader>(new shoot_upgrader(_cam, 80, 209, _cstuff)));
    _interactables.emplace_front(bn::unique_ptr<slash_upgrader>(new slash_upgrader(_cam, 110, 112, _cstuff)));
    _interactables.emplace_front(bn::unique_ptr<warp_point>(new warp_point(_cam, 256, 192, _cstuff)));

    _player.update();
    _player.move_to(315, true);
    _cam.set_position(315,150);
    for(const char *line : LV1_CUTSCENE_DIALOGUE){
        uint8_t anim_index = 1;
        if(bn::string<256>(line) == bn::string<256>("...")){
            anim_index = 2;
        }
        _text_boxes.push_back(bn::unique_ptr<text_box>(new text_box(_cstuff.text_generator, 
            line, bn::sprite_items::portrait, true, false, anim_index)));
    }

    for(bn::unique_ptr<interactable_entity> &ent : _interactables){
        ent->set_blending_enabled(true);
        ent->set_mosaic_enabled(true);
    }
}

bn::optional<scene_type> lab_scene::update()
{

    bn::optional<scene_type> result;
    uint16_t old_currency = _cstuff.savefile.ultramatter;
    bool text_box_frame = false;

    if(!_text_boxes.empty()){
        _interact_icon.set_visible(false);
        _text_boxes.front()->update();      

        if(_text_boxes.front()->done() || (bn::keypad::select_pressed() && !_text_boxes.front()->input_required())){
            bn::unique_ptr<box> next_box = _text_boxes.front()->next_box();
            _text_boxes.pop_front();
            if(next_box) _text_boxes.push_front(bn::move(next_box));
        }
        else if(bn::keypad::start_pressed())
        {
            while(!_text_boxes.empty() && !_text_boxes.front()->input_required()){
                _text_boxes.pop_front();
            }
        }
        text_box_frame = true;
    }else{
        _player.clear_target();
    }

    bool can_interact = false;

    //yeah this sucks but the scene itself controls whether or not we go to the next scene...
    if(_interacting_with == _interactables.front().get()){
        if(!_text_boxes.empty()){
            if(bn::keypad::b_pressed()){
                _text_boxes.pop_front();
            }
            if(bn::keypad::a_pressed()){
                selection_box *warp_sel_box = (selection_box *) _text_boxes.front().get();
                if(warp_sel_box->selected() == bn::string<8>("Yes")){
                    result = scene_type::LEVEL;
                }
                _text_boxes.pop_front();
            }
        }
    }

    for(bn::unique_ptr<interactable_entity> &ent : _interactables){
        ent->update();

        if(_interacting_with){
            if(_text_boxes.empty()){
                _interacting_with->set_current_anim(0);
            }else{
                _interacting_with->set_current_anim(_text_boxes.front()->anim_index());
            }
        }

        //this guarantees that you can't open a text box the same frame you close it...
        if(!text_box_frame && _player.hitbox().intersects(ent->hitbox())) {
            _interact_icon.set_position(ent->x(), ent->hitbox().top() - 16);
            can_interact = true;

            if(bn::keypad::a_pressed()){
                _text_boxes = ent->interact_boxes();
                _player.move_to(ent->x() + (ent->facing_right() ? 35 : -35), !ent->facing_right());
                _interacting_with = ent.get();
            }

            break;
        }
    }
    _interact_icon.set_visible(can_interact);
    _interact_icon_anim.update();

    if(!result && !(can_interact && bn::keypad::a_pressed())) result = play_scene::update();
    

    if(old_currency != _cstuff.savefile.ultramatter){
        _hud.update_currency(_cstuff.savefile.ultramatter);
    }



    return result;
}

}