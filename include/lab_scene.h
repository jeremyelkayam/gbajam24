#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include "scene.h"


namespace aru{

class lab_scene : public scene{ 
private:

public:
    lab_scene();
    [[nodiscard]] virtual bn::optional<scene_type> update() final;
};

}