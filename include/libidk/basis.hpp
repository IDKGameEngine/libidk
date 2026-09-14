#pragma once

#include <glm/glm.hpp>

namespace idk
{
    namespace world_axis
    {
        inline constexpr glm::vec3 FORWARD = glm::vec3( 0,  0, -1);
        inline constexpr glm::vec3 RIGHT   = glm::vec3(+1,  0,  0);
        inline constexpr glm::vec3 UP      = glm::vec3( 0, +1,  0);
    }
}

