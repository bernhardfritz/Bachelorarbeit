//
//  DirectionalLight.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 23/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "DirectionalLight.hpp"
#include <glm/gtc/constants.hpp>
#include <stdio.h>

DirectionalLight::DirectionalLight(float theta, float phi) {
    setDirection(theta, phi);
    setAmbientIntensity(vec3(0.2f, 0.2f, 0.2f));
    setDiffuseIntensity(vec3(0.7f, 0.7f, 0.7f));
    setSpecularIntensity(vec3(1.0f, 1.0f, 1.0f));
}

DirectionalLight::DirectionalLight(float theta, float phi, vec3 ambientIntensity, vec3 diffuseIntensity, vec3 specularIntensity) {
    setDirection(theta, phi);
    setAmbientIntensity(ambientIntensity);
    setDiffuseIntensity(diffuseIntensity);
    setSpecularIntensity(specularIntensity);
}

void DirectionalLight::setDirection(float theta, float phi) {
    setTheta(theta);
    setPhi(phi);
}

vec3 DirectionalLight::getDirection() {
    float r = 1.0f;
    return vec3(r * sin(theta) * cos(phi), r * cos(theta), r * sin(theta) * sin(phi));
}

void DirectionalLight::setTheta(float theta) {
    float f = theta * one_over_two_pi<float>();
    f = f - (int)f;
    theta = f * two_pi<float>();
    if(theta < 0) theta += two_pi<float>();
    this->theta = theta;
    //print();
}

float DirectionalLight::getTheta() {
    return theta;
}

void DirectionalLight::setPhi(float phi) {
    float f = phi * one_over_two_pi<float>();
    f = f - (int)f;
    phi = f * two_pi<float>();
    if(phi < 0) phi += two_pi<float>();
    this->phi = phi;
    //print();
}

float DirectionalLight::getPhi() {
    return phi;
}

void DirectionalLight::print() {
    printf("theta: %.2f phi: %.2f\n", theta, phi);
    vec3 dir = getDirection();
    printf("x: %.2f y: %.2f z: %.2f\n", dir.x, dir.y, dir.z);
}