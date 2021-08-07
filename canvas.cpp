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

    //This function is called for window resize. Also called when first window created
    glutReshapeFunc(Canvas::reshape);

}

void Canvas::reshape(int w, int h) {

    //Is called when window is created or resized
    auto oldWidth = scrWidth;
    auto oldHeight = scrHeight;
    scrWidth = w;
    scrHeight = h;

    //Load essentials for window creation and resizing
    glViewport(0, 0,scrWidth ,scrWidth);
    glLoadIdentity();
    gluOrtho2D(0.0,  scrWidth, 0.0,  scrHeight);

}

void Canvas::update(int val) {
    //The fps given below is maximum fps limit
    int maxfps = 60;

    glClear( GL_COLOR_BUFFER_BIT);
    // glClearColor(0,0,0,1);
    glutPostRedisplay();
    glutTimerFunc(1000 /maxfps, update, 0);
}

void Canvas::display() {
    
    //this function draws into the screen the pixel buffer stored
    glBegin(GL_POINTS);

    while (!buffers.empty()){
        Buffer tmp = buffers.back();
        int x = tmp.cords[0];
        int y = tmp.cords[1];
        maths::vec3f col = tmp.color;
        buffers.pop_back();
        std::cout << x << "\t" << y << "\n";
        glColor4f(col[0],col[1],col[2],1);
        glVertex2i(x, y);
    }
    glEnd();
    glutSwapBuffers();
    
}


void Canvas::putpixel(int x, int y,float zBuf, const maths::vec3f col) {
    
    //Stores the pixels in buffers to draw later in display func
    Buffer tmp;
    tmp.cords = {x,y};
    tmp.color = col;
    buffers.push_back(tmp);
}

void Canvas::drawline(int x1, int y1, int x2, int y2, const maths::vec3f color)
{
    // Bresenham line drawing algorithm
    
    int dx, dy;
    int steps, k;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    int lx, ly;

    if (x2 > x1) { lx = 1; }
    else { lx = -1; }

    if (y2 > y1) { ly = 1; }
    else { ly = -1; }

    int x = x1, y = y1;
    
    //slope is less than 1
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
    // slope is greater than equal to 1
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

