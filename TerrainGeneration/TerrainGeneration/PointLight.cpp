//
//  PointLight.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 23/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "PointLight.hpp"

PointLight::PointLight(vec3 position) {
    setPosition(position);
    setAmbientIntensity(vec3(0.2f, 0.2f, 0.2f));
    setDiffuseIntensity(vec3(0.7f, 0.7f, 0.7f));
    setSpecularIntensity(vec3(1.0f, 1.0f, 1.0f));
}

PointLight::PointLight(vec3 position, vec3 ambientIntensity, vec3 diffuseIntensity, vec3 specularIntensity) {
    setPosition(position);
    setAmbientIntensity(ambientIntensity);
    setDiffuseIntensity(diffuseIntensity);
    setSpecularIntensity(specularIntensity);
}

void PointLight::setPosition(vec3 position) {
    this->position = position;
}

vec3 PointLight::getPosition() {
    return position;
}