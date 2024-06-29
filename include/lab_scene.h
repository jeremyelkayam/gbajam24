#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include "scene.h"
#include "level.h"
#include "player.h"
#include "level.h"
#include "text_box.h"

namespace aru{

class lab_scene : public scene{ 
private:
    bn::camera_ptr _cam;
    level _level;
    player _player;
    bn::regular_bg_ptr _bg;
    bn::optional<text_box> _text_box;

public:
    lab_scene();
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
};

}