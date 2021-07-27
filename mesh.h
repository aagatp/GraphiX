#pragma once
#include "triangle.h"
#include <vector>
#include <string>
#include<iostream>
#include <fstream>
#include <sstream>
#include "maths.h"

class Mesh{
public:
    Mesh(Canvas*);
    void load(std::string);
    void draw();
    void rotate(float,float,float);
    void translate(float,float,float);
    void scale(float, float, float);
    void applyTransform(maths::mat4f&);
private:
    std::vector<Triangle> triangles;
    Canvas* canvas;
};