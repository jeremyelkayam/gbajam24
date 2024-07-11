#pragma once
#include <bn_sprite_ptr.h>
#include <bn_unique_ptr.h>
#include <bn_deque.h>
#include "text_box.h"
#include "save_selection_box.h"
#include "interactable_entity.h"
#include "play_scene.h"
#include "donation_box.h"

namespace aru{

class lab_scene : public play_scene{ 
private:
    bn::forward_list<bn::unique_ptr<interactable_entity>, 6> _interactables;


    bn::sprite_ptr _interact_icon;
    bn::sprite_animate_action<2> _interact_icon_anim;
    bn::deque<bn::unique_ptr<box>, 16> _text_boxes;
    interactable_entity *_interacting_with;
 
public:
    lab_scene(common_stuff &cstuff);
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
};

}