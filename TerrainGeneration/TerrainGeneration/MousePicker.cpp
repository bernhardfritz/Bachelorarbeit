//
//  MousePicker.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 07/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "MousePicker.hpp"

MousePicker::MousePicker() {
    
}

MousePicker::~MousePicker() {
    
}

vec3 MousePicker::getNormalizedDeviceCoordinates(int mouse_x, int mouse_y, int width, int height) {
    float x = (2.0f * mouse_x) / width - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / height;
    float z = 1.0f;
    return vec3(x, y, z);
}

vec4 MousePicker::getHomogeneousClipCoordinates(vec3 ray_nds) {
    return vec4(ray_nds.x, ray_nds.y, -1.0f , 1.0f);
}

vec4 MousePicker::getEyeCoordinates(vec4 ray_clip, mat4 proj) {
    vec4 ray_eye = inverse(proj) * ray_clip;
    return vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
}

vec3 MousePicker::getWorldCoordinates(vec4 ray_eye, mat4 view) {
    vec3 ray_wor = vec3(inverse(view) * ray_eye);
    return normalize(ray_wor);
}

vec3 MousePicker::getPointOnRay(vec3 ray, float distance, Camera& camera) {
    vec3 start = vec3(camera.getX(), camera.getY(), camera.getZ());
    vec3 scaledRay = ray * distance;
    return start + scaledRay;
}

vec3 MousePicker::binarySearch(int count, float start, float finish, vec3 ray, Camera& camera, Heightmap& heightmap) {
    float half = start + ((finish - start) / 2.0f);
    if(count >= RECURSION_COUNT) {
        vec3 endPoint = getPointOnRay(ray, half, camera);
        return endPoint;
    }
    if(intersectionInRange(start, half, ray, camera, heightmap)) {
        return binarySearch(count + 1, start, half, ray, camera, heightmap);
    } else {
        return binarySearch(count + 1, half, finish, ray, camera, heightmap);
    }
}

bool MousePicker::intersectionInRange(float start, float finish, vec3 ray, Camera& camera, Heightmap& heightmap) {
    vec3 startPoint = getPointOnRay(ray, start, camera);
    vec3 endPoint = getPointOnRay(ray, finish, camera);
    return !isUnderGround(startPoint, heightmap) && isUnderGround(endPoint, heightmap);
}

bool MousePicker::isUnderGround(vec3 testPoint, Heightmap& heightmap) {
    float height = heightmap.getHeightAt(floor(testPoint.x * heightmap.getColumns()), floor(testPoint.z * heightmap.getRows()));
    return testPoint.y < height;
}

bool MousePicker::triangle_intersection(vec3 v1, vec3 v2, vec3 v3, vec3 o, vec3 d, float* out) {
    vec3 e1, e2;  //Edge1, Edge2
    vec3 P, Q, T;
    float det, inv_det, u, v;
    float t;
    
    //Find vectors for two edges sharing V1
    e1 = v2 - v1;
    e2 = v3 - v1;
    //Begin calculating determinant - also used to calculate u parameter
    P = glm::cross(d, e2);
    //if determinant is near zero, ray lies in plane of triangle
    det = glm::dot(e1, P);
    //NOT CULLING
    if(det > -EPSILON && det < EPSILON) return 0;
    inv_det = 1.f / det;
    
    //calculate distance from V1 to ray origin
    T = o - v1;
    
    //Calculate u parameter and test bound
    u = glm::dot(T, P) * inv_det;
    //The intersection lies outside of the triangle
    if(u < 0.f || u > 1.f) return false;
    
    //Prepare to test v parameter
    Q = glm::cross(T, e1);
    
    //Calculate V parameter and test bound
    v = glm::dot(d, Q) * inv_det;
    //The intersection lies outside of the triangle
    if(v < 0.f || u + v  > 1.f) return 0;
    
    t = glm::dot(e2, Q) * inv_det;
    
    if(t > EPSILON) { //ray intersection
        *out = t;
        return true;
    }
    
    // No hit, no win
    return false;
}

void MousePicker::update(int mouse_x, int mouse_y, int width, int height, mat4 proj, mat4 view) {
    vec3 ray_nds = getNormalizedDeviceCoordinates(mouse_x, mouse_y, width, height);
    vec4 ray_clip = getHomogeneousClipCoordinates(ray_nds);
    vec4 ray_eye = getEyeCoordinates(ray_clip, proj);
    currentRay = getWorldCoordinates(ray_eye, view);
}

vec3 MousePicker::getCurrentRay() {
    return currentRay;
}

vec3 MousePicker::getIntersection(Camera& camera, Heightmap& heightmap) {
    return binarySearch(0, 0, RAY_RANGE, currentRay, camera, heightmap);
}

bool MousePicker::triangle_intersects(Camera& camera, Heightmap& heightmap) {
    vec3 o = vec3(camera.getX(), camera.getY(), camera.getZ());
    vec3 d = currentRay;
    float out;
    for(int i = 0; i < heightmap.getIndices().size(); i = i + 3) {
        vec3 v1 = heightmap.getVertices()[heightmap.getIndices()[i]];
        vec3 v2 = heightmap.getVertices()[heightmap.getIndices()[i+1]];
        vec3 v3 = heightmap.getVertices()[heightmap.getIndices()[i+2]];
        if(triangle_intersection(v1, v2, v3, o, d, &out)) return true;
    }
    return false;
}

