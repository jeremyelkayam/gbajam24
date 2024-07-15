#include "level_scene.h"

#include <bn_log.h>

namespace aru {

level_scene::level_scene(common_stuff &cstuff) :
    play_scene(cstuff, test_level_data)
{
    
    _cam.set_position(_player.x(),_player.y());
}

bn::optional<scene_type> level_scene::update()
{
    bn::optional<scene_type> result;

    return play_scene::update();
}

}