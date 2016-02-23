//
//  Light.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "Light.hpp"

Light::Light() {
    
}

void Light::setAmbientIntensity(vec3 ambientIntensity) {
    this->ambientIntensity = ambientIntensity;
}

vec3 Light::getAmbientIntensity() {
    return ambientIntensity;
}

void Light::setDiffuseIntensity(vec3 diffuseIntensity) {
    this->diffuseIntensity = diffuseIntensity;
}

vec3 Light::getDiffuseIntensity() {
    return diffuseIntensity;
}
void Light::setSpecularIntensity(vec3 specularIntensity) {
    this->specularIntensity = specularIntensity;
}

vec3 Light::getSpecularIntensity() {
    return specularIntensity;
}