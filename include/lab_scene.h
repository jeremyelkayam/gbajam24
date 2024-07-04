#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_unique_ptr.h>
#include <bn_deque.h>
#include "scene.h"
#include "level.h"
#include "player.h"
#include "level.h"
#include "text_box.h"
#include "selection_box.h"
#include "interactable_entity.h"
#include "common_stuff.h"
#include "donation_box.h"

namespace aru{

class lab_scene : public scene{ 
private:
    common_stuff &_cstuff;
    bn::camera_ptr _cam;
    level _level;
    player _player;
    interactable_entity _slung;

    bn::sprite_ptr _interact_icon;
    bn::sprite_animate_action<2> _interact_icon_anim;
    bn::regular_bg_ptr _bg;
    bn::deque<bn::unique_ptr<text_box>, 16> _text_boxes;
    // selection_box _menu;

public:
    lab_scene(common_stuff &cstuff);
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
};

}