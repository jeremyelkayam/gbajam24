
#pragma once
#include <bn_optional.h>


namespace aru {

enum class scene_type
{
    LOGOS,
    TITLE,
    LEVEL,
    LAB,
    MENU,
    BACK,
    SOUND_TEST
};

class scene {

    public:
        virtual ~scene() = default;

        [[nodiscard]] virtual bn::optional<scene_type> update() = 0;
        virtual void set_transition_effects_enabled(bool enabled) = 0;

    protected:
        scene() = default;
};

}