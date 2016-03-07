//
//  MousePicker.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 07/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef MousePicker_hpp
#define MousePicker_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Heightmap.hpp"

#define RECURSION_COUNT 200
#define RAY_RANGE 0.5f

#define EPSILON 0.000001

using namespace glm;

class MousePicker {
private:
    vec3 currentRay;
    
    vec3 getNormalizedDeviceCoordinates(int mouse_x, int mouse_y, int width, int height);
    vec4 getHomogeneousClipCoordinates(vec3 ray_nds);
    vec4 getEyeCoordinates(vec4 ray_clip, mat4 proj);
    vec3 getWorldCoordinates(vec4 ray_eye, mat4 view);
    
    vec3 getPointOnRay(vec3 ray, float distance, Camera& camera);
    vec3 binarySearch(int count, float start, float finish, vec3 ray, Camera& camera, Heightmap& heightmap);
    bool intersectionInRange(float start, float finish, vec3 ray, Camera& camera, Heightmap& heightmap);
    bool isUnderGround(vec3 testPoint, Heightmap& heightmap);
    
    // möller trumbore triangle ray intersection test
    bool triangle_intersection(vec3 v1, vec3 v2, vec3 v3, vec3 o, vec3 d, float* out);
    
public:
    MousePicker();
    ~MousePicker();
    
    void update(int mouse_x, int mouse_y, int width, int height, mat4 proj, mat4 view);
    vec3 getCurrentRay();
    vec3 getIntersection(Camera& camera, Heightmap& heightmap);
    
    // far too slow
    bool triangle_intersects(Camera& camera, Heightmap& heightmap);
};

#endif /* MousePicker_hpp */
