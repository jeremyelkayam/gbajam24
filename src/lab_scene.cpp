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
    _slung(_cam, 188,180,80,50,bn::sprite_items::slung),
    _interact_icon(bn::sprite_items::a_button_prompt.create_sprite(_slung.x(), _slung.hitbox().top() - 16)),
    _interact_icon_anim(bn::create_sprite_animate_action_forever(_interact_icon, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1)),
    _bg(bn::regular_bg_items::testbg.create_bg(0,0)){
    _bg.set_z_order(1);
    _interact_icon.set_visible(false);
    _interact_icon.set_camera(_cam);
    
}

bn::optional<scene_type> lab_scene::update(){
    bn::optional<scene_type> result;
    if(!_text_boxes.empty()){
        
        _interact_icon.set_visible(false);
        _text_boxes.front()->update();

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
        _slung.update();

        _interact_icon.set_visible(_player.hitbox().intersects(_slung.hitbox()));
        _interact_icon_anim.update();
        if(_player.hitbox().intersects(_slung.hitbox()) && bn::keypad::a_pressed()){
            
            _text_boxes.push_back(bn::unique_ptr<text_box>(new text_box(_cstuff.text_generator, 
            "i can save the game for you if ya want", 
            bn::sprite_items::portrait, false, true)));         
            _text_boxes.push_back(bn::unique_ptr<save_selection_box>(new save_selection_box(_cstuff)));
            _text_boxes.front()->set_visible(true);

        }else{
            //have to put this here otherwise you jump when coming out of menus
            _player.update();
        }
    }


    return result;
}

}