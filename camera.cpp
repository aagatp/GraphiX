#include <GL/glut.h>
#include "camera.h"

Camera::Camera()
{
    m_pos         = {0.0f, 10.0f, 20.0f};
    m_front       = {0.0f, 0.0f, 1.0f};
    m_up          = {0.0f, 1.0f, 0.0f};
    m_right       = maths::normalize(maths::cross(m_front,m_up));
    yaw = 0.0f;
    pitch = 0.0f;
    roll = 0.0f;
    zoom = 20.0f;
    m_speed = 5.0f;
}

void Camera::processKeyboard(unsigned char key,float dt)
{
    float m_velocity = m_speed*dt;
    switch (key) {  

        case 's':
            m_pos = maths::add(m_pos,maths::mul(m_up,m_velocity));
            break;

        case 'w':
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
            if (zoom>200.0f)
                zoom = 200.0f;
            break;

        case 'x':
            zoom -= 10*dt;
            if (zoom<0.05f)
                zoom = 0.05f;
            break;
            
        case 'q':
            exit(0);
    }
}

void Camera::processMouse(int xoffset, int yoffset)
{
    float mouseSensitivity = 0.00001f;
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // pitch = x-axis , yaw = y-axis , roll = z-axis rotation
    if (xoffset > 0 && yoffset < 0){
        yaw -= xoffset;
        pitch += yoffset;
    }

    else if (xoffset <0 && yoffset >0){
        yaw += xoffset;
        pitch -= yoffset;
    }

    else{
        yaw   += xoffset;
        pitch += yoffset;
    }

    // maths::vec3f front;
    // front[0] = cos(maths::radians(yaw)) * cos(maths::radians(pitch));
    // front[1] = sin(maths::radians(pitch));
    // front[2] = sin(maths::radians(yaw)) * cos(maths::radians(pitch));
    m_front = maths::mul(maths::y_rotation(yaw),m_front);
    m_front = maths::mul(maths::x_rotation(pitch),m_front);
    m_front = maths::normalize(m_front);
    m_right = maths::normalize(maths::cross(m_front, m_up));  
    m_up = maths::normalize(maths::cross(m_right,m_front));
}


maths::mat4f Camera::getViewMatrix()
{
    maths::mat4f view;
    view = maths::lookAt(m_pos, maths::add(m_pos,m_front), m_up);
    return view;
}
