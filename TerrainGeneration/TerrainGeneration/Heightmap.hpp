//
//  Heightmap.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 09/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Heightmap_hpp
#define Heightmap_hpp

#include <vector>
#include "Mesh.hpp"

using namespace std;

class Heightmap : public Mesh{
private:
    int columns;
    int rows;
    
public:
    Heightmap(int columns, int rows);
};

#endif /* Heightmap_hpp */
