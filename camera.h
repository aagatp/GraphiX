#pragma once

#include "maths.h"
#include <vector>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ZOOMIN,
    ZOOMOUT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  500.0f;
const float SENSITIVITY =  0.05f;
const float ZOOM        =  20.0f;


class Camera
{
public:
    // camera Attributes
    maths::vec3f Position;
    maths::vec3f Front;
    maths::vec3f Up;
    maths::vec3f Right;
    maths::vec3f WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(maths::vec3f position = maths::vec3f{0.0f, 0.0f, 0.0f}, maths::vec3f up = maths::vec3f{0.0f, 1.0f, 0.0f}, float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    maths::mat4f getViewMatrix();

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

