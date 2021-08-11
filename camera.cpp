#include <GL/glut.h>
#include "camera.h"

Camera::Camera()
{
    m_pos         = {0.5f, 2.0f, 20.0f};
    m_front       = {0.0f, 0.0f, -1.0f};
    m_up          = {0.0f, 1.0f, 0.0f};
    m_right       = maths::normalize(maths::cross(m_front,m_up));
    yaw = -90.0f;
    pitch = 0.0f;
    roll = 0.0f;
    zoom = 20.0f;
    m_speed = 1.0f;
    lastX = 0;
    lastY = 0;
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
                zoom = 0.05;
            break;
            
        case 'q':
            exit(0);
    }
}

void Camera::processClicks(int button,int state, int xpos, int ypos){
    if (state==GLUT_DOWN)
    {
        lastX = xpos;
        lastY = ypos;
    }
    else if (state==GLUT_UP)
    {   
        lastX = 0;
        lastY = 0;
    }
}

void Camera::processMouse(int xpos, int ypos)
{   
    float factor = 1;

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY- ypos);

    float length = sqrt(pow(xoffset,2)+pow(yoffset,2));

    if (length!=0){
        yaw += xoffset/length*factor;
        pitch += yoffset/length*factor;
    }

    if(pitch > 89.0f)   
        pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;

    maths::vec3f m_front = maths::normalize({cosf(maths::radians(yaw))*cos(maths::radians(pitch)),
                                            sinf(maths::radians(pitch)),
                                            sinf(maths::radians(yaw))*cos(maths::radians(pitch))});

    // m_front = maths::mul(maths::y_rotation(yaw),maths::vec3f{0,0,-1});
    // m_front = maths::mul(maths::z_rotation(pitch),maths::vec3f{0,0,-1});

    m_front = maths::normalize(m_front);
    m_right = maths::normalize(maths::cross(m_front, m_up));  
    m_up = maths::normalize(maths::cross(m_right,m_front));

    lastX = xpos;
    lastY = ypos;
}


maths::mat4f Camera::getViewMatrix()
{
    maths::mat4f view;
    view = maths::lookAt(m_pos, maths::add(m_pos,m_front), m_up);
    return view;
}
