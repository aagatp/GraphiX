#include "canvas.h"

int Canvas::scrHeight = 1080;
int Canvas::scrWidth = 1920;
// float* Canvas::zbuffers= new float[scrHeight*scrWidth];

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
    for (auto it=buffermaps.begin(); it!=buffermaps.end(); ++it){
        Buffer tmp = it->second;
        glColor3ub(tmp.color[0],tmp.color[1],tmp.color[2]);
        glVertex2i(tmp.cords[0],tmp.cords[1]);
    }
    glEnd();
    glutSwapBuffers();
    buffermaps.clear();
}

void Canvas::putpixel(float x, float y,float zBuf, const maths::vec3f col) {
    
    
    // Clipping
    if (x>0 && x<scrWidth && y>0 && y<scrWidth){

        //Depth Test
        auto tmp = buffermaps.find((int)x+(int)y*scrWidth);
        if (tmp!= buffermaps.end()){
            if (zBuf > buffermaps.at((int)x+(int)y*scrWidth).zBuffer){
                Buffer tmp;
                tmp.cords = {(int)x,(int)y};
                tmp.color = col;
                tmp.zBuffer = zBuf;
                buffermaps[(int)x+(int)y*scrWidth] = tmp;
            }
        }
        else{
            Buffer tmp;
            tmp.cords = {(int)x,(int)y};
            tmp.color = col;
            tmp.zBuffer = zBuf;
            buffermaps[(int)x+(int)y*scrWidth] = tmp;
        }
    }
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

