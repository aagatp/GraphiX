#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include "mesh.h"
#include "light.h"

//Global Setup
Canvas* canvas;
Camera* camera;
Mesh* mesh;
Light* light;
float deltaTime;

void processArrowKeys(int key, int x, int y){
    light->processKeyboard(key,deltaTime);
}

void processKeys(unsigned char key, int x, int y){
    camera->processKeyboard(key,deltaTime);
    mesh->processKeyboard(key,deltaTime);
    light->processKeyboard(key, deltaTime);
}

void renderer(){

    //Calculate deltatime and framePerSecond
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)/1000;
    lastFrame = currentFrame;

    maths::mat4f view = camera->getViewMatrix();
    maths::mat4f projection = maths::perspective(maths::radians(camera->zoom), (float)canvas->scrWidth/canvas->scrHeight);
    
    mesh->setView(view);
    mesh->setProjection(projection);

    
    mesh->render(); // engine pipeline lies here

    canvas->update();
    canvas->display();

}

int main(int argc, char** argv){
    
    //Canvas setup
    canvas = new Canvas(argc, argv);

    //Camera setup
    camera = new Camera();

    //Setup Lights
    light = new Light();

    //Creating mesh
    mesh=new Mesh(canvas,light);
    mesh->parse("../res/dharahara.obj");
    mesh->camera = camera;

    //Glut specific functions
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processArrowKeys);
    glutDisplayFunc(renderer);
    glutMainLoop();
}