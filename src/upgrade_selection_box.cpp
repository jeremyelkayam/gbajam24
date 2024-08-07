#include "upgrade_selection_box.h"
#include "text_box.h"
#include <bn_keypad.h>
#include <bn_log.h>

namespace aru {

upgrade_selection_box::upgrade_selection_box(bn::sprite_text_generator &text_generator,
    uint8_t &upgrade_lvl, uint16_t &ultramatter, const uint16_t &price,
    const bn::string<16> &upgrade_type) : 
        selection_box(text_generator, [&upgrade_type, &price]{
                    bn::string<64> result;
                    bn::ostringstream stream(result);
                    stream << "Upgrade ";
                    stream << upgrade_type;
                    stream << " for ";
                    stream << price;
                    stream << " ultramatter?";

                    return result;
                }(), 
                []{
                    bn::vector<bn::string<8>,4> result;
                    result.push_back("No");
                    result.push_back("Yes");
                    return result;
                }()
            ),
        _upgrade_lvl(upgrade_lvl),
        _ultramatter(ultramatter),
        _price(price),
        _upgrade_type(upgrade_type),
        _upgraded(false),
        _not_enough_money(false)
{
    
}

//todo: for these, probably should make virtual confirm/cancel functions
void upgrade_selection_box::update(){
    selection_box::update();
    if(bn::keypad::a_pressed() && _selected_option == 1){
        _done = true;
        if(_ultramatter >= _price){
            _upgraded = true;
            ++_upgrade_lvl;
            _ultramatter -= _price;
        }else{
            _not_enough_money = true;
        }
    }else if(bn::keypad::a_pressed() || bn::keypad::b_pressed()){
        _done = true;
    }
}

bn::unique_ptr<box> upgrade_selection_box::next_box(){
    BN_ASSERT(_done);
    bn::string<256> result;
    bn::ostringstream stream(result);
    if(_upgraded) {
        stream << _upgrade_type;
        stream << " ability was upgraded to level ";
        stream << _upgrade_lvl + 1;
        stream << ".";
    }else if(_not_enough_money){
        stream << "ERROR: Insufficient ultramatter stored (current: ";
        stream << _ultramatter;
        stream << "). Upgrade could not be performed.";
    }

    if (result.length() > 0)
    {
        return bn::unique_ptr<box>(new text_box(_text_generator, result.c_str(), true));
    }
    

    return bn::unique_ptr<box>();
    
}


}
