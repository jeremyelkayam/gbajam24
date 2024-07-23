#pragma once
#include "common_stuff.h"
#include "scene.h"
#include <bn_regular_bg_ptr.h>

namespace aru{

class logos_scene : public scene{ 
protected:

    bn::vector<bn::sprite_ptr, 64> _text_sprites;
    bn::regular_bg_ptr _bg;
    uint8_t _timer;
 
public:
    logos_scene();
    [[nodiscard]] virtual bn::optional<scene_type> update();
};

}