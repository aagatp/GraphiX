#include <GL/glut.h>
#include "camera.h"

Camera::Camera()
{
    m_pos          = {0.0f, 0.0f, 0.0f};
    m_front       = {0.0f, 0.0f, -1.0f};
    m_up           = {0.0f, 1.0f, 0.0f};
    m_right        = maths::normalize(maths::cross(m_front,m_up));
}


void Camera::processKeyboard(unsigned char key,float dt)
{
    float m_velocity = m_speed*dt;
    switch (key) {  

        case 'w':
            m_pos = maths::add(m_pos,maths::mul(m_up,m_velocity));
            break;

        case 's':
            m_pos = maths::sub(m_pos,maths::mul(m_up,m_velocity));
            break;

        case 'a':
            m_pos = maths::add(m_pos,maths::mul(m_right,m_velocity));
            break;

        case 'd':
            m_pos = maths::sub(m_pos,maths::mul(m_right,m_velocity));
            break;

        case 'r':
            m_pos = maths::add(m_pos,maths::mul(m_front,m_velocity));
            break;

        case 'f':
            m_pos = maths::sub(m_pos,maths::mul(m_front,m_velocity));
            break;

        case 'z':
            zoom += 10*dt;
            if (zoom>150.0f)
                zoom = 150.0f;
            break;

        case 'x':
            zoom -= 10*dt;
            if (zoom<5.0f)
                zoom = 5.0f;
            break;
            
        case 'q':
            exit(0);
    }
}

void Camera::processMouse(int xoffset, int yoffset)
{
    float mouseSensitivity = 0.02f;
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   -= xoffset;
    pitch -= yoffset;

 
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    maths::vec3f front;
    front[0] = cos(maths::radians(yaw)) * cos(maths::radians(pitch));
    front[1] = sin(maths::radians(pitch));
    front[2] = sin(maths::radians(yaw)) * cos(maths::radians(pitch));

    m_front = maths::normalize(front);
    m_right = maths::normalize(maths::cross(m_front, m_up));  
    m_up = maths::normalize(maths::cross(m_right,m_front));
}


maths::mat4f Camera::getViewMatrix()
{
    maths::mat4f view;
    view = maths::lookAt(m_pos, maths::add(m_pos,m_front), m_up);
    return view;
}
