#include "interactable_entity.h"
#include "bn_sprite_items_slung.h"
#include "bn_sprite_items_portrait.h"
#include "bn_sprite_items_hover_upgrade.h"
#include "bn_sprite_items_dummy_sprite.h"
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

slung::slung(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,30,bn::sprite_items::slung, cstuff)
{
    _anims.emplace_back(bn::create_sprite_animate_action_forever(_sprite, 10, 
        bn::sprite_items::slung.tiles_item(), 0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 1, 1, 1));
}

bn::deque<bn::unique_ptr<box>, 16> slung::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    result.push_back(bn::unique_ptr<text_box>(new text_box(_cstuff.text_generator, 
        "i can save the game for you if ya want", 
    bn::sprite_items::portrait, false, true)));         
    result.push_back(bn::unique_ptr<save_selection_box>(new save_selection_box(_cstuff)));
    return result;
}

vax_mchn::vax_mchn(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,50,bn::sprite_items::dummy_sprite, cstuff)
{
    
}

bn::deque<bn::unique_ptr<box>, 16> vax_mchn::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    result.push_back(bn::unique_ptr<donation_box>(new donation_box(
        _cstuff.text_generator, _cstuff.savefile.ultramatter)));
    return result;
}

hover_upgrader::hover_upgrader(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,40,bn::sprite_items::hover_upgrade, cstuff)
{
    
}

bn::deque<bn::unique_ptr<box>, 16> hover_upgrader::interact_boxes(){
    bn::deque<bn::unique_ptr<box>, 16> result;
    result.push_back(bn::unique_ptr<upgrade_selection_box>(new upgrade_selection_box(
            _cstuff.text_generator, _cstuff.savefile.hover_upgrade_lvl,
            _cstuff.savefile.ultramatter, 1000, "HOVER")));
    return result;
}

}