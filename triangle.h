#pragma once
#include "maths.h"
#include "canvas.h"
#include <vector>

struct Vertex{
    maths::vec3f normal;
    maths::vec3f position;
    maths::vec2f texCoords;
    maths::vec3f color;
    float intensity;
};

class Triangle{
public:
    Triangle(Canvas*);
    Triangle(Canvas*,maths::vec3f a, maths::vec3f b, maths::vec3f c);
    Triangle(Canvas*,maths::vec3f a, maths::vec3f b, maths::vec3f c, maths::vec3f mcolor);
    void setColor(maths::vec3f col);
    void setVertex(maths::vec3f a, maths::vec3f b, maths::vec3f c);
    void setNormals(maths::vec3f na, maths::vec3f nb, maths::vec3f nc);
    void setTexCoords(maths::vec2f, maths::vec2f, maths::vec2f);
    void flatRasterize();
    void gouraudRasterize();
    void populateVertices();
    void rasterize();
    void wireframe_draw();
    std::array <maths::vec3f,3> vertices;
    std::array <maths::vec3f,3> normals;
    std::array <maths::vec2f,3> texCoords;
    maths::vec3f intensities;
    maths::vec3f color;
    bool isGouraudShading;
    void setIntensity(maths::vec3f);
private:
    void fillBottomFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3);
    void fillTopFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3);
    Canvas* m_canvas;
    std::array<Vertex,3> vertex;
};