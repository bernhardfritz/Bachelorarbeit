//
//  Light.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "Light.hpp"

Light::Light(vec3 position, vec3 ambientIntensity, vec3 diffuseIntensity, vec3 specularIntensity) {
    setPosition(position);
    setAmbientIntensity(ambientIntensity);
    setDiffuseIntensity(diffuseIntensity);
    setSpecularIntensity(specularIntensity);
}

void Light::setPosition(vec3 position) {
    this->position = position;
}

vec3 Light::getPosition() {
    return position;
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