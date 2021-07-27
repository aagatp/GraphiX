#include "triangle.h"
#include <algorithm>

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c){
    m_canvas = canvas;
    color = {255,0,0};
    vertices = {a,b,c};
}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c, maths::vec3f mcolor){
    m_canvas = canvas;
    color = {mcolor[0],mcolor[1],mcolor[2]};
    vertices = {a,b,c};
}

void Triangle::draw(){
    
    float x1 = vertices[0][0];
    float y1 = vertices[0][1];
    float x2 = vertices[1][0];
    float y2=  vertices[1][1];
    float x3 = vertices[2][0];
    float y3 = vertices[2][1];
    if (y1 == y2 && y2 == y3) return;
    //Bubble sort on y-position
    if (y1 > y2) { 
        // x1 and y1 swap x2 and y2 
        std::swap(x1,x2);
        std::swap(y1,y2);
    }
    if (y1 > y3) { 
        // x1 and y1 swap x3 and y3
        std::swap(x1,x3);
        std::swap(y1,y3);
    }
    if (y2 > y3) {
        // x3 and y3 swap x2 and y2
        std::swap(x2,x3);
        std::swap(y2,y3);
    }

    //divide triangle into two halves

    int height = y3 - y1;

    for (int y = y1; y <= y2; y++)
    {
        int partialHeight = y2 - y1 + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - y1) / height;// be careful with divisions by zero 
        if (partialHeight != 0)
        {
            float beta = (float)(y - y1) / partialHeight;
            int Ax = (x1 + (x3 - x1) * alpha), Ay = y1 + (y3 - y1) * alpha;
            int Bx = x1 + (x2 - x1) * beta, By = y1 + (y2 - y1) * beta;
            if (Ax > Bx) { 
                int tmp = Ax;
                Ax = Bx;
                Bx = tmp;
            }
            for (int j = Ax; j <= Bx; j++)
                m_canvas->putpixel(j, y,1, color);
        }

    }

    for (int y = y2; y <= y3; y++)
    {
        int partialHeight = y3 - y2 + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - y1) / height;
        if (partialHeight != 0)
        {
            float beta = (float)(y - y2) / partialHeight; // be careful with divisions by zero 

            int Ax = x1 + (x3 - x1) * alpha, Ay = y1 + (y3 - y1) * alpha;
            int Bx = x2 + (x3 - x2) * beta, By = y2 + (y3 - y2) * beta;
            if (Ax > Bx) { 
                int tmp = Ax;
                Ax = Bx;
                Bx = tmp;
            }
            for (int j = Ax; j <= Bx; j++)
                m_canvas->putpixel(j, y,1, color);
        }

    }
}

void Triangle::wireframe_draw(){
    
    m_canvas->drawline((int)vertices[0][0], (int)vertices[0][1], (int)vertices[1][0], (int)vertices[1][1],color);
	m_canvas->drawline((int)vertices[1][0], (int)vertices[1][1], (int)vertices[2][0], (int)vertices[2][1],color);
	m_canvas->drawline((int)vertices[2][0], (int)vertices[2][1], (int)vertices[0][0], (int)vertices[0][1],color);
}

