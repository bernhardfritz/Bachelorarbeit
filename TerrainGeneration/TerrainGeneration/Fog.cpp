//
//  Fog.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 23/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Fog.hpp"

Fog::Fog(float near, float far) {
    setNear(near);
    setFar(far);
    setIntensity(0.5f);
}

Fog::Fog(float near, float far, vec3 color) {
    setNear(near);
    setFar(far);
    setColor(color);
}

Fog::Fog(float near, float far, float intensity) {
    setNear(near);
    setFar(far);
    setIntensity(intensity);
}

void Fog::setNear(float near) {
    this->near = near;
}

float Fog::getNear() {
    return near;
}

void Fog::setFar(float far) {
    this->far = far;
}

float Fog::getFar() {
    return far;
}

void Fog::setColor(vec3 color) {
    this->color = color;
}

vec3 Fog::getColor() {
    return color;
}

void Fog::setIntensity(float intensity) {
    this->color = vec3(intensity, intensity, intensity);
}

float Fog::getIntensity() {
    return (color.r + color.g + color.b) / 3.0f;
}