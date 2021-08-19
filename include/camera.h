#pragma once
#include "maths.h"

//Pinhole camera model
class Camera
{
public:

    Camera();
    void processKeyboard(unsigned char key, float);
    maths::mat4f getViewMatrix();

    float zoom;
    maths::vec3f m_pos;
    maths::vec3f m_front;
    maths::vec3f m_up;
    maths::vec3f m_right;
    float m_speed;
    float yaw,pitch;
};
