#include "triangle.h"
#include <algorithm>

Triangle::Triangle(Canvas* canvas){
    m_canvas = canvas;
    // color = maths::normalize({220,220,220});
    color = {220,220,220};
    vertices = {{
        {0,0,0},
        {0,0,0},
        {0,0,0}
    }};
    isGouraudShading = false;
}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c){
    m_canvas = canvas;
    // color = maths::normalize({220,220,220});
    color = {220,220,220};

    vertices = {a,b,c};
    isGouraudShading = false;

}

Triangle::Triangle(Canvas* canvas, maths::vec3f a, maths::vec3f b, maths::vec3f c, maths::vec3f mcolor){
    m_canvas = canvas;
    color = {mcolor[0],mcolor[1],mcolor[2]};
    vertices = {a,b,c};
    isGouraudShading = false;

}

void Triangle::setVertex(maths::vec3f a, maths::vec3f b, maths::vec3f c){
    vertices = {a,b,c};
    // maths::printvec(a);
}

void Triangle::setNormals(maths::vec3f na, maths::vec3f nb, maths::vec3f nc){
    // std::cout << "Hello Normal:\n";
    normals = {na,nb,nc};
}

void Triangle::setTexCoords(maths::vec2f uv1,maths::vec2f uv2,maths::vec2f uv3){
    // std::cout << "Hello Texture:\n";
    texCoords = {uv1,uv2,uv3};
}

void Triangle::setIntensity(maths::vec3f intense){
    vertex_intensity[0] = {vertices[0][0],vertices[0][1],vertices[0][2], intense[0]};
    vertex_intensity[1] = {vertices[1][0],vertices[1][1],vertices[1][2], intense[1]};
    vertex_intensity[2] = {vertices[2][0],vertices[2][1],vertices[2][2], intense[2]};
    isGouraudShading = true;
}

void Triangle::gouraudRasterize1(){
    // Sort the points so that y0 <= yl <= y2
    auto P0 = vertex_intensity[0];
    auto P1 = vertex_intensity[1];
    auto P2 = vertex_intensity[2];
    
    if (P1[1] < P0[1]) { std::swap(P1, P0); }
    if (P2[1] < P0[1]) { std::swap(P2, P0);}
    if (P2[1] < P1[1]) { std::swap(P2, P1);}

    //  Compute the x coordinates of the triangle edges
    std::vector<float> x01 = maths::interpolate(P0[1], P0[0], P1[1], P1[0]);
    std::vector<float> h01 = maths::interpolate(P0[1], P0[3],  P1[1], P1[3]);

    std::vector<float> x12 = maths::interpolate(P1[1], P1[0], P2[1], P2[0]);
    std::vector<float> h12 = maths::interpolate(P1[1], P1[3], P2[1], P2[3]);

    std::vector<float> x02 = maths::interpolate(P0[1], P0[0], P2[1], P2[0]);
    std::vector<float> h02 = maths::interpolate(P0[1], P0[3], P2[1], P2[3]);

    //  Concatenate the short sides
    x01.pop_back();

    std::vector<float> x012;
    x012.resize(x01.size() + x12.size());
    std::copy(x01.begin(), x01.end(), x012.begin());
    std::copy(x12.begin(), x12.end(), x012.begin() + x01.size());

    // std::cout<<"------------------------------------------------\n";
    h01.pop_back();

    std::vector<float> h012;
    h012.resize(h01.size() + h12.size());
    std::copy(h01.begin(), h01.end(), h012.begin());
    std::copy(h12.begin(), h12.end(), h012.begin() + h01.size());


    //  Determine which is left and which is right
    int m = x012[x012.size()/2];
    std::vector<float> x_left,x_right, h_left, h_right;

    if (x02[m] < x012[m]) {
        x_left = x02;
        h_left = h02;

        x_right = x012;
        h_right = h012;
    } 
    else {
        x_left = x012;
        h_left = h012;

        x_right = x02;
        h_right = h02;
    }
    // printVec(x_left);
    // std::cout<<"------------------------------------------------\n";
    // printVec(x_right);

    // Draw the horizontal segments
    for (float y = P0[1];y<P2[1]-1;y++) {
        auto x_l = x_left[y- P0[1]];
        auto x_r = x_right[y-P0[1]];

        auto h_segment = maths::interpolate(x_l,h_left[y-P0[1]],x_r, h_right[y-P0[1]]);

        for (float x = x_l; x < x_r;x++) {
            maths::vec3f shadedColor = maths::mul(color,h_segment[x-x_l]);
            // maths::printvec(shadedColor);
            m_canvas->putpixel(x, y,1,shadedColor);
        }
    }
}

