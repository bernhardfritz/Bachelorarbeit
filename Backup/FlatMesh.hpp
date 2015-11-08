//
//  FlatMesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef FlatMesh_hpp
#define FlatMesh_hpp

#include "Mesh.hpp"

using namespace std;
using namespace glm;

class FlatMesh : public Mesh{
private:
    
public:
    FlatMesh(vector<vec3> vertices);
    FlatMesh(vector<vec3> vertices, vector<vec3> normals);
    
    void draw();
    void calculateNormals();
};

#endif /* FlatMesh_hpp */
