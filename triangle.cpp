#include "triangle.h"
#include <algorithm>
#include <limits>

Triangle::Triangle(Canvas* canvas){
    m_canvas = canvas;
    color = {200,200,200};
    vertices = {{
        {0,0,0},
        {0,0,0},
        {0,0,0}
    }};
    intensities = {1,1,1};
    populateVertices();
}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c){
    m_canvas = canvas;
    color = {200,200,200};
    vertices = {a,b,c};
    intensities = {1,1,1};
    populateVertices();
}

void Triangle::setVertex(maths::vec3f a, maths::vec3f b, maths::vec3f c){
    vertices = {a,b,c};
    populateVertices();
}

void Triangle::setNormals(maths::vec3f na, maths::vec3f nb, maths::vec3f nc){
    normals = {na,nb,nc};
    populateVertices();
}

void Triangle::setTexCoords(maths::vec2f uv1,maths::vec2f uv2,maths::vec2f uv3){
    texCoords = {uv1,uv2,uv3};
    populateVertices();
}

void Triangle::setIntensity(maths::vec3f intense){
    intensities = intense;
    populateVertices();
}

void Triangle::populateVertices(){
    for (int i=0; i<3; i++){
        vertex[i].color = maths::mul(color,intensities[i]);
        vertex[i].position = vertices[i];
        vertex[i].normal = normals[i];
        vertex[i].texCoords = texCoords[i];
    }
}

void Triangle::rasterize(){
    flatRasterize();
    // baryRasterize();
}

void Triangle::flatRasterize()
{
    auto v1 = vertex[0];
    auto v2 = vertex[1];
    auto v3 = vertex[2];

    if(v1.position[1] > v2.position[1]) {std::swap(v2, v1);}
    if(v1.position[1] > v3.position[1]) {std::swap(v3, v1);}
    if(v2.position[1] > v3.position[1]) {std::swap(v3, v2);}

    if(v2.position[1] == v3.position[1]){
        if (v2.position[0]>v3.position[0])
            std::swap(v2,v3);
        fillBottomFlatTriangle(v1, v2, v3);
    }
    else if(v1.position[1] == v2.position[1]){
        if (v2.position[0]<v1.position[0])
            std::swap(v1,v2);
        fillTopFlatTriangle(v1, v2, v3);
    }
    else
    {   
        float split = (v2.position[1] - v1.position[1]) / (v3.position[1] - v1.position[1]);
        Vertex v4 = v1 + (v3-v1)*split;

        if (v2.position[0]<v4.position[0]){
            fillBottomFlatTriangle(v1, v2, v4);
            fillTopFlatTriangle(v2, v4, v3);
        }
        else{
            fillBottomFlatTriangle(v1, v4, v2);
            fillTopFlatTriangle(v4, v2, v3);
        }
    }
}

void Triangle::fillBottomFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
{   
    float delta = v3.position[1] - v1.position[1]; //v3.y = v2.y so anything can be used
    
    auto dt1 = (v2 - v1) / delta; 
    auto dt2 = (v3 - v1) / delta;
    
    auto e2 = v1;

    drawFlatTriangle(v1,v2,v3,dt1,dt2,e2);
}

void Triangle::fillTopFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
{
    float delta = v3.position[1] - v1.position[1]; //v1.y = v2.y so anything can be used
    
    auto dt1 = (v3 - v1) / delta; 
    auto dt2 = (v3 - v2) / delta;
    
    auto e2 = v2;

    drawFlatTriangle(v1,v2,v3,dt1,dt2,e2);
    
}

void Triangle::drawFlatTriangle(Vertex& v1, Vertex& v2, Vertex&v3, Vertex& d1, Vertex& d2, Vertex e2){
    auto e1 = v1;

    const int yStart = (int)ceil(v1.position[1] - 0.5f);
    const int yEnd = (int)ceil(v3.position[1] -   0.5f);
    e1 += d1 * (float(yStart) + 0.5f - v1.position[1]);
    e2 += d2 * (float(yStart) + 0.5f - v1.position[1]);

    for (int y = yStart; y < yEnd; y++, e1 += d1, e2 += d2)
    {
        const int xStart = (int)ceil(e1.position[0]-0.5f);
        const int xEnd = (int)ceil(e2.position[0]-0.5f); // the pixel AFTER the last pixel drawn
        
        auto iLine = e1;
        const float dx = e2.position[0] - e1.position[0];
        const auto diLine = (e2 - iLine) / dx;
        iLine += diLine * (float(xStart)+0.5f- e1.position[0]);
        for (int x = xStart; x < xEnd; x++, iLine += diLine)
        {
            maths::vec3f color = {iLine.color[0], iLine.color[1], iLine.color[2]};
            m_canvas->putpixel(x, y,1, color);
        }
    }
}

void Triangle::wireframe_draw(){
    
    m_canvas->drawline(vertices[0][0],vertices[0][1],vertices[1][0],vertices[1][1],color);
	m_canvas->drawline(vertices[1][0],vertices[1][1],vertices[2][0],vertices[2][1],color);
	m_canvas->drawline(vertices[2][0],vertices[2][1],vertices[0][0],vertices[0][1],color);
}

