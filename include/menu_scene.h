#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include "scene.h"

namespace aru{

class menu_scene : public scene{ 
private:
    bn::vector<bn::sprite_ptr, 64> _text_sprites;
    bn::sprite_ptr _selector;


public:
    menu_scene();
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
};

}