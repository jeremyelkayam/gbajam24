#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>
#include "bn_regular_bg_items_lab.h"
#include "bn_regular_bg_items_testbg.h"
#include "bn_sprite_items_dummy_sprite.h"
#include "bn_sprite_items_a_button_prompt.h"
#include "bn_sprite_items_portrait.h"

#include "lab_scene.h"

namespace aru {

lab_scene::lab_scene(common_stuff &cstuff) :
    _cstuff(cstuff),
    _cam(bn::camera_ptr::create(128,128)),
    _level(_cam, bn::regular_bg_items::lab),
    _player(_cam,128,128,_level),
    _slung(_cam, 188,170,80,50,bn::sprite_items::dummy_sprite),
    _interact_icon(bn::sprite_items::a_button_prompt.create_sprite(_slung.x(), _slung.hitbox().top() - 16)),
    _interact_icon_anim(bn::create_sprite_animate_action_forever(_interact_icon, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1)),
    _bg(bn::regular_bg_items::testbg.create_bg(0,0)){
    _bg.set_z_order(1);
    _interact_icon.set_visible(false);
    _interact_icon.set_camera(_cam);
    
}

bn::optional<scene_type> lab_scene::update(){
    bn::optional<scene_type> result;
    if(_text_box){
        _interact_icon.set_visible(false);
        _text_box->update();

        if(_text_box->done()){
            _text_box.reset();
        }
        if(bn::keypad::a_pressed()){
            _text_box->advance();
        }
    }else{

        _interact_icon.set_visible(_player.hitbox().intersects(_slung.hitbox()));
        _interact_icon_anim.update();
        if(_interact_icon.visible() && bn::keypad::a_pressed()){
            _text_box.emplace(_cstuff.text_generator, "Dummy text", bn::sprite_items::portrait);
        }else{
            _player.update();
            _slung.update();
        }
    }


    return result;
}

}