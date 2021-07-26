#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include <chrono>
#include "mesh.h"

// float lastX = 1920/2;
// float lastY = 1080/2;
// Camera camera = Camera(maths::vec3f{0.0f, 0.0f, 3.0f});
// bool firstMouse = true;

void mouseCallback(float xpos, float ypos)
{
    // if (firstMouse)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    // lastX = xpos;
    // lastY = ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(float xoffset, float yoffset)
{
    // camera.ProcessMouseScroll(yoffset);
}

void processEvents(float deltaTime){
// {GLFWwindow* window, 
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         camera.ProcessKeyboard(FORWARD, deltaTime);

//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         camera.ProcessKeyboard(BACKWARD, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         camera.ProcessKeyboard(LEFT, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main(int argc, char** argv){
    Canvas canvas(argc,argv);
    Mesh mesh(&canvas);
    mesh.load("../videoship.obj");
    mesh.translate(10.0,10.0,10.0);
    mesh.scale(50.0,50.0,50.0);
    mesh.draw();
    canvas.update(0);
    glutMainLoop();
}