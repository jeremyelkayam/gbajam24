#include "interactable_entity.h"
#include "bn_sprite_items_slung.h"
#include "bn_sprite_items_portrait.h"
#include "text_box.h"
#include "save_selection_box.h"
#include "donation_box.h"
#include "bn_sprite_items_dummy_sprite.h"

namespace aru 
{
interactable_entity::interactable_entity(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, 
    const bn::fixed &height, const bn::sprite_item &spritem, common_stuff &cstuff): 
        entity(cam, x, y, width, height, spritem),
        _cstuff(cstuff)
{
    
}

void interactable_entity::update(){

}

slung::slung(const bn::camera_ptr &cam, 
    const bn::fixed &x, const bn::fixed &y, common_stuff &cstuff): 
        interactable_entity(cam, x, y, 50,50,bn::sprite_items::slung, cstuff)
{
    
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
    result.push_back(bn::unique_ptr<donation_box>(new donation_box(_cstuff.text_generator, _cstuff.savefile.ultramatter)));
    return result;
}

}