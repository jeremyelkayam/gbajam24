#pragma once
#include "common_stuff.h"
#include "scene.h"
#include <bn_regular_bg_ptr.h>

namespace aru{

class pause_info_scene : public scene{ 
protected:

    bn::vector<bn::sprite_ptr, 64> _text_sprites;
    // bn::regular_bg_ptr _bg;
    // uint8_t _timer;
    scene_type _last_scene;
 
public:
    pause_info_scene(bn::sprite_text_generator &generator, 
        scene_type last_scene, 
        const common_stuff::saved_data &savefile);
    [[nodiscard]] virtual bn::optional<scene_type> update();
};

}