#pragma once

#include <array>
#include <cmath>

namespace maths{
    using vec3f = std::array<float, 3>;
    using mat4f = std::array<std::array<float,4>,4>;

    float radians(float degree);

    float dot(vec3f a, vec3f b);

    vec3f cross(vec3f a, vec3f b);

    float veclength(vec3f v);

    vec3f normalize(vec3f a);

    vec3f mul(mat4f a, vec3f b);

    vec3f mul(vec3f a, float num);

    vec3f add(vec3f a, vec3f b);

    vec3f sub(vec3f a, vec3f b);

    mat4f orthoproject();

    mat4f rotate(float yaw, float, float);

    mat4f lookAt(vec3f eye, vec3f target, vec3f vUp);

    mat4f translate(float tx, float ty, float tz);
    
    mat4f scale(float sx, float sy, float sz);
}
