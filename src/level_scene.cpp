#include "level_scene.h"

#include <bn_log.h>

namespace aru {

level_scene::level_scene(common_stuff &cstuff) :
    play_scene(cstuff, test_level_data)
{
    
}

bn::optional<scene_type> level_scene::update()
{
    bn::optional<scene_type> result;

    return play_scene::update();
}

}