#include "canvas.h"
#include <limits>

int Canvas::scrHeight = 1080;
int Canvas::scrWidth = 1920;

Canvas::Canvas(int argc,char **argv){

    //Initialize GLUT and create Windows
    glutInit(&argc, argv);
    glutInitWindowSize(scrWidth,scrHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Graphix");
    glutReshapeFunc(Canvas::reshape);
    
}
void Canvas::reshape(int w, int h) {
    auto oldWidth = scrWidth;
    auto oldHeight = scrHeight;
    scrWidth = w;
    scrHeight = h;

    glViewport(0, 0,  scrWidth,  scrHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,  scrWidth, 0.0,  scrHeight);
}

void Canvas::update(int value) {
    glClear( GL_COLOR_BUFFER_BIT);
    float fps = 60;
    glutPostRedisplay();
    glutTimerFunc(1000 /fps, update, 0);
}
void Canvas::display() {
    
    glBegin(GL_POINTS);
    while (!buffers.empty()){
        Buffer tmp = buffers.back();
        float x = tmp.cords[0];
        float y = tmp.cords[1];
        maths::vec3f col = tmp.color;
        buffers.pop_back();
        glColor3ub(col[0], col[1], col[2]);
        glVertex2f(x, y);
    }
    glEnd();
    glutSwapBuffers();

}

void Canvas::putpixel(float x, float y,float zBuf, const maths::vec3f col) {
    Buffer tmp;
    tmp.cords = {x,y};
    tmp.color = col;
    buffers.push_back(tmp);
}

void Canvas::drawline(float x1, float y1, float x2, float y2, const maths::vec3f color)
{
    float dx, dy;
    float steps, k;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    //Sets increment/decrement : stepsize
    float lx, ly;

    if (x2 > x1) { lx = 1; }
    else { lx = -1; }

    if (y2 > y1) { ly = 1; }
    else { ly = -1; }
    //initialize
    float x = x1, y = y1;
    //slope<1
    if (dx > dy) {
        float p = 2 * dy - dx;
        for (float k = 0; k <= dx; k++) {
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
        float p = 2 * dx - dy;
        for (float k = 0; k <= dy; k++) {
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

