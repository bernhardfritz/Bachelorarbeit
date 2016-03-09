//
//  Cone.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 15/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Cone.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;
using namespace glm;

Cone::Cone(float radius, float height, int triangles) {
    vector<vec3> vertices;
    vector<unsigned int> indices;
    float angle = 0.0f;
    float delta = two_pi<float>() / triangles;
    for(int i = 0; i < triangles; i++) {
        vertices.push_back(vec3(radius * sin(angle), 0.0f, radius * cos(angle)));
        angle += delta;
    }
    vertices.push_back(vec3(0.0f, height, 0.0f)); // top center
    for(int i = 0; i < vertices.size() - 1; i++) {
        indices.push_back(i);
        indices.push_back((i + 1) % (vertices.size() - 1));
        indices.push_back((int)vertices.size() - 1);
    }
    setVertices(vertices);
    setIndices(indices);
    material.setDiffuseReflectance(vec3(drand48(), drand48(), drand48()));
    init();
    calculateNormals();
}