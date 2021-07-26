// #pragma once

// #include "maths.h"
// #include <vector>

// // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
// enum Camera_Movement {
//     FORWARD,
//     BACKWARD,
//     LEFT,
//     RIGHT
// };

// // Default camera values
// const float YAW         = -90.0f;
// const float PITCH       =  0.0f;
// const float SPEED       =  2.5f;
// const float SENSITIVITY =  0.1f;
// const float ZOOM        =  45.0f;


// class Camera
// {
// public:
//     // camera Attributes
//     maths::vec3f Position;
//     maths::vec3f Front;
//     maths::vec3f Up;
//     maths::vec3f Right;
//     maths::vec3f WorldUp;
//     // euler Angles
//     float Yaw;
//     float Pitch;
//     // camera options
//     float MovementSpeed;
//     float MouseSensitivity;
//     float Zoom;
//     Camera(maths::vec3f position = maths::vec3f{0.0f, 0.0f, 0.0f}, maths::vec3f up = maths::vec3f{0.0f, 1.0f, 0.0f}, float yaw = YAW, float pitch = PITCH);
//     // constructor with vectors
//     // constructor with scalar values
//     Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

//     // returns the view matrix calculated using Euler Angles and the LookAt Matrix
//     maths::mat4f GetViewMatrix();

//     // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
//     void ProcessKeyboard(Camera_Movement direction, float deltaTime);

//     // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
//     void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
   
//     // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
//     void ProcessMouseScroll(float yoffset);

// private:
//     // calculates the front vector from the Camera's (updated) Euler Angles
//     void updateCameraVectors();
// };

