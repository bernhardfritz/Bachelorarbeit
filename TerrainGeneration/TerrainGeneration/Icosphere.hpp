//
//  Icosphere.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 24/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Icosphere_hpp
#define Icosphere_hpp

#include <stdio.h>
#include "Mesh.hpp"

class Icosphere : public Mesh{
private:
    float radius;
    
    void tesselateIcosahedron(int tesselationLevel);
    int getMiddlePoint(unsigned int p1, unsigned int p2);
public:
    Icosphere(float radius, int tesselationLevel);
};

#endif /* Icosphere_hpp */
