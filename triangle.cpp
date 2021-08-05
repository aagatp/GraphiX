#include "triangle.h"
#include <algorithm>

Triangle::Triangle(Canvas* canvas){
    m_canvas = canvas;
    color = maths::normalize({220,220,220});
    vertices = {{
        {0,0,0},
        {0,0,0},
        {0,0,0}
    }};
}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c){
    m_canvas = canvas;
    color = maths::normalize({220,220,220});
    vertices = {a,b,c};
}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c, maths::vec3f mcolor){
    m_canvas = canvas;
    color = {mcolor[0],mcolor[1],mcolor[2]};
    vertices = {a,b,c};
}

void Triangle::setVertex(maths::vec3f a, maths::vec3f b, maths::vec3f c){
    vertices = {a,b,c};
    //maths::printvec(a);
}

void Triangle::setNormals(maths::vec3f na, maths::vec3f nb, maths::vec3f nc){
    // std::cout << "Hello Normal:\n";
    normals = {na,nb,nc};
}

void Triangle::setTexCoords(maths::vec2f uv1,maths::vec2f uv2,maths::vec2f uv3){
    // std::cout << "Hello Texture:\n";
    texCoords = {uv1,uv2,uv3};
}

void Triangle::rasterize()
{
    
    maths::vec3f v1 = vertices[0];
    maths::vec3f v2 = vertices[1];
    maths::vec3f v3 = vertices[2];


    if(v1[1] > v2[1]) {std::swap(v2, v1);}
    if(v1[1] > v3[1]) {std::swap(v3, v1);}
    if(v2[1] > v3[1]) {std::swap(v3, v2);}

    if(int(v2[1]) == int(v3[1])){fillBottomFlatTriangle(v1, v2, v3);}
    else if(int(v1[1]) == int(v2[1])){fillTopFlatTriangle(v1, v2, v3);}
    else
    {
        
        maths::vec3f v4 = {(v1[0] + ((float)(v2[1]-v1[1])/(float)(v3[1]-v1[1]))*(v3[0]-v1[0])),v2[1], 0};
        fillBottomFlatTriangle(v1, v2, v4);
        fillTopFlatTriangle(v2, v4, v3);
    }
}

void Triangle::fillBottomFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3)
{
    float invslope1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
    float invslope2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

    float curx1 = v1[0];
    float curx2 = v1[0];

    for (int scanlineY = v1[1]; scanlineY < v2[1] - 0.5f; scanlineY++)
    {
        
        m_canvas->drawline(curx1, scanlineY, curx2, scanlineY, color);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void Triangle::fillTopFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3)
{
    float invslope1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
    float invslope2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

    float curx1 = v3[0];
    float curx2 = v3[0];

    for (int scanlineY = v3[1]; scanlineY > v1[1]; scanlineY--)
    {
        m_canvas->drawline(curx1, scanlineY, curx2, scanlineY, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void Triangle::wireframe_draw(){
    
    m_canvas->drawline((int)vertices[0][0], (int)vertices[0][1], (int)vertices[1][0], (int)vertices[1][1],color);
	m_canvas->drawline((int)vertices[1][0], (int)vertices[1][1], (int)vertices[2][0], (int)vertices[2][1],color);
	m_canvas->drawline((int)vertices[2][0], (int)vertices[2][1], (int)vertices[0][0], (int)vertices[0][1],color);
}

