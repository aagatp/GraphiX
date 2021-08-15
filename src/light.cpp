#include "light.h"
#include <GL/glut.h>

Light::Light(){
    position = {10,10,10};
    float ambientInt = 0.2;
    float pointInt = 0.5;
    float specularCoefficient = 32;

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

void Light::processKeyboard(int key, float dt){
    switch (key){
        case GLUT_KEY_RIGHT:
                position[0] += 1;
                break;

        case GLUT_KEY_LEFT:
                position[0] -= 1;
                break;

        case GLUT_KEY_UP:
                position[2] += 1;
                break;

        case GLUT_KEY_DOWN:
                position[2] -= 1;
                break;
    }
}