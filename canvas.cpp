#include "canvas.h"
#include <limits>

int Canvas::scrHeight = 1080;
int Canvas::scrWidth = 1920;
int Canvas::fps = 60;
// bool* Canvas::grid = new bool[scrWidth * scrHeight];
// maths::vec3f* Canvas::color = new maths::vec3f[scrWidth * scrHeight];
// float* Canvas::zBuffer = new float[scrWidth * scrHeight];

Canvas::Canvas(int argc,char **argv){
    glutInit(&argc, argv);
    glutInitWindowSize(scrWidth,scrHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Dharahara");
    glutReshapeFunc(Canvas::reshape);
    // for (GLint x = 0; x < scrWidth * scrHeight; x++)
    // {
    //     grid[x] = false;
    //     color[x] = {0,0,0};
    //     zBuffer[x] = std::numeric_limits<float>::min();
    // }
}
void Canvas::reshape(int w, int h) {
    auto oldWidth = scrWidth;
    auto oldHeight = scrHeight;
    scrWidth = w;
    scrHeight = h;

    // bool* newGrid = new bool[scrWidth * scrHeight];
    // maths::vec3f* newcolor = new maths::vec3f[scrWidth * scrHeight];
    // float* newzBuffer = new float[scrWidth * scrHeight];
    // for (GLint x = 0; x <scrWidth ; ++x) {
    //     for (GLint y = 0; y < scrHeight; ++y) {
    //         if (x < oldWidth && x >= 0 && y < oldHeight && y >= 0) {
    //             newGrid[x + y * scrWidth] = grid[x + y * oldWidth];
    //             newcolor[x + y * scrWidth] = color[x + y * oldWidth];
    //             newzBuffer[x + y * scrWidth] = zBuffer[x + y * oldWidth];
    //         }
    //         else {
    //             newGrid[x + y *  scrWidth] = false;
    //             newcolor[x + y *  scrWidth] = {0};
    //             newzBuffer[x + y *  scrWidth] = std::numeric_limits<float>::min();
    //         }
    //     }
    // }

    // delete[] grid;
    // delete[] color;
    // delete[] zBuffer;
    // grid = newGrid;
    // color = newcolor;
    // zBuffer = newzBuffer;

    glViewport(0, 0,  scrWidth,  scrHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,  scrWidth, 0.0,  scrHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}
void Canvas::update(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000 /fps, update, 0);
}
void Canvas::display() {
    
    glBegin(GL_POINTS);
    // for (GLint x = 0; x < scrWidth; ++x) {
    //     for (GLint y = 0; y < scrHeight; ++y) {
    //         if (grid[x + y * scrWidth]) {
    //             maths::vec3f c = color[x + y * scrWidth];
    //             glColor3f(c[0], c[1], c[2]);
    //             glVertex2i(x, y);
    //         }
    //     }
    // }
    while (!buffers.empty()){
        Buffer tmp = buffers.back();
        int x = tmp.cords[0];
        int y = tmp.cords[1];
        maths::vec3f col = tmp.color;
        // std::cout << x << "\t" << y <<"\n";
        buffers.pop_back();
        glColor3f(col[0], col[1], col[2]);
        glVertex2i(x, y);
    }
    glEnd();
    glFlush();
    glutSwapBuffers();

}

void Canvas::cleargrid() {
    glClear( GL_COLOR_BUFFER_BIT);
    // bool* newgrid = new bool[scrWidth*scrHeight];
    // maths::vec3f* newcolor = new maths::vec3f[scrWidth*scrHeight]; 
    // float* newzbuf = new float[scrWidth*scrHeight]; 

    // for (GLint x = 0; x <  scrWidth *  scrHeight; ++x) {
    //     newgrid[x] = false;
    //     newcolor[x] = {0,0,0};
    //     newzbuf[x] = std::numeric_limits<float>::min();
    // }
    // delete[] grid;
    // delete[] color;
    // delete[] zBuffer;

    // grid = newgrid;
    // color = newcolor;
    // zBuffer = newzbuf;
    // glutPostRedisplay();

}

void Canvas::putpixel(int x, int y,float zBuf, const maths::vec3f col) {
    // scrWidth = (int)scrWidth;
    // if (x < scrWidth && x >= 0 && y < scrHeight && y >= 0) {
    //    if (zBuffer[x + y * scrWidth] <= zBuf)
    //     {
    //         // std::cout << x <<"\t" <<y << "\n";
    //        color[x + y * scrWidth] = col;
    //        grid[x + y * scrWidth] = true;
    //        zBuffer[x + y * scrWidth] = zBuf;
    //     } 

    // }
    // std::cout << x <<"\t" << y << "prn\n";
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

