#include "canvas.h"
#include <limits>

int Canvas::scrHeight = 1080;
int Canvas::scrWidth = 1920;

Canvas::Canvas(int argc,char **argv){

    //Initialize Windows

    glutInit(&argc, argv);
    glutInitWindowSize(scrWidth,scrHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Dharahara");
    glutReshapeFunc(Canvas::reshape);

}

void Canvas::reshape(int w, int h) {

    //Reshapes the windows and viewport
    auto oldWidth = scrWidth;
    auto oldHeight = scrHeight;
    scrWidth = w;
    scrHeight = h;

    glViewport(0, 0,  scrWidth,  scrHeight);
    glLoadIdentity();
    gluOrtho2D(0.0,  scrWidth, 0.0,  scrHeight);

}

void Canvas::update(int value) {
    int fps = 60;

    glClear( GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
    glutTimerFunc(1000 /fps, update, 0);
}

void Canvas::display() {
    
    glBegin(GL_POINTS);
    glLoadIdentity();

    while (!buffers.empty()){
        Buffer tmp = buffers.back();
        int x = tmp.cords[0];
        int y = tmp.cords[1];
        maths::vec3f col = tmp.color;
        buffers.pop_back();
        
        glColor4f(col[0],col[1],col[2],1);
        glVertex2i(x, y);
    }
    glEnd();
    glFlush();
    glutSwapBuffers();
}


void Canvas::putpixel(int x, int y,float zBuf, const maths::vec3f col) {
    
    Buffer tmp;
    tmp.cords = {x,y};
    tmp.color = col;
    buffers.push_back(tmp);
}

void Canvas::drawline(int x1, int y1, int x2, int y2, const maths::vec3f color)
{

    int dx, dy;
    int steps, k;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    //Sets increment/decrement : stepsize
    int lx, ly;

    if (x2 > x1) { lx = 1; }
    else { lx = -1; }

    if (y2 > y1) { ly = 1; }
    else { ly = -1; }
    //initialize
    int x = x1, y = y1;
    //slope<1
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int k = 0; k <= dx; k++) {
            putpixel(x, y,1, color);
            if (p < 0) {
                x += lx;
                p += 2 * dy;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    //if slope>=1
    else {
        int p = 2 * dx - dy;
        for (int k = 0; k <= dy; k++) {
            putpixel(x, y,1, color);
            if (p < 0) {
                y += ly;
                p += 2 * dx;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    putpixel(x, y,1, color);
}

