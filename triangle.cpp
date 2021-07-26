#include "triangle.h"

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
    
}

void Triangle::wireframe_draw(){
    
    m_canvas->drawline((int)vertices[0][0], (int)vertices[0][1], (int)vertices[1][0], (int)vertices[1][1],color);
	m_canvas->drawline((int)vertices[1][0], (int)vertices[1][1], (int)vertices[2][0], (int)vertices[2][1],color);
	m_canvas->drawline((int)vertices[2][0], (int)vertices[2][1], (int)vertices[0][0], (int)vertices[0][1],color);
}

