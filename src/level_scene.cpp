#include "level_scene.h"

#include <bn_log.h>
#include "bn_regular_bg_items_testmap.h"
#include "bn_regular_bg_items_testbg.h"

namespace aru {

level_scene::level_scene(common_stuff &cstuff) :
    play_scene(cstuff, bn::regular_bg_items::testmap, bn::regular_bg_items::testbg)
{
    
}

bn::optional<scene_type> level_scene::update()
{
    bn::optional<scene_type> result;

    _player.update();

    return result;
}

}