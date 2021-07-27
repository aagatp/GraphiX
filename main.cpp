#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include <chrono>
#include "mesh.h"

float lastX = 1920/2;
float lastY = 1080/2;

Camera camera = Camera(maths::vec3f{0.0f, 0.0f, 3.0f});
bool firstMouse = true;
Canvas* canvas;
Mesh* mesh;


void processKeys(unsigned char key, int x, int y){
    std::cout << key<< " Hello Keys\n";
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
    
    maths::mat4f view = camera.getViewMatrix();
    maths::mat4f perspec = maths::perspective();

    maths::mat4f view_projection = maths::mul(perspec,view);
    mesh->applyTransform(view_projection);

    canvas->cleargrid();
    mesh->draw();
    canvas->display();
    canvas->update(0);
    canvas->cleargrid();

}

int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Dharahara");
    canvas = new Canvas();
    mesh=new Mesh(canvas);
    mesh->load("../res/videoship.obj");
    mesh->translate(10.0,10.0,10.0);
    mesh->scale(50.0,50.0,50.0);
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    // glutPassiveMotionFunc(processMouse);
    glutMainLoop();

}