#include "light.h"
#include <GL/glut.h>

Light::Light(){
    position = {10,10,30};
    float ambientInt = 0.2;
    float pointInt = 0.8;
    float specularCoefficient = 4;

    float ambientConstant = 1;
    float diffuseConstant = 1;
    float specularConstant = 1;

}

float Light::calculateIntensity(maths::vec3f point, maths::vec3f normal, maths::vec3f view){

    maths::vec3f l_dir = maths::normalize(maths::sub(position,point));

    float ambientLight = ambientConstant*ambientInt;
    
    float diffuseLight = maths::max(diffuseConstant* 1 *maths::dot(normal,l_dir),0.0f);

    maths::vec3f reflection = maths::normalize(maths::sub(maths::mul(normal,(2* maths::dot(normal,l_dir))),l_dir));
    float specularLight = specularConstant * pointInt * pow(maths::dot(reflection,view),specularCoefficient);
    
    float tmp = ambientLight+diffuseLight;
    tmp = (tmp > 1) ? 1: tmp;
    return tmp;

}

// maths::mat4f Light::getLightTransform(){
//     maths::mat4f proj = maths::perspective(lightcam->zoom, (float)1920/1080);
//     maths::mat4f view = lightcam->getViewMatrix();
//     maths::mat4f product = maths::mul(proj,view);
//     return product;
// }

void Light::processKeyboard(int key, float dt){
    float speed = 20;
    switch (key){
        case GLUT_KEY_RIGHT:
            position[0] += speed*dt;
            break;

        case GLUT_KEY_LEFT:
            position[0] -= speed*dt;
            break;

        case GLUT_KEY_UP:
            position[1] += speed*dt;
            break;

        case GLUT_KEY_DOWN:
            position[1] -= speed*dt;
            break;
        
        case GLUT_KEY_PAGE_UP:
            position[2] += speed*dt;
            break;
        
        case GLUT_KEY_PAGE_DOWN:
            position[2] -= speed*dt;
            break;

        case 'p':
            ambientInt = ambientInt > 0.8 ? 0.8 : ambientInt+dt/speed;
            break;
        
        case 'o':
            ambientInt = ambientInt < 0.01 ? 0.01 : ambientInt-dt/speed;
            break;  
    }
    // lightobj->translate(position[0],position[1],position[2]);
}
