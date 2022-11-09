#pragma once

#include <glm/glm.hpp>

namespace WhiteDeer {
namespace Utils {

using glm::vec3;

struct AABB {
    AABB() {}
    AABB(vec3 _center, vec3 _extent): center(_center), extent(_extent) {}
    vec3 center;
    vec3 extent;
};

struct Plane {
    Plane() {}
    Plane(vec3 _normal, float _distance): normal(_normal), distance(_distance) {}

    vec3 normal;
    float distance;

    float GetDistanceToPoint(vec3 point) {
        return glm::dot(normal, point) + distance;
    }

    int IntersectAABB(AABB aabb) {
        float distance = GetDistanceToPoint(aabb.center);
        float diagnal = glm::dot(aabb.extent, glm::abs(normal));
        if (distance < -diagnal) return 1; // outside
        if (distance > diagnal) return -1; // inside
        return 0; // intersect
    }

    static Plane FromThreePoint(vec3 a, vec3 b, vec3 c) {
        vec3 ab = b - a;
        vec3 ac = c - a;
        return FromNormalAndPoint(glm::cross(ab, ac), a);
    }

    static Plane FromNormalAndPoint(vec3 normal, vec3 point) {
        normal = glm::normalize(normal);
        float distance = - glm::dot(normal, point);
        return Plane(normal, distance);
    }
};

}  // namespace Utils
}  // namespace WhiteDeer