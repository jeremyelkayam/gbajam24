#pragma once
#include "common_stuff.h"
#include "scene.h"
#include <bn_regular_bg_ptr.h>

namespace aru{

class pause_info_scene : public scene{ 
protected:

    bn::vector<bn::sprite_ptr, 64> _text_sprites;
    // bn::regular_bg_ptr _bg;
 
public:
    pause_info_scene(bn::sprite_text_generator &generator,
        const common_stuff::saved_data &savefile);
    [[nodiscard]] virtual bn::optional<scene_type> update();
};

}