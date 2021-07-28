#include "maths.h"


namespace maths{

float radians(float degree){
    return 3.1416*degree/180;
}

float dot(vec3f a, vec3f b){
    float product= 0;
    for (int i=0; i<3; i++){
        product += a[i]*b[i];
    }
    return product;
}

vec3f cross(vec3f a, vec3f b){
    vec3f product = {};
    for (int i=0; i<3; i++){
        int tmpindex = (i+1)%3;
        product[i] = a[tmpindex]*b[(tmpindex+1)%3] - a[(tmpindex+1)%3]*b[tmpindex];
    }
    return product;
}

float veclength(vec3f v){
    float temp = sqrt(maths::dot(v,v));
    return temp;
}

vec3f normalize(vec3f a){
    vec3f product = {0};
    float length = maths::veclength(a);
    for (int i=0;i<3; i++){
        product[i] = a[i]/length;
    }
    return product;
}

mat4f mul(mat4f a, mat4f b){
    mat4f product = {0};
    for (int i=0;i<4; i++){
        for (int j=0; j<4; j++){
            for (int k=0; k<4; k++){
                product[i][j] += a[i][k]*b[k][j];
            }
        }
    }
    return product;
}

vec3f mul(mat4f a, vec3f b){

    float homogenous_b[] = {b[0],b[1],b[2],1};
    float homogenous_product[4] = {0};
    for (int i=0;i<4;i++){
        for (int j=0; j<4; j++){
            homogenous_product[i] += a[i][j] * homogenous_b[j]; 
        }
    }
    vec3f product = {homogenous_product[0],homogenous_product[1],homogenous_product[2]};
    return product;
}

vec3f mul(vec3f a, float num){
    vec3f product = {};
    for (int i=0;i<3;i++){
        product[i] = a[i]*num;
    }
    return product;
}

vec3f add(vec3f a, vec3f b){
    vec3f product = {};
    for (int i=0; i<3; i++){
        product[i] = a[i] + b[i];
    }
    return product;
}

vec3f sub(vec3f a, vec3f b){
    vec3f product = {};
    for (int i=0; i<3; i++){
        product[i] = a[i] - b[i];
    }
    return product;
}

mat4f orthoproject(){
    mat4f ortho= {{{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 1}}};
    return ortho;
}

mat4f perspective(){
    float zprp= 100, xprp = 0, yprp = 0;
    float zvp=0;
    float dp = zprp - zvp;
    maths::mat4f persmatrix = {{{1,0,xprp/dp,-xprp*zvp/dp},
                            {0,1,yprp/dp,-yprp*zvp/dp},
                            {0,0,-zvp/dp,zvp*(zprp/dp)},
                            {0,0,-1/dp,zprp/dp}}};        

    maths::mat4f pers = {{
                        {1,0,xprp/dp,-xprp*zvp/dp},
                        {0,1,yprp/dp,-yprp*zvp/dp},
                        {0,0,-zvp/dp,zvp*(zprp/dp)},
                        {0,0,-1/dp,zprp/dp}
                        }};        
    
    return persmatrix;
}

mat4f rotate(float yaw, float pitch=0, float roll=0){
    // std::cout << "Hello";
    mat4f rotation = {{
        {cos(yaw)*cos(pitch),cos(yaw)*sin(pitch)*sin(roll)-sin(yaw)*cos(roll),cos(yaw)*sin(pitch)*cos(roll)+sin(yaw)*sin(roll),0},
        {sin(yaw)*cos(pitch),sin(yaw)*sin(pitch)*sin(roll)+cos(yaw)*cos(roll),sin(yaw)*sin(pitch)*cos(roll)-cos(yaw)*sin(roll),0},
        {-sin(pitch),cos(pitch)*sin(roll),cos(pitch)*cos(roll),0},
        {0,0,0,1}}};
    return rotation;
}

mat4f lookAt(vec3f eye, vec3f target, vec3f vUp={0,1,0})
{
    // Calculate new forward direction
    vec3f forward = maths::normalize(maths::sub(eye,target));
    vec3f left = maths::normalize(maths::cross(vUp,forward));
    // Calculate new Up direction
    vec3f up = maths::cross(forward,left);

    mat4f view ={{
                {left[0],left[1],left[2],-maths::dot(left,eye)},
                {up[0],up[1],up[2],-maths::dot(up,eye)},
                {forward[0],forward[1],forward[2],-maths::dot(forward,eye)},
                {0,0,0,1}
            }};
    // Construct Dimensioning and Translation Matrix	
    return view;

}

mat4f translate(float tx, float ty, float tz){
    mat4f translation = {{{1,0,0,tx},{0,1,0,ty},{0,0,1,tz},{0,0,0,1}}};
    return translation;
}

mat4f scale(float sx, float sy, float sz){
    mat4f scaling = {{{sx,0,0,0},{0,sy,0,0},{0,0,sz,0},{0,0,0,1}}};
    return scaling;
}

}