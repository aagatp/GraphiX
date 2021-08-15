#pragma once
#include "triangle.h"
#include <vector>
#include <string>
#include<iostream>
#include <fstream>
#include <sstream>
#include "maths.h"
#include "camera.h"
#include "light.h"

class Mesh{
public:
    Mesh(Canvas*,Light*);
    void load(std::string);
    void parse(std::string);

    void xrotate(float);
    void yrotate(float);
    void zrotate(float);
    void translate(float,float,float);
    void scale(float, float, float);

    bool backFaceCulling(Triangle&);

    void flatShading(Triangle& tri);
    void gouraudShading(Triangle& tri);
    void phongShading(Triangle& tri);
    
    void setView(maths::mat4f);
    void setProjection(maths::mat4f);

    void processKeyboard(char, float);
    void render();

    Camera* camera;
    Canvas* canvas;
    Light* light;

private:
    std::vector<Triangle> triangles;
    std::vector<Triangle> finalTris;

    bool isWireframe;
    bool isGouraudShade;
    bool isTextured;

    maths::mat4f view;
    maths::mat4f projection;
};