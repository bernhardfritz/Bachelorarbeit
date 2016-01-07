//
//  Ray.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Ray {
private:
    vec2 origin;
    vec2 direction;
    
public:
    Ray(vec2 v1, vec2 v2);
    vec2 getIntersection(Ray ray);
    bool intersects(Ray ray);
    vec2 getOrigin();
    vec2 getDirection();
    void sortIntersections(vector<vec2>& intersectionsOfRay);
    bool operator==(Ray ray);
};

#endif /* Ray_hpp */
