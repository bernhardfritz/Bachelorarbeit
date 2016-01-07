//
//  Ray.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Ray.hpp"

Ray::Ray(vec2 v1, vec2 v2) {
    origin = v1;
    direction = normalize(v2 - v1);
}

vec2 Ray::getIntersection(Ray ray) {
    vec2 as = origin;
    vec2 ad = direction;
    vec2 bs = ray.getOrigin();
    vec2 bd = ray.getDirection();
    float dx = bs.x - as.x;
    float dy = bs.y - as.y;
    float det = bd.x * ad.y - bd.y * ad.x;
    float u = (dy * bd.x - dx * bd.y) / det;
    //float v = (dy * ad.x - dx * ad.y) / det;
    return origin + u * direction;
}

bool Ray::intersects(Ray ray) {
    vec2 as = origin;
    vec2 ad = direction;
    vec2 bs = ray.getOrigin();
    vec2 bd = ray.getDirection();
    float dx = bs.x - as.x;
    float dy = bs.y - as.y;
    float det = bd.x * ad.y - bd.y * ad.x;
    float u = (dy * bd.x - dx * bd.y) / det;
    float v = (dy * ad.x - dx * ad.y) / det;
    return u >= 0 && v >= 0;
}

vec2 Ray::getOrigin() {
    return origin;
}

vec2 Ray::getDirection() {
    return direction;
}

void Ray::sortIntersections(vector<vec2>& intersectionsOfRay) {
    for(int n = (int)intersectionsOfRay.size(); n > 1; n--) {
        for(int i = 0; i < n - 1; i++) {
            float a = length(intersectionsOfRay[i] - origin);
            float b = length(intersectionsOfRay[i+1] - origin);
            if(a > b) {
                vec2 tmp = intersectionsOfRay[i];
                intersectionsOfRay[i] = intersectionsOfRay[i+1];
                intersectionsOfRay[i+1] = tmp;
            }
        }
    }
}

bool Ray::operator==(Ray ray) {
    return origin == ray.getOrigin() && direction == ray.getDirection();
}