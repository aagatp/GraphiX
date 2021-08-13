#include <GL/glut.h>
#include "camera.h"

Camera::Camera()
{
    m_pos         = {1.0f, 1.0f, 20.0f};
    m_front       = {0.0f, 0.0f, -1.0f};
    m_up          = {0.0f, 1.0f, 0.0f};
    m_right       = maths::normalize(maths::cross(m_front,m_up));
    zoom = 30.0f;
    m_speed = 1.0f;
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
            zoom = zoom > 200.0f ? 200.0f : zoom+10*dt;
            break;

        case 'x':
            zoom = zoom < 0.05 ? 0.05 : zoom-10*dt;
            break;
            
        // case 'j':
        //     m_pos= maths::mul(maths::x_rotation(-0.05),m_pos);
        //     break;

        // case 'k':
        //     m_pos= maths::mul(maths::x_rotation(0.05),m_pos);
        //     break;

        // case 'h':
        //     m_pos= maths::mul(maths::y_rotation(-0.05),m_pos);
        //     break;

        // case 'l':
        //     m_pos= maths::mul(maths::y_rotation(0.05),m_pos);
        //     break;
        
        case 'q':
            exit(0);

    }
}

maths::mat4f Camera::getViewMatrix()
{
    maths::mat4f view;
    view = maths::lookAt(m_pos, maths::add(m_pos,m_front), m_up);
    return view;
}
