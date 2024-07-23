
#pragma once
#include <bn_optional.h>


namespace aru {

enum class scene_type
{
    LOGOS,
    TITLE,
    LEVEL,
    LAB,
    MENU
};

class scene {

    public:
        virtual ~scene() = default;

        [[nodiscard]] virtual bn::optional<scene_type> update() = 0;

    protected:
        scene() = default;
};

}