//
//  Cube.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 17/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Cube.hpp"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

Cube::Cube(float a) {
    vector<vec3> vertices;
    vector<unsigned int> indices;
    float h = a/2.0f;
    
    vertices.push_back(vec3(-h, -h, -h)); // 000 lbb 0
    vertices.push_back(vec3(-h, -h,  h)); // 001 lbf 1
    vertices.push_back(vec3(-h,  h, -h)); // 010 ltb 2
    vertices.push_back(vec3(-h,  h,  h)); // 011 ltf 3
    vertices.push_back(vec3( h, -h, -h)); // 100 rbb 4
    vertices.push_back(vec3( h, -h,  h)); // 101 rbf 5
    vertices.push_back(vec3( h,  h, -h)); // 110 rtb 6
    vertices.push_back(vec3( h,  h,  h)); // 111 rtf 7
    
    //left
    indices.push_back(2); indices.push_back(0), indices.push_back(1); // ltb lbb lbf
    indices.push_back(1); indices.push_back(3), indices.push_back(2); // lbf ltf ltb
    
    //right
    indices.push_back(7); indices.push_back(5); indices.push_back(4); // rtf rbf rbb
    indices.push_back(4); indices.push_back(6); indices.push_back(7); // rbb rtb rtf
    
    //top
    indices.push_back(2); indices.push_back(3); indices.push_back(7); // ltb ltf rtf
    indices.push_back(7); indices.push_back(6); indices.push_back(2); // rtf rtb ltb
    
    //bottom
    indices.push_back(1); indices.push_back(0); indices.push_back(4); // lbf lbb rbb
    indices.push_back(4); indices.push_back(5); indices.push_back(1); // rbb rbf lbf
    
    //front
    indices.push_back(3); indices.push_back(1); indices.push_back(5); // ltf lbf rbf
    indices.push_back(5); indices.push_back(7); indices.push_back(3); // rbf rtf ltf
    
    //back
    indices.push_back(6); indices.push_back(4); indices.push_back(0); // rtb rbb lbb
    indices.push_back(0); indices.push_back(2); indices.push_back(6); // lbb ltb rtb
    
    setVertices(vertices);
    setIndices(indices);
    setMaterial(new Material());
    init();
    calculateNormals();
}