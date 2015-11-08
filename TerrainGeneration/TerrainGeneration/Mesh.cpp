//
//  Mesh.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include "Mesh.hpp"

Mesh::Mesh(vector<vec3> vertices, vector<unsigned int> indices) {
    setVertices(vertices);
    setIndices(indices);
    setMaterial(new Material());
    calculateNormals(); // calculateNormals() calls init()
}

Mesh::Mesh(vector<vec3> vertices, vector<unsigned int> indices, vector<vec3> normals) {
    setVertices(vertices);
    setIndices(indices);
    setNormals(normals);
    setMaterial(new Material());
    init();
}

void Mesh::setVertices(vector<vec3> vertices) {
    this->vertices = vertices;
}

vector<vec3> Mesh::getVertices() {
    return vertices;
}

void Mesh::setNormals(vector<vec3> normals) {
    this->normals = normals;
}

vector<vec3> Mesh::getNormals() {
    return normals;
}

void Mesh::setIndices(vector<unsigned int> indices) {
    this->indices = indices;
}

vector<unsigned int> Mesh::getIndices() {
    return indices;
}

void Mesh::setMaterial(Material* material) {
    this->material = material;
}

Material* Mesh::getMaterial() {
    return material;
}

void Mesh::calculateNormals() {
    vector<vec3> temporaryNormals;
    for(int i = 0; i < vertices.size(); i++) {
        temporaryNormals.push_back(vec3(0.0, 0.0, 0.0));
    }
    for(int i = 0; i < indices.size(); i += 3) {
        vec3 v0 = vertices[indices[i+1]] - vertices[indices[i]];
        vec3 v1 = vertices[indices[i+2]] - vertices[indices[i]];
        vec3 normal = normalize(cross(v0, v1));
        temporaryNormals[indices[i]] += normal;
        temporaryNormals[indices[i+1]] += normal;
        temporaryNormals[indices[i+2]] += normal;
    }
    for(int i = 0; i < temporaryNormals.size(); i++) {
        temporaryNormals[i] = normalize(temporaryNormals[i]);
    }
    setNormals(temporaryNormals);
    init();
}

void Mesh::init() {
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    nbo = 0;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void Mesh::print() {
    cout << "vertices:" << endl;
    for(int i = 0; i < vertices.size(); i++) {
        cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
    }
    cout << "normals:" << endl;
    for(int i = 0; i < normals.size(); i++) {
        cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
    }
    cout << "indices:" << endl;
    for(int i = 0; i < indices.size(); i += 3) {
        cout << indices[i] << " " << indices[i+1] << " " << indices[i+2] << endl;
    }
}