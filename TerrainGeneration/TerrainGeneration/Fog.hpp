//
//  Fog.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 23/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Fog_hpp
#define Fog_hpp

#include <glm/glm.hpp>

using namespace glm;

class Fog {
private:
    float near;
    float far;
    vec3 color;
public:
    Fog(float near, float far);
    Fog(float near, float far, vec3 color);
    Fog(float near, float far, float intensity);
    
    void setNear(float near);
    float getNear();
    void setFar(float far);
    float getFar();
    void setColor(vec3 color);
    vec3 getColor();
    void setIntensity(float intensity);
    float getIntensity();
};

#endif /* Fog_hpp */
