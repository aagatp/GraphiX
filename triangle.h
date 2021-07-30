#pragma once
#include "maths.h"
#include "canvas.h"

class Triangle{
public:
    Triangle(Canvas*,maths::vec3f a, maths::vec3f b, maths::vec3f c);
    Triangle(Canvas*,maths::vec3f a, maths::vec3f b, maths::vec3f c, maths::vec3f mcolor);
    void setColor(maths::vec3f col);
    void setVertex(maths::vec3f a, maths::vec3f b, maths::vec3f c);
    void rasterize();
    void wireframe_draw();
    std::array <maths::vec3f,3> vertices;

    maths::vec3f color;
private:
    void fillBottomFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3);
    void fillTopFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3);

    Canvas* m_canvas;
};