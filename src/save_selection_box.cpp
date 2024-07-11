#include "save_selection_box.h"
#include "text_box.h"
#include <bn_keypad.h>
#include <bn_log.h>
#include "bn_sprite_items_portrait.h"

namespace aru {

save_selection_box::save_selection_box(common_stuff &cstuff, uint8_t anim_index) : 
    selection_box(cstuff.text_generator, "Save game data?", 
            []{
                 bn::vector<bn::string<8>,4> result;
                 result.push_back("No");
                 result.push_back("Yes");
                 return result;
            }()
        ),
    _cstuff(cstuff)
{
    _anim_index = anim_index;
}

void save_selection_box::update(){
    selection_box::update();
    if(bn::keypad::a_pressed() && _selected_option == 1){
        _cstuff.save();
        _done = true;
        _next_text = "Thank you for waiting. I've successfully logged your progress thus far.";
    }else if(bn::keypad::a_pressed() || bn::keypad::b_pressed()){
        _done = true;
        _next_text = "No problem! Well, let me know if you need anything else before you head out.";
    }
}

bn::unique_ptr<box> save_selection_box::next_box(){
    BN_ASSERT(_done);
    return bn::unique_ptr<box>(new text_box(_cstuff.text_generator, 
            _next_text.c_str(), 
            bn::sprite_items::portrait, true, true, 1));
    

}


}
