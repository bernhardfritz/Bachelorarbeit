//
//  MeshLoader.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "MeshLoader.hpp"

Mesh MeshLoader::loadMesh(string filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
    if(!scene) {
        cout << importer.GetErrorString();
        vector<vec3> vertices;
        vector<unsigned int> indices;
        return Mesh(vertices, indices);
    } else {
        const aiMesh* mesh = scene->mMeshes[0];
        vector<vec3> vertices;
        for(int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D* v = &(mesh->mVertices[i]);
            vertices.push_back(vec3(v->x, v->y, v->z));
        }
        vector<unsigned int> indices;
        for(int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace* face = &(mesh->mFaces[i]);
            indices.push_back(face->mIndices[0]);
            indices.push_back(face->mIndices[1]);
            indices.push_back(face->mIndices[2]);
        }
        if(mesh->HasNormals()) {
            vector<vec3> normals;
            for(int i = 0; i < mesh->mNumVertices; i++) {
                aiVector3D* vn = &(mesh->mNormals[i]);
                normals.push_back(vec3(vn->x, vn->y, vn->z));
            }
            return Mesh(vertices, indices, normals);
        } else {
            return Mesh(vertices, indices);
        }
    }
}