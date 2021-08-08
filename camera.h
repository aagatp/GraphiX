#pragma once
#include "maths.h"


class Camera
{
public:

    Camera();
    void processKeyboard(unsigned char key, float);
    void processMouse(int x, int y);
    maths::mat4f getViewMatrix();

    float zoom;
    float deltaTime;
    maths::vec3f m_pos;
    maths::vec3f m_front;
    maths::vec3f m_up;
    maths::vec3f m_right;
    float m_speed;
    maths::vec2i m_mousePos;
    float yaw, pitch;
};