void Triangle::gouraudRasterize2(){
    
    auto a = vertex_intensity[0];
    auto b = vertex_intensity[1];
    auto c = vertex_intensity[2];
    // vertex arr[3] = { A,B,C };
    if (a[0] > b[1]) { std::swap(b, a); }
    if (a[0] > c[1]) { std::swap(c, a); }
    if (b[1] > c[1]) { std::swap(c, b); }

    float dx1,di1, dr1, dg1, db1, ddp1, ddp2, ddp3, dx2, dr2, dg2, db2, dx3, dr3, dg3, db3, dr, dg, db, ddp;
    if (b[1] - a[1] > 0) {
        dx1 = (b[0] -a[0]) /(b[1] - a[1]);
        ddp1 = (b[3] - a[3]) / (b[1] - a[1]);
    }
    else
        dx1 = dr1 = dg1 = db1 = 0, ddp1 = 0;;

    if (c[1] - a[1] > 0) {
        dx2 = (c[0] - a[0]) / (c[1] - a[1]);
        ddp2 =(c[3] - a[3]) / (c[1] - a[1]);
    }
    else
        dx2 = dr2 = dg2 = db2 = ddp2 = 0;

    if (c[1] - b[1] > 0) {
        dx3 = (c[0] - c[0]) / (c[1] - b[1]);
        ddp3 = (c[3] - b[3]) / (c[1] - b[1]);
    }
    else
        dx3 = dr3 = dg3 = db3 = ddp3 = 0;
    maths::vec4f S, E, P;
    S = a;
    E = a;
    if (dx1 > dx2) {
        for (; S[1] <= b[1]; S[1]++, E[1]++) {
            if (E[0] - S[0] > 0) {
                ddp = (E[3] - S[3]) / (E[0] - S[0]);
            }
            else
                dr = dg = db = ddp = 0;
            P = S;
            for (; P[0] < E[0]; P[0]++) {
                //MULTIPLY
                maths::vec3f ncolor = maths::mul(color,P[3]);
                m_canvas->putpixel(P[0],P[1],1, ncolor);
                P[3] += ddp;
            }
            //MULTIPLY ?? 
            S[0] += dx2; S[3] += ddp2;
            E[0] += dx1; E[3] += ddp1;
        }

        E = b;
        for (; S[1] <= c[1]; S[1]++, E[1]++) {
            if (E[0] - S[0] > 0) {
                //MULTIPLY
                maths::vec3f ncolor = maths::mul(color,P[3]);
                ddp = (E[3] - S[3]) / (E[0] - S[0]);
            }
            else
                dr = dg = db = 0;
            P = S;
            for (; P[0] <= E[0]; P[0]++) {
                //multiply 
                maths::vec3f ncolor = maths::mul(color,P[3]);
                m_canvas->putpixel(P[0],P[1],1, ncolor);
                P[3] += ddp;
            }
            S[0] += dx2; S[3] += ddp2;
            E[0] += dx3; E[3] += ddp3;
        }
    }
    else {
        for (; S[1] <= b[1]; S[1]++, E[1]++) {
            if (E[0] - S[0] > 0) {
                //MULTIPLY
                maths::vec3f ncolor = maths::mul(color,P[3]);
                ddp = (E[3] - S[3]) / (E[0] - S[0]);
            }
            else
                dr = dg = db = ddp = 0;

            P = S;
            for (; P[0] < E[0]; P[0]++) {
                maths::vec3f ncolor = maths::mul(color,P[3]);
                //multiply
                m_canvas->putpixel(P[0],P[1],P[2],ncolor);
                P[3] += ddp;
            }
            S[0] += dx1;S[3] += ddp1;
            E[0] += dx2;E[3] += ddp2;
        }

        S = b;
        for (; S[1] <= c[1]; S[1]++, E[1]++) {
            if (E[0] - S[0] > 0) {
                //MULTIPLY
                ddp = (E[3] - S[3]) / (E[0] - S[0]);
            }
            else
                dr = dg = db = ddp = 0;

            P = S;
            for (; P[0] < E[0]; P[0]++) {  
                maths::vec3f ncolor = maths::mul(color,P[3]);
                m_canvas->putpixel(P[0],P[1],P[2],ncolor);
                //MULTIPLY
                P[3] += ddp;
            }
            S[0] += dx3; S[3] += ddp3;
            E[0] += dx2; E[3] += ddp2;
        }
    }
}


void Triangle::rasterize(){
    if (isGouraudShading){
        gouraudRasterize1();
        // std::cout << "Hoo gouraud\n";
    }
    else{
        flatRasterize();
    }
}

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
        
        m_canvas->drawline(static_cast<int>(curx1), scanlineY, static_cast<int>(curx2), scanlineY, color);
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
        m_canvas->drawline(static_cast<int>(curx1), scanlineY,static_cast<int>(curx2), scanlineY, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void Triangle::wireframe_draw(){
    
    m_canvas->drawline((int)vertices[0][0], (int)vertices[0][1], (int)vertices[1][0], (int)vertices[1][1],color);
	m_canvas->drawline((int)vertices[1][0], (int)vertices[1][1], (int)vertices[2][0], (int)vertices[2][1],color);
	m_canvas->drawline((int)vertices[2][0], (int)vertices[2][1], (int)vertices[0][0], (int)vertices[0][1],color);
}

