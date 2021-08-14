#pragma once
#include "maths.h"
#include "canvas.h"
#include <vector>
#include "vertex.h"

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
    void rasterize();
    void wireframe_draw();
    void populateVertices();
    std::array <maths::vec3f,3> vertices;
    std::array <maths::vec3f,3> normals;
    std::array <maths::vec2f,3> texCoords;
    maths::vec3f intensities;
    maths::vec3f color;
    bool isGouraudShading;
    void setIntensity(maths::vec3f);
    void drawFlatTriangle(Vertex &,Vertex &,Vertex &,Vertex &,Vertex &,Vertex);
    Canvas* m_canvas;
    std::array<Vertex,3> vertex;
private:
    void fillBottomFlatTriangle(Vertex& v1,Vertex& v2, Vertex& v3);
    void fillTopFlatTriangle(Vertex& v1,Vertex& v2, Vertex& v3);
};