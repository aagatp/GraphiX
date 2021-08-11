#include "triangle.h"
#include <algorithm>

Triangle::Triangle(Canvas* canvas){
    m_canvas = canvas;
    color = maths::normalize({50,50,50});
    // color = maths::normalize({220,220,220});
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
    color = maths::normalize({50,50,50});
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
    // if (isGouraudShading){
        // gouraudRasterize();
    // }
    // else{
        flatRasterize();
    // }
}

// void Triangle::flatRasterize()
// {
//     auto v1 = vertex[0];
//     auto v2 = vertex[1];
//     auto v3 = vertex[2];

//     if(v1.position[1] > v2.position[1]) {std::swap(v2, v1);}
//     if(v1.position[1] > v3.position[1]) {std::swap(v3, v1);}
//     if(v2.position[1] > v3.position[1]) {std::swap(v3, v2);}

//     if(v2.position[1] == v3.position[1]){

//         if (v3.position[0] < v2.position[0])
//             std::swap(v3,v2);
//         fillBottomFlatTriangle(v1, v2, v3);
//     }
//     else if(v1.position[1] == v2.position[1]){
//         if (v2.position[0] < v1.position[0])
//             std::swap(v2,v1);
//         fillTopFlatTriangle(v1, v2, v3);
//     }
//     else
//     {

//         const float split = (v2.position[1]-v1.position[1])/(v3.position[1]-v1.position[1]);
//         auto v4 = v1 + (v3-v1)*split; // interpolation

//         if (v2.position[0]< v4.position[0]){
//             fillBottomFlatTriangle(v1, v2, v4);
//             fillTopFlatTriangle(v2, v4, v3);
//         }
//         else{
//             fillBottomFlatTriangle(v1, v4, v2);
//             fillTopFlatTriangle(v4, v2, v3);
//         }
//     }
// }

// void Triangle::fillBottomFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
// {
//         const float delta_y = v3.position[1] - v1.position[1];
//     const auto dit0 = (v3 - v1) / delta_y;
//     const auto dit1 = (v3 - v2) / delta_y;
//     // create right edge interpolant
//     auto itEdge1 = v2;
//     // call the flat triangle render routine
//     drawFlatTriangle(v1, v2, v3, dit0, dit1, itEdge1);
// }

// void Triangle::fillTopFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
// {
//     const float delta_y = v3.position[1] - v1.position[1];
//     const auto dit0 = (v2 - v1) / delta_y;
//     const auto dit1 = (v3 - v1) / delta_y;
//     // create right edge interpolant
//     auto itEdge1 = v1;
//     // call the flat triangle render routine
//     drawFlatTriangle(v1, v2, v3, dit0, dit1, itEdge1);
// }

// void Triangle::drawFlatTriangle(const Vertex &it0,
//                       const Vertex &it1,
//                       const Vertex &it2,
//                       const Vertex &dv0,
//                       const Vertex &dv1,
//                       Vertex itEdge1)
// {
//     // create edge interpolant for left edge (always v0)
//     auto itEdge0 = it0;
//     // calculate start and end scanlines
//     const int yStart = (int)ceil(it0.position[1] - 0.5f);
//     const int yEnd = (int)ceil(it2.position[1] - 0.5f); // the scanline AFTER the last line drawn
//     // do interpolant prestep
//     itEdge0 += dv0 * (float(yStart) + 0.5f - it0.position[1]);
//     itEdge1 += dv1 * (float(yStart) + 0.5f - it0.position[1]);
//     for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
//     {
//         // calculate start and end pixels
//         const int xStart = (int)ceil(itEdge0.position[0] - 0.5f);
//         const int xEnd = (int)ceil(itEdge1.position[0] - 0.5f); // the pixel AFTER the last pixel drawn
//         // create scanline interpolant startpoint
//         // (some waste for interpolating x,y,z, but makes life easier not having
//         //  to split them off, and z will be needed in the future anyways...)
//         auto iLine = itEdge0;
//         // calculate delta scanline interpolant / dx
//         const float dx = itEdge1.position[0] - itEdge0.position[0];
//         const auto diLine = (itEdge1 - iLine) / dx;
//         // prestep scanline interpolant
//         iLine += diLine * (float(xStart) + 0.5f - itEdge0.position[0]);
//         for (int x = xStart; x < xEnd; x++, iLine += diLine)
//         {
//             // invoke pixel shader and write resulting color value
//             maths::vec3f color = {iLine.color[0], iLine.color[1], iLine.color[2]};
//             m_canvas->putpixel(x, y,1, color);
            
//         }
//     }
// }

void Triangle::flatRasterize()
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
        
        m_canvas->drawline(curx1,(float)scanlineY,curx2,(float)scanlineY, color);
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
        m_canvas->drawline(curx1,(float)scanlineY,curx2,(float)scanlineY, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void Triangle::wireframe_draw(){
    
    m_canvas->drawline(vertices[0][0],vertices[0][1],vertices[1][0],vertices[1][1],color);
	m_canvas->drawline(vertices[1][0],vertices[1][1],vertices[2][0],vertices[2][1],color);
	m_canvas->drawline(vertices[2][0],vertices[2][1],vertices[0][0],vertices[0][1],color);
}

