#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include <chrono>
#include "mesh.h"

int lastX = 800;
int lastY = 500;

//Global Setup
Canvas* canvas;
Camera* camera;
Mesh* mesh;
float deltaTime;


void processKeys(unsigned char key, int x, int y){
    camera->processKeyboard(key,deltaTime);
}

void processMouseMotion(int xpos, int ypos) {
    // int xoffset = lastX- xpos;
    // int yoffset = lastY - ypos; 
    // // reversed since y-coordinates go from bottom to top

    //     lastX = xpos;
    //     lastY = ypos;

    camera->processMouse(xpos, ypos);
 }

void renderer(){

    //Calculate deltatime and framePerSecond
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)/1000;
    lastFrame = currentFrame;

    maths::mat4f view = camera->getViewMatrix();
    maths::mat4f projection = maths::perspective(maths::radians(camera->zoom), (float)canvas->scrWidth/canvas->scrHeight);
    // maths::mat4f projection = maths::persproject(camera->m_pos);

    mesh->setView(view);
    mesh->setProjection(projection);
    mesh->render();

    canvas->update();
    canvas->display();

}

int main(int argc, char** argv){
    
    //Canvas setup
    canvas = new Canvas(argc, argv);

    //Camera setup
    camera = new Camera();

    //Creating mesh
    mesh=new Mesh(canvas);
    mesh->parse("../res/dharahara.obj");
    mesh->camera = camera;

    //Glut specific functions
    glutReshapeFunc(canvas->reshape);
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    glutMotionFunc(processMouseMotion);
    glutMainLoop();

}