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
    intensities = {1,1,1};
    populateVertices();
}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c){
    m_canvas = canvas;
    color = maths::normalize({220,220,220});
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
        vertex[i].color = color;
        vertex[i].position = vertices[i];
        vertex[i].normal = normals[i];
        vertex[i].texCoords = texCoords[i];
        vertex[i].intensity = intensities[i];
    }
}

void Triangle::gouraudRasterize(){
    // Sort the points so that y0 <= yl <= y2
    // auto P0 = vertex[0];
    // auto P1 = vertex[1];
    // auto P2 = vertex[2];
    
    // if (P1[1] < P0[1]) { std::swap(P1, P0); }
    // if (P2[1] < P0[1]) { std::swap(P2, P0);}
    // if (P2[1] < P1[1]) { std::swap(P2, P1);}

    // //  Compute the x coordinates of the triangle edges
    // std::vector<float> x01 = maths::interpolate(P0[1], P0[0], P1[1], P1[0]);
    // std::vector<float> h01 = maths::interpolate(P0[1], P0[3],  P1[1], P1[3]);

    // std::vector<float> x12 = maths::interpolate(P1[1], P1[0], P2[1], P2[0]);
    // std::vector<float> h12 = maths::interpolate(P1[1], P1[3], P2[1], P2[3]);

    // std::vector<float> x02 = maths::interpolate(P0[1], P0[0], P2[1], P2[0]);
    // std::vector<float> h02 = maths::interpolate(P0[1], P0[3], P2[1], P2[3]);

    // //  Concatenate the short sides
    // x01.pop_back();

    // std::vector<float> x012;
    // x012.resize(x01.size() + x12.size());
    // std::copy(x01.begin(), x01.end(), x012.begin());
    // std::copy(x12.begin(), x12.end(), x012.begin() + x01.size());

    // // std::cout<<"------------------------------------------------\n";
    // h01.pop_back();

    // std::vector<float> h012;
    // h012.resize(h01.size() + h12.size());
    // std::copy(h01.begin(), h01.end(), h012.begin());
    // std::copy(h12.begin(), h12.end(), h012.begin() + h01.size());

    // //  Determine which is left and which is right
    // int m = x012[x012.size()/2];
    // std::vector<float> x_left,x_right, h_left, h_right;

    // if (x02[m] < x012[m]) {
    //     x_left = x02;
    //     h_left = h02;

    //     x_right = x012;
    //     h_right = h012;
    // } 
    // else {
    //     x_left = x012;
    //     h_left = h012;

    //     x_right = x02;
    //     h_right = h02;
    // }
    // for (float y = P0[1];y<P2[1]-1;y++) {
    //     auto x_l = x_left[y- P0[1]];
    //     auto x_r = x_right[y-P0[1]];

    //     auto h_segment = maths::interpolate(x_l,h_left[y-P0[1]],x_r, h_right[y-P0[1]]);

    //     for (float x = x_l; x < x_r;x++) {
    //         maths::vec3f shadedColor = maths::mul(color,h_segment[x-x_l]);
    //         m_canvas->putpixel(x, y,1,shadedColor);
    //     }
    // }
}


void Triangle::rasterize(){
    // if (isGouraudShading){
    //     gouraudRasterize();
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

//     if(int(v2.position[1]) == int(v3.position[1])){fillBottomFlatTriangle(v1, v2, v3);}
//     else if(int(v1.position[1]) == int(v2.position[1])){fillTopFlatTriangle(v1, v2, v3);}
//     else
//     {
//         Vertex v4;
//         v4.position = {(v1.position[0] + ((float)(v2.position[1]-v1.position[1])/(float)(v3.position[1]-v1.position[1]))*(v3.position[0]-v1.position[0])),v2.position[1], 0};
        
//         v4.
                
//         fillBottomFlatTriangle(v1, v2, v4);
//         fillTopFlatTriangle(v2, v4, v3);
//     }
// }

// void Triangle::fillBottomFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3)
// {
//     float invslope1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
//     float invslope2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

//     float curx1 = v1[0];
//     float curx2 = v1[0];

//     for (int scanlineY = v1[1]; scanlineY < v2[1] - 0.5f; scanlineY++)
//     {
        
//         m_canvas->drawline(curx1,(float)scanlineY,curx2,(float)scanlineY, color);
//         curx1 += invslope1;
//         curx2 += invslope2;
//     }
// }

// void Triangle::fillTopFlatTriangle(maths::vec3f v1, maths::vec3f v2, maths::vec3f v3)
// {
//     float invslope1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
//     float invslope2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

//     float curx1 = v3[0];
//     float curx2 = v3[0];

//     for (int scanlineY = v3[1]; scanlineY > v1[1]; scanlineY--)
//     {
//         m_canvas->drawline(curx1,(float)scanlineY,curx2,(float)scanlineY, color);
//         curx1 -= invslope1;
//         curx2 -= invslope2;
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

