#pragma once
#include "maths.h"
#include <GL/glut.h>
#include <iostream>
#include <functional>

struct Buffer{
    maths::vec2i cords;
    maths::vec3f color;
};

class Canvas{
public:
    Canvas(int, char**);
    static void reshape(int,int);
    static void display();
    static void update(int);
    static void cleargrid();
    void putpixel(int x, int y,float zBuf, const maths::vec3f col);
    void drawline(int x1, int y1, int x2, int y2, const maths::vec3f color);
    static int scrHeight, scrWidth;
    static int fps;
    static bool* grid;
    static maths::vec3f* color;
    static float* zBuffer;

private:
    bool isPerspective;
};