#pragma once
#include "triangle.h"
#include <vector>
#include <string>
#include<iostream>
#include <fstream>
#include <sstream>
#include "maths.h"
#include "camera.h"

class Mesh{
public:
    Mesh(Canvas*);
    void load(std::string);
    void parse(std::string);
    void draw();
    void rotate(float,float,float);
    void translate(float,float,float);
    void scale(float, float, float);
    void applyTransform(maths::mat4f&);
    bool backFaceCulling(Triangle&);
    float calculateIntensity(maths::vec3f point, maths::vec3f normal, maths::vec3f view);
    void flatShading(Triangle& tri);
    void gouraudShading(Triangle& tri);
    void phongShading(Triangle& tri);
    
    void update();
    void setView(maths::mat4f);
    void setProjection(maths::mat4f);

    Camera* camera;
private:
    std::vector<Triangle> triangles;
    Canvas* canvas;
    std::vector<Triangle> finalTris;
    maths::mat4f view;
    maths::mat4f projection;
};