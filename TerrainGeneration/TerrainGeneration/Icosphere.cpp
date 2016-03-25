//
//  Icosphere.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 24/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Icosphere.hpp"

Icosphere::Icosphere(float radius, int tesselationLevel) {
    this->radius = radius;
    
    float t = (1.0f + sqrt(5.0f)) / 2.0f;
    
    vertices.push_back(radius * normalize(vec3(-1.0f,  t, 0.0f)));
    vertices.push_back(radius * normalize(vec3( 1.0f,  t, 0.0f)));
    vertices.push_back(radius * normalize(vec3(-1.0f, -t, 0.0f)));
    vertices.push_back(radius * normalize(vec3( 1.0f, -t, 0.0f)));
    
    vertices.push_back(radius * normalize(vec3(0.0f, -1.0f,  t)));
    vertices.push_back(radius * normalize(vec3(0.0f,  1.0f,  t)));
    vertices.push_back(radius * normalize(vec3(0.0f, -1.0f, -t)));
    vertices.push_back(radius * normalize(vec3(0.0f,  1.0f, -t)));
    
    vertices.push_back(radius * normalize(vec3( t, 0.0f, -1.0f)));
    vertices.push_back(radius * normalize(vec3( t, 0.0f,  1.0f)));
    vertices.push_back(radius * normalize(vec3(-t, 0.0f, -1.0f)));
    vertices.push_back(radius * normalize(vec3(-t, 0.0f,  1.0f)));
    
    indices.push_back(0); indices.push_back(11); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(1);
    indices.push_back(0); indices.push_back(1); indices.push_back(7);
    indices.push_back(0); indices.push_back(7); indices.push_back(10);
    indices.push_back(0); indices.push_back(10); indices.push_back(11);
    
    indices.push_back(1); indices.push_back(5); indices.push_back(9);
    indices.push_back(5); indices.push_back(11); indices.push_back(4);
    indices.push_back(11); indices.push_back(10); indices.push_back(2);
    indices.push_back(10); indices.push_back(7); indices.push_back(6);
    indices.push_back(7); indices.push_back(1); indices.push_back(8);
    
    indices.push_back(3); indices.push_back(9); indices.push_back(4);
    indices.push_back(3); indices.push_back(4); indices.push_back(2);
    indices.push_back(3); indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(8);
    indices.push_back(3); indices.push_back(8); indices.push_back(9);
    
    indices.push_back(4); indices.push_back(9); indices.push_back(5);
    indices.push_back(2); indices.push_back(4); indices.push_back(11);
    indices.push_back(6); indices.push_back(2); indices.push_back(10);
    indices.push_back(8); indices.push_back(6); indices.push_back(7);
    indices.push_back(9); indices.push_back(8); indices.push_back(1);
    
    tesselateIcosahedron(tesselationLevel);
    
    setVertices(vertices);
    setIndices(indices);
    init();
    calculateNormals();
}

void Icosphere::tesselateIcosahedron(int tesselationLevel) {
    for(int i = 0; i < tesselationLevel; i++) {
        vector<unsigned int> temporaryIndices;
        for(int j = 0; j < indices.size(); j += 3) {
            unsigned int v1 = indices[j];
            unsigned int v2 = indices[j+1];
            unsigned int v3 = indices[j+2];
            
            unsigned int a = getMiddlePoint(v1, v2);
            unsigned int b = getMiddlePoint(v2, v3);
            unsigned int c = getMiddlePoint(v3, v1);
            
            temporaryIndices.push_back(v1); temporaryIndices.push_back(a); temporaryIndices.push_back(c);
            temporaryIndices.push_back(v2); temporaryIndices.push_back(b); temporaryIndices.push_back(a);
            temporaryIndices.push_back(v3); temporaryIndices.push_back(c); temporaryIndices.push_back(b);
            temporaryIndices.push_back(a); temporaryIndices.push_back(b); temporaryIndices.push_back(c);
        }
        indices = temporaryIndices;
    }

}

int Icosphere::getMiddlePoint(unsigned int p1, unsigned int p2) {
    vec3 point1 = vertices[p1];
    vec3 point2 = vertices[p2];
    vec3 middle = vec3((point1.x + point2.x) / 2.0f, (point1.y + point2.y) / 2.0f, (point1.z + point2.z) / 2.0f);
    vertices.push_back(radius * normalize(middle));
    return (int)vertices.size() - 1;
}