//
//  Line.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include <glm/glm.hpp>

using namespace glm;

class Line {
private:
    vec2 origin;
    vec2 direction;
    
public:
    Line(vec2 v1, vec2 v2);
    vec2 intersection(Line l);
    vec2 getOrigin();
    vec2 getDirection();
    bool operator==(Line l);
};

#endif /* Line_hpp */
