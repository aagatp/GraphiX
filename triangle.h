#pragma once
#include "maths.h"
#include "canvas.h"

class Triangle{
public:
    Triangle(Canvas*,maths::vec3f a, maths::vec3f b, maths::vec3f c);
    Triangle(Canvas*,maths::vec3f a, maths::vec3f b, maths::vec3f c, maths::vec3f mcolor);
    void setColor(int col);
    void setVertex(maths::vec3f a, maths::vec3f b, maths::vec3f c);
    void draw();
    void wireframe_draw();
    std::array <maths::vec3f,3> vertices;

private:
    maths::vec3f color;
    Canvas* m_canvas;
    // maths::vec3f v1;
    // maths::vec3f v2;
    // maths::vec3f v3;
};