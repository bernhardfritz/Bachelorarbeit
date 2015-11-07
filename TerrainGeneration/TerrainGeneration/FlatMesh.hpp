//
//  FlatMesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef FlatMesh_hpp
#define FlatMesh_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class FlatMesh {
private:
    vector<vec3> vertices;
    vector<vec3> normals;
    
public:
    FlatMesh(vector<vec3> vertices);
    FlatMesh(vector<vec3> vertices, vector<vec3> normals);
    
    void setVertices(vector<vec3> vertices);
    vector<vec3> getVertices();
    void setNormals(vector<vec3> normals);
    vector<vec3> getNormals();
    void calculateNormals();
    void print();
};

#endif /* FlatMesh_hpp */
