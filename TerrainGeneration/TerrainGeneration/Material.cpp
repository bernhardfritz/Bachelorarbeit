//
//  Material.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "Material.hpp"

Material::Material() {
    setAmbientReflectance(1.0f);
    setDiffuseReflectance(1.0f);
    setSpecularReflectance(0.2f);
    setShininess(30.0f);
}

Material::Material(vec3 ambientReflectance, vec3 diffuseReflectance, vec3 specularReflectance, float shininess) {
    setAmbientReflectance(ambientReflectance);
    setDiffuseReflectance(diffuseReflectance);
    setSpecularReflectance(specularReflectance);
    setShininess(shininess);
}

Material::Material(float ambientReflectance, float diffuseReflectance, float specularReflectance, float shininess) {
    setAmbientReflectance(ambientReflectance);
    setDiffuseReflectance(diffuseReflectance);
    setSpecularReflectance(specularReflectance);
    setShininess(shininess);
}

void Material::setAmbientReflectance(vec3 ambientReflectance) {
    this->ambientReflectance = ambientReflectance;
}

void Material::setAmbientReflectance(float ambientReflectance) {
    this->ambientReflectance = vec3(ambientReflectance, ambientReflectance, ambientReflectance);
}

vec3 Material::getAmbientReflectance() {
    return ambientReflectance;
}

void Material::setDiffuseReflectance(vec3 diffuseReflectance) {
    this->diffuseReflectance = diffuseReflectance;
}

void Material::setDiffuseReflectance(float diffuseReflectance) {
    this->diffuseReflectance = vec3(diffuseReflectance, diffuseReflectance, diffuseReflectance);
}

vec3 Material::getDiffuseReflectance() {
    return diffuseReflectance;
}

void Material::setSpecularReflectance(vec3 specularReflectance) {
    this->specularReflectance = specularReflectance;
}

void Material::setSpecularReflectance(float specularReflectance) {
    this->specularReflectance = vec3(specularReflectance, specularReflectance, specularReflectance);
}

vec3 Material::getSpecularReflectance() {
    return specularReflectance;
}

void Material::setShininess(float shininess) {
    this->shininess = shininess;
}

float Material::getShininess() {
    return shininess;
}