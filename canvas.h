#pragma once
#include "maths.h"
#include <GL/glut.h>
#include <iostream>
#include <functional>
#include <vector>

struct Buffer{
    maths::vec2f cords;
    maths::vec3f color;
};

class Canvas{
public:
    Canvas(int, char**);
    static void reshape(int,int);
    void display();
    static void update(int fps=30);
    void putpixel(float x, float y,float zBuf, const maths::vec3f col);
    void drawline(float x1, float y1, float x2, float y2, const maths::vec3f color);
    static int scrHeight, scrWidth;
    std::vector<Buffer> buffers;
};