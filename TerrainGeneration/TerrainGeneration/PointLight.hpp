//
//  PointLight.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 23/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include "Light.hpp"

class PointLight : public Light {
private:
    vec3 position;
public:
    PointLight(vec3 position);
    PointLight(vec3 position, vec3 ambientIntensity, vec3 diffuseIntensity, vec3 specularIntensity);
    
    void setPosition(vec3 position);
    vec3 getPosition();
};

#endif /* PointLight_hpp */
