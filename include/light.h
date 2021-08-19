#pragma once
#include "maths.h"
#include "camera.h"

//Phong Lighting Model
struct Light
{
    Light();
    Light(maths::vec3f position);
    Light(maths::vec3f position, float, float, float);

    void setParams(float,float,float);

    maths::vec3f position;

    float ambientInt;
    float pointInt;
    float specularCoefficient;

    float ambientConstant = 1;
    float diffuseConstant = 1;
    float specularConstant = 1;

    float calculateIntensity(maths::vec3f,maths::vec3f, maths::vec3f);
    bool applyShadows(maths::vec2i);
    Camera* lightcam;

    void processKeyboard(int, float);
    maths::mat4f getLightTransform();
};