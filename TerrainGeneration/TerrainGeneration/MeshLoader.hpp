//
//  MeshLoader.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef MeshLoader_hpp
#define MeshLoader_hpp

#include <iostream>
#include <string>
#include "Mesh.hpp"

using namespace std;

class MeshLoader {
private:
    
public:
    Mesh loadMesh(string filename);
};

#endif /* MeshLoader_hpp */
