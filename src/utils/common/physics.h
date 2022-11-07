#pragma once

#include <glm/glm.hpp>

namespace WhiteDeer {
namespace Utils {

using glm::vec3;

struct AABB {
    AABB() {}
    AABB(vec3 _min, vec3 _max): m_min(_min), m_max(_max) {}
    vec3 m_min;
    vec3 m_max;
};

}  // namespace Utils
}  // namespace WhiteDeer