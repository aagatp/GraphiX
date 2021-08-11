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
    camera->processKeyboard(key,deltaTime);
}

void processMouseMotion(int xpos, int ypos) {
    camera->processMouse(xpos, ypos);
}

void processMouseClick(int button,int state, int xpos, int ypos){
    camera->processClicks(button,state,xpos,ypos);
}

void processRotation(int key, int x, int y){
    if (GLUT_KEY_LEFT){
        std::cout << "left";
        mesh->xrotate(0.05);
    }
    if (GLUT_KEY_RIGHT){
        std::cout << "right";
        mesh->xrotate(-0.05);
    }
    if (GLUT_KEY_UP){
        std::cout << "up";
        mesh->yrotate(0.05);
    }
    if (GLUT_KEY_DOWN){
        std::cout << "down";
        mesh->yrotate(-0.05);
    }
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
    mesh->parse("../res/dharaharascene.obj");
    mesh->camera = camera;

    //Glut specific functions
    glutReshapeFunc(canvas->reshape);
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processRotation);
    glutMotionFunc(processMouseMotion);
    glutMouseFunc(processMouseClick);
    glutMainLoop();

}