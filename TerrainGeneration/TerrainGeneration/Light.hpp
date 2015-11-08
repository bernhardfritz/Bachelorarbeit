//
//  Light.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <glm/glm.hpp>

using namespace glm;

class Light {
private:
    vec3 position;
    vec3 ambientIntensity;
    vec3 diffuseIntensity;
    vec3 specularIntensity;
    
public:
    Light(vec3 position);
    Light(vec3 position, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor);
    void setPosition(vec3 position);
    vec3 getPosition();
    void setAmbientIntensity(vec3 ambientIntensity);
    vec3 getAmbientIntensity();
    void setDiffuseIntensity(vec3 diffuseIntensity);
    vec3 getDiffuseIntensity();
    void setSpecularIntensity(vec3 specularIntensity);
    vec3 getSpecularIntensity();
};

#endif /* Light_hpp */
