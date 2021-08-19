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
    void setIntensity(maths::vec3f);

    void wireframe_draw();
    void rasterize();

    std::array <maths::vec3f,3> vertices;
    std::array <maths::vec3f,3> normals;
    std::array <maths::vec2f,3> texCoords;
    maths::vec3f color;
    maths::vec3f intensities;
    std::array<Vertex,3> vertex;
    Canvas* m_canvas;
    void shadowRasterize();

private:
    void populateVertices();
    void drawFlatTriangle(Vertex &,Vertex &,Vertex &,Vertex &,Vertex &,Vertex);
    void fillBottomFlatTriangle(Vertex& v1,Vertex& v2, Vertex& v3);
    void fillTopFlatTriangle(Vertex& v1,Vertex& v2, Vertex& v3);
    bool isShadow=false;
};