#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include <chrono>
#include "mesh.h"


//Global Setup
Canvas* canvas;
Camera* camera;
Mesh* mesh;

float deltaTime = 0;


void processKeys(unsigned char key, int x, int y){
    
    if (key == GLUT_KEY_END)
        glutDestroyWindow(0);
    if (key== 's')
        camera->processKeyboard(FORWARD, deltaTime);
    if (key== 'w')
        camera->processKeyboard(BACKWARD, deltaTime);
    if (key== 'd')
        camera->processKeyboard(LEFT, deltaTime);
    if (key== 'a')
        camera->processKeyboard(RIGHT, deltaTime);
    if (key=='z')
        camera->processKeyboard(ZOOMIN,deltaTime);
    if (key=='x')
        camera->processKeyboard(ZOOMOUT,deltaTime);
}

void processMouse(int xpos, int ypos){

    static float lastX = 800;
    static float lastY = 500;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xoffset, yoffset);
}

void renderer(){

    //Calculate deltatime and framePerSecond
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)/1000;
    lastFrame = currentFrame;

    maths::mat4f view = camera->getViewMatrix();
    maths::mat4f projection = maths::perspective();

    maths::mat4f view_projection = maths::mul(projection,view);
    mesh->applyTransform(view);
    mesh->draw();

    canvas->update();
    canvas->display();

}

int main(int argc, char** argv){
    
    //Canvas setup
    canvas = new Canvas(argc, argv);

    //Camera setup
    camera = new Camera(maths::vec3f{0.0f, 0.0f, 3.0f});

    //Creating mesh
    mesh=new Mesh(canvas);
    // mesh->load("../res/cube.obj");
    mesh->parse("../res/cubeUV.obj");
    mesh->camera = camera;
    // mesh->translate(1.0,1.0,1.0);
    mesh->scale(100.0,100.0,100.0);

    //Glut specific functions
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    glutPassiveMotionFunc(processMouse);
    glutMainLoop();

}