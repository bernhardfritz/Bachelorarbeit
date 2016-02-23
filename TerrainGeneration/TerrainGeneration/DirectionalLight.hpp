//
//  DirectionalLight.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 23/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include "Light.hpp"

class DirectionalLight : public Light {
private:
    float theta;
    float phi;
public:
    DirectionalLight(float theta, float phi);
    DirectionalLight(float theta, float phi, vec3 ambientIntensity, vec3 diffuseIntensity, vec3 specularIntensity);
    
    void setDirection(float theta, float phi);
    vec3 getDirection();
    void setTheta(float theta);
    float getTheta();
    void setPhi(float phi);
    float getPhi();
    void print();
};

#endif /* DirectionalLight_hpp */
