#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include "mesh.h"
#include "light.h"
#include "utils.h"
#include <thread>

//Global Setup
Canvas* canvas;
Camera* camera;
Mesh* mesh;
Light* light;
float deltaTime;
maths::mat4f view;
maths::mat4f projection;
maths::mat4f viewport;
maths::mat4f world_to_screennorm;
maths::mat4f screennorm_to_device;
bool isTransform=false;
bool isFirstRender=true;

void processArrowKeys(int key, int x, int y){
    light->processKeyboard(key,deltaTime);
    isTransform = true;
}

void processKeys(unsigned char key, int x, int y){

    camera->processKeyboard(key,deltaTime);
    mesh->processKeyboard(key,deltaTime);
    light->processKeyboard(key, deltaTime);

    projection = maths::perspective(maths::radians(camera->zoom), (float)canvas->scrWidth/canvas->scrHeight);
    view = camera->getViewMatrix();
    world_to_screennorm = maths::mul(projection,view);
    screennorm_to_device = maths::mul(viewport,world_to_screennorm);
    mesh->setTransform(screennorm_to_device);
    isTransform = true;
}

void renderer(){

    //Calculate deltatime and framePerSecond
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)/1000;
    lastFrame = currentFrame;
    
    std::cout << 1/deltaTime <<"\n";

    if (isFirstRender){

        view = camera->getViewMatrix();
        projection=maths::perspective(maths::radians(camera->zoom), (float)canvas->scrWidth/canvas->scrHeight);
        viewport = maths::mul(maths::scale(0.5*canvas->scrWidth,0.5*canvas->scrHeight,1.0),maths::translate(1.0,1.0,0.0));

        world_to_screennorm = maths::mul(projection,view);
        screennorm_to_device = maths::mul(viewport,world_to_screennorm);
        mesh->setTransform(screennorm_to_device);

        mesh->render();
        canvas->update();
        canvas->display();

        isFirstRender = false;
    }
    if (isTransform){

        canvas->cleargrid();
        mesh->render(); // engine pipeline lies here
        canvas->update();
        canvas->display();
    }

}

int main(int argc, char** argv){
    
    //Canvas setup
    canvas = new Canvas(argc, argv);

    //Camera setup
    camera = new Camera();

    //Setup Lights
    light = new Light();

    //Creating mesh
    mesh=new Mesh(canvas);
    mesh->light = light;
    mesh->camera = camera;
    mesh->parse("../res/lowpoly.obj");

    //Glut specific functions
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processArrowKeys);
    glutDisplayFunc(renderer);
    glutMainLoop();
}