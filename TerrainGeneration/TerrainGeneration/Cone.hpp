//
//  Cone.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 15/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Cone_hpp
#define Cone_hpp

#include "Mesh.hpp"

class Cone : public Mesh {
private:
public:
    Cone(float radius, float height, int triangles);
};

#endif /* Cone_hpp */