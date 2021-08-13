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


void Triangle::shadeLine(float x1, float x2, float y){
    auto v1 = vertex[0].position;
    auto v2 = vertex[1].position;
    auto v3 = vertex[2].position;
    
    if (x2 < x1)
		std::swap(x1, x2);	
	for (float i = x1; i <= x2; i++) {
		//Barycentric coordinates
        maths::vec3f vt = {i,y,1};  
        maths::vec3f bary = maths::barycentric(v1,v2,v3,vt);

        float clrtmp = bary[0]*intensities[0]+bary[1]*intensities[1]+bary[2]*intensities[2];
        maths::vec3f col = maths::mul(color,clrtmp);
		m_canvas->putpixel(i, y,1, col);
	}
}

void Triangle::baryRasterize(){
    auto v1 = vertex[0].position;
    auto v2 = vertex[1].position;
    auto v3 = vertex[2].position;
    
    if(v1[1] > v2[1]) {std::swap(v2, v1);}
    if(v1[1] > v3[1]) {std::swap(v3, v1);}
    if(v2[1] > v3[1]) {std::swap(v3, v2);}

    float dx1, dx2, dx3;

    if (v2[1] - v1[1] > 0) dx1 = (v2[0] - v1[0]) / (v2[0] - v1[1]); else dx1 = 0;
	if (v3[1] - v1[1] > 0) dx2 = (v3[0] - v1[0]) / (v3[0] - v1[1]); else dx2 = 0;
	if (v3[1] - v2[1] > 0) dx3 = (v3[0] - v2[0]) / (v3[0] - v2[1]); else dx3 = 0;

    maths::vec3f source, end;

    source = v1, end = v1;
    if (dx1 > dx2){
        for (; source[1] <= v2[1]; source[1]++, end[1]++, source[0] += dx2, end[0] += dx1)
			shadeLine(source[0], end[0], source[1]);
		end = v2;
		for (; source[1] <= v3[1]; source[1]++, end[1]++, source[0] += dx2, end[0] += dx3)
			shadeLine(source[0], end[0], source[1]);
	}
	else {
		for (;source[1] <= v2[1]; source[1]++, end[1]++, source[0] += dx1, end[0] += dx2)
			shadeLine(source[0], end[0], source[1]);
        source = v2;
		for (; source[1] <= v3[1]; source[1]++, end[1]++, source[0] += dx3, end[0] += dx2)
			shadeLine(source[0], end[0], source[1]);
	}
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

// void Triangle::flatRasterize()
// {
//     auto v1 = vertex[0];
//     auto v2 = vertex[1];
//     auto v3 = vertex[2];

//     if(v1.position[1] > v2.position[1]) {std::swap(v2, v1);}
//     if(v1.position[1] > v3.position[1]) {std::swap(v3, v1);}
//     if(v2.position[1] > v3.position[1]) {std::swap(v3, v2);}

//     if(v2.position[1] == v3.position[1]){
//         if (v2.position[0]<v1.position[0])
//             std::swap(v1,v2);
//         fillBottomFlatTriangle(v1, v2, v3);
//     }
//     else if(v1.position[1] == v2.position[1]){
//         if (v3.position[0]<v2.position[0])
//             std::swap(v2,v3);
//         fillTopFlatTriangle(v1, v2, v3);
//     }
//     else
//     {   
//         float split = (v2.position[1] - v1.position[1]) / (v3.position[1] - v1.position[1]);
//         Vertex v4 = v1 + (v3-v1)*split;

//         if (v2.position[0]<v4.position[0]){
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
//     float invslope1 = (v2.position[0] - v1.position[0]) / (v2.position[1] - v1.position[1]);
//     float invslope2 = (v3.position[0] - v1.position[0]) / (v3.position[1] - v1.position[1]);

//     maths::vec3f colorSlope1 = maths::div(maths::sub(v2.color,v1.color),v2.position[1]-v1.position[1]);
//     maths::vec3f colorSlope2 = maths::div(maths::sub(v3.color,v1.color),v3.position[1]-v1.position[1]);
//     float curx1 = v1.position[0];
//     float curx2 = v1.position[0];
    
//     maths::vec3f currentColor1 = v1.color;
//     maths::vec3f currentColor2 = v1.color;

//     // maths::printvec(colorSlope2);
//     // maths::printvec(colorSlope1);

//     maths::vec3f color_alpha= {0};
//     for (float scanlineY = v1.position[1]; scanlineY < v2.position[1]; scanlineY++)
//     {
//         if (curx1 != curx2)
//             color_alpha = maths::div(maths::sub(currentColor2,currentColor1),(curx2 - curx1));

//         for (float i = curx1; i <=curx2; i++) {
//             maths::vec3f finalColor = maths::add(currentColor1,maths::mul(color_alpha,i-curx1));
//             maths::printvec(finalColor);
//             m_canvas->putpixel(i, scanlineY, 1, finalColor);
//         }
//         // m_canvas->drawline(curx1,scanlineY,curx2,scanlineY, color);
//         curx1 += invslope1;
//         curx2 += invslope2;

//         currentColor1= maths::add(currentColor1,colorSlope1);
//         currentColor2= maths::add(currentColor2,colorSlope2);

//     }
// }

// void Triangle::fillTopFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
// {
//     float invslope1 = (v3.position[0] - v1.position[0]) / (v3.position[1] - v1.position[1]);
//     float invslope2 = (v3.position[0] - v2.position[0]) / (v3.position[1] - v2.position[1]);

//     maths::vec3f colorSlope1 = maths::div(maths::sub(v3.color,v1.color),v3.position[1]-v1.position[1]);
//     maths::vec3f colorSlope2 = maths::div(maths::sub(v3.color,v2.color),v3.position[1]-v2.position[1]);

//     float curx1 = v3.position[0];
//     float curx2 = v3.position[0];

//     maths::vec3f currentColor1 = v3.color;
//     maths::vec3f currentColor2 = v3.color;

//     maths::vec3f color_alpha= {0};
//     for (float scanlineY = v3.position[1]; scanlineY > v1.position[1]; scanlineY--)
//     {
//         if (curx1 != curx2)
//             color_alpha = maths::div(maths::sub(currentColor2,currentColor1),(curx2 - curx1));

//         for (float i = curx1; i <=curx2; i++) {
//             maths::vec3f finalColor = maths::add(currentColor1,maths::mul(color_alpha,i-curx1));
//             maths::printvec(finalColor);
//             m_canvas->putpixel(i, scanlineY, 1, finalColor);
//         }

//         // m_canvas->drawline(curx1,scanlineY,curx2,scanlineY, color);
//         curx1 -= invslope1;
//         curx2 -= invslope2;

//         currentColor1= maths::sub(currentColor1,colorSlope1);
//         currentColor2= maths::sub(currentColor2,colorSlope2);
//     }
// }

void Triangle::wireframe_draw(){
    
    m_canvas->drawline(vertices[0][0],vertices[0][1],vertices[1][0],vertices[1][1],color);
	m_canvas->drawline(vertices[1][0],vertices[1][1],vertices[2][0],vertices[2][1],color);
	m_canvas->drawline(vertices[2][0],vertices[2][1],vertices[0][0],vertices[0][1],color);
}

