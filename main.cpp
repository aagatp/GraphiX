#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include "mesh.h"


//Global Setup
Canvas* canvas;
Camera* camera;
Mesh* mesh;
float deltaTime;


void processKeys(unsigned char key, int x, int y){
    if (key=='j')
        mesh->xrotate(0.05);
    else if (key=='k')
        mesh->xrotate(-0.05);
    else if (key=='h')
        mesh->yrotate(0.05);
    else if (key=='l')
        mesh->yrotate(-0.05);
    else if (key=='e')
        mesh->isWireframe = !mesh->isWireframe;
    else if (key=='g')
        mesh->isGouraudShade=!mesh->isGouraudShade;
    else
        camera->processKeyboard(key,deltaTime);
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
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    glutMainLoop();
}