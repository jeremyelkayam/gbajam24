#include "interactable_entity.h"
#include "bn_sprite_items_slung.h"
#include "bn_sprite_items_portrait.h"
#include "bn_sprite_items_gun_upgrade.h"
#include "bn_sprite_items_hover_upgrade.h"
#include "bn_sprite_items_sword_upgrade.h"
#include "bn_sprite_items_dummy_sprite.h"
#include "bn_sprite_items_warp.h"
#include "text_box.h"
#include "save_selection_box.h"
#include "donation_box.h"
#include "upgrade_selection_box.h"

namespace aru 
{
interactable_entity::interactable_entity(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, 
    const bn::fixed &height, const bn::sprite_item &spritem, common_stuff &cstuff): 
        entity(cam, x, y, width, height, spritem),
        _current_anim(0),
        _cstuff(cstuff)
{
    
}

void interactable_entity::update(){
    if(_anims.size()){
        _anims.at(_current_anim).update();
    }
}

void interactable_entity::set_current_anim(uint8_t index){
    if(!_anims.empty()){
        BN_ASSERT(index < _anims.size());
        if(_current_anim != index){
            _current_anim = index;
            _anims.at(_current_anim).reset();        
            _anims.at(_current_anim).update();        
        }
    }

}

slung::slung(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,30,bn::sprite_items::slung, cstuff)
{
    _anims.emplace_back(bn::create_sprite_animate_action_forever(_sprite, 10, 
        bn::sprite_items::slung.tiles_item(), 2, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 1, 1, 1));

    _anims.emplace_back(bn::create_sprite_animate_action_forever(_sprite, 7, 
        bn::sprite_items::slung.tiles_item(), 4, 4, 3, 4, 4, 5, 4, 5, 4, 5));

    _anims.emplace_back(bn::create_sprite_animate_action_forever(_sprite, 20, 
        bn::sprite_items::slung.tiles_item(), 6, 7, 6, 8, 9, 8));

}



bn::deque<bn::unique_ptr<box>, 16> slung::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;

    bn::string<256> text;
    bn::ostringstream stream(text);
    stream << "Hello, ARU-133n. The machine is currently at ";
    stream << _cstuff.savefile.total_donated;
    stream << " power. Anything you can give helps.\nWhile you're here, I can also record the progress we've made thus far.";
    result.push_back(bn::unique_ptr<text_box>(new text_box(_cstuff.text_generator, 
        text.c_str(), bn::sprite_items::portrait, true, true, 1)));         
    result.push_back(bn::unique_ptr<save_selection_box>(new save_selection_box(_cstuff, 2)));
    return result;
}

vax_mchn::vax_mchn(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,50,bn::sprite_items::dummy_sprite, cstuff)
{
    
}

bn::deque<bn::unique_ptr<box>, 16> vax_mchn::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    result.push_back(bn::unique_ptr<donation_box>(new donation_box(_cstuff)));
    return result;
}

hover_upgrader::hover_upgrader(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,40,bn::sprite_items::hover_upgrade, cstuff)
{

}

bn::deque<bn::unique_ptr<box>, 16> hover_upgrader::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    if(_cstuff.savefile.hover_upgrade_lvl == 2){
        result.push_back(bn::unique_ptr<text_box>(
                new text_box(_cstuff.text_generator,
                "No more hover upgrades are available at this time. Have a nice day!", 
                true)));
    }else{
        result.push_back(bn::unique_ptr<upgrade_selection_box>(new upgrade_selection_box(
                _cstuff.text_generator, _cstuff.savefile.hover_upgrade_lvl,
                _cstuff.savefile.ultramatter, 1000, "HOVER")));

    }
    return result;
}

shoot_upgrader::shoot_upgrader(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,40,bn::sprite_items::gun_upgrade, cstuff)
{
    _sprite.set_horizontal_flip(true);
}

bn::deque<bn::unique_ptr<box>, 16> shoot_upgrader::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    result.push_back(bn::unique_ptr<upgrade_selection_box>(new upgrade_selection_box(
            _cstuff.text_generator, _cstuff.savefile.shoot_upgrade_lvl,
            _cstuff.savefile.ultramatter, 1000, "SHOOT")));
    return result;
}

slash_upgrader::slash_upgrader(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 30,40,bn::sprite_items::sword_upgrade, cstuff)
{
    _anims.emplace_back(bn::create_sprite_animate_action_forever(_sprite, 9, 
        bn::sprite_items::sword_upgrade.tiles_item(), 0, 1, 2, 1));
    _sprite.set_horizontal_flip(true);
}

bn::deque<bn::unique_ptr<box>, 16> slash_upgrader::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    result.push_back(bn::unique_ptr<upgrade_selection_box>(new upgrade_selection_box(
            _cstuff.text_generator, _cstuff.savefile.shoot_upgrade_lvl,
            _cstuff.savefile.ultramatter, 1000, "SLASH")));
    return result;
}

warp_point::warp_point(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,40,bn::sprite_items::warp, cstuff)
{
}

bn::deque<bn::unique_ptr<box>, 16> warp_point::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    bn::vector<bn::string<8>, 4> opts;
    opts.emplace_back("No");
    opts.emplace_back("Yes");
    result.push_back(bn::unique_ptr<selection_box>(new selection_box(
        _cstuff.text_generator, 
        "Warp to the next level?",
        opts)));
    return result;
}


}