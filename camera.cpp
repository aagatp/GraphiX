#include "camera.h"
#include <iostream>

Camera::Camera(maths::vec3f position , maths::vec3f up , float yaw , float pitch ) : Front(maths::vec3f{0.0f, 0.0f, -1.0f}), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(maths::vec3f{0.0f, 0.0f, -1.0f}), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{

    Position = maths::vec3f{posX, posY, posZ};
    WorldUp = maths::vec3f{upX, upY, upZ};
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

maths::mat4f Camera::getViewMatrix()
{
    return maths::lookAt(Position, maths::add(Position,Front), Up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    std::cout << deltaTime;
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position = maths::add(Position, maths::mul(Up,velocity));
    if (direction == BACKWARD)
        Position = maths::sub(Position, maths::mul(Up,velocity));
    if (direction == LEFT)
        Position = maths::sub(Position, maths::mul(Right,velocity));
    if (direction == RIGHT)
        Position = maths::add(Position, maths::mul(Right,velocity));
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   -= xoffset;
    Pitch -= yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f; 
}

void Camera::updateCameraVectors()
{
    maths::vec3f front;
    front[0] = cos(maths::radians(Yaw)) * cos(maths::radians(Pitch));
    front[1] = sin(maths::radians(Pitch));
    front[2] = sin(maths::radians(Yaw)) * cos(maths::radians(Pitch));
    Front = maths::normalize(front);

    Right = maths::normalize(maths::cross(Front, WorldUp));  
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = maths::normalize(maths::cross(Right, Front));
}
   