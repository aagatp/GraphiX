#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include <chrono>
#include "mesh.h"

float lastX = 800;
float lastY = 500;

Camera camera = Camera(maths::vec3f{0.0f, 0.0f, 0.0f});
bool firstMouse = true;
Canvas* canvas;
Mesh* mesh;


void processKeys(unsigned char key, int x, int y){
    static float lastFrame = 0;
    static float deltaTime = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)/1000;
    lastFrame = currentFrame;
    if (key == GLUT_KEY_END)
        glutDestroyWindow(0);
    if (key== 's')
        camera.processKeyboard(FORWARD, deltaTime);
    if (key== 'w')
        camera.processKeyboard(BACKWARD, deltaTime);
    if (key== 'd')
        camera.processKeyboard(LEFT, deltaTime);
    if (key== 'a')
        camera.processKeyboard(RIGHT, deltaTime);
    
}

// void processClick(int button, int state, int x, int y){

// }

void processMouse(int xpos, int ypos){
    // std::cout << xpos << "\t" << ypos << "\n";
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void renderer(){
    
    canvas->cleargrid();

    maths::mat4f view = camera.getViewMatrix();
    maths::mat4f projection = maths::perspective();

    maths::mat4f view_projection = maths::mul(projection,view);
    mesh->applyTransform(view);
    mesh->draw();

    canvas->display();
    canvas->update(0);

}

int main(int argc, char** argv){
    
    canvas = new Canvas(argc, argv);
    mesh=new Mesh(canvas);
    mesh->load("../res/videoship.obj");
    mesh->camera = &camera;
    mesh->translate(2.0,2.0,2.0);
    mesh->scale(100.0,100.0,100.0);
    // mesh->rotate(0,maths::radians(30),0);
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    glutPassiveMotionFunc(processMouse);
    glutMainLoop();

}