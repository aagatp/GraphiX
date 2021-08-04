#pragma once

#include <array>
#include <vector>
#include <cmath>
#include <iostream>

namespace maths{
    using vec2i = std::array<int,2>;
    using vec2f = std::array<float,2>;

    using vec3f = std::array<float, 3>;
    using vec3i = std::array<int, 3>;
    
    using vec4f = std::array<float,4>;
    using vec4i = std::array<int,4>;

    using mat4f = std::array<std::array<float,4>,4>;
    using mat4i = std::array<std::array<int,4>,4>;

    float radians(float degree);

    float dot(vec3f a, vec3f b);

    vec3f cross(vec3f a, vec3f b);

    float veclength(vec3f v);

    vec3f normalize(vec3f a);

    mat4f mul(mat4f a, mat4f b); 

    vec3f mul(mat4f a, vec3f b);

    vec3f mul(vec3f a, float num);

    vec3f add(vec3f a, vec3f b);

    vec3f sub(vec3f a, vec3f b);

    vec3f negate(vec3f a);

    mat4f orthoproject();

    mat4f perspective(float, float);

    mat4f persproject();

    mat4f rotate(float yaw, float, float);

    mat4f lookAt(vec3f eye, vec3f target, vec3f vUp);

    mat4f translate(float tx, float ty, float tz);
    
    mat4f scale(float sx, float sy, float sz);

    void printvec(vec3f);

    float max(float,float);

    std::vector<float> interpolate(float, float, float, float);

    vec3f getnormal(vec3f, vec3f, vec3f);
}
