
#pragma once
#include <bn_optional.h>


namespace aru {

enum class scene_type
{
    GJ24,
    TITLE,
    LEVEL,
    LAB
};

class scene {

    public:
        virtual ~scene() = default;

        [[nodiscard]] virtual bn::optional<scene_type> update() = 0;

    protected:
        scene() = default;
};

}