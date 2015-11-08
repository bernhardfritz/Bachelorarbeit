//
//  Material.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <glm/glm.hpp>

using namespace glm;

class Material {
private:
    vec3 ambientReflectance;
    vec3 diffuseReflectance;
    vec3 specularReflectance;
    float shininess;
    
public:
    Material();
    Material(vec3 ambientReflectance, vec3 diffuseReflectance, vec3 specularReflectance, float shininess);
    Material(float ambientReflectance, float diffuseReflectance, float specularReflectance, float shininess);
    void setAmbientReflectance(vec3 ambientReflectance);
    void setAmbientReflectance(float ambientReflectance);
    vec3 getAmbientReflectance();
    void setDiffuseReflectance(vec3 diffuseReflectance);
    void setDiffuseReflectance(float diffuseReflectance);
    vec3 getDiffuseReflectance();
    void setSpecularReflectance(vec3 specularReflectance);
    void setSpecularReflectance(float specularReflectance);
    vec3 getSpecularReflectance();
    void setShininess(float shininess);
    float getShininess();
};

#endif /* Material_hpp */