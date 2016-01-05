//
//  Line.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Line.hpp"

Line::Line(vec2 v1, vec2 v2) {
    origin = v1;
    direction = normalize(v2 - v1);
}

vec2 Line::intersection(Line l) {
    float o1 = origin.x;
    float o2 = origin.y;
    float o3 = l.origin.x;
    float o4 = l.origin.y;
    float d1 = direction.x;
    float d2 = direction.y;
    float d3 = l.direction.x;
    float d4 = l.direction.y;
    float t = (-d3 * o2 + d3 * o4 + d4 * o1 - d4 * o3) / (d2 * d3 - d1 * d4);
    return origin + t * direction;
}

vec2 Line::getOrigin() {
    return origin;
}

vec2 Line::getDirection() {
    return direction;
}

bool Line::operator==(Line l) {
    return origin == l.getOrigin() && direction == l.getDirection();
}