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
    void draw();
    void rotate(float,float,float);
    void translate(float,float,float);
    void scale(float, float, float);
    void applyTransform(maths::mat4f&);
    void backFaceCulling(Triangle&);
    float calculateIntensity(maths::vec3f point, maths::vec3f Normal, maths::vec3f View,float specularExp);
    void phongIlluminationModel(Triangle& tri);
    
    bool isbackfaceCulling = true;
    Camera* camera;
private:
    std::vector<Triangle> triangles;
    Canvas* canvas;
    // std::vector<Triangle> trisViewProject;
    
    std::vector<Triangle> finalTris;
};