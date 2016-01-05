//
//  Graph.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include <algorithm>
#include "Graph.hpp"

using namespace std;

void Graph::addEdge(int i, int j) {
    if(i > adjacencyMatrix.size()) {
        setVertexCount(i+1);
    }
    if(j > adjacencyMatrix.size()) {
        setVertexCount(j+1);
    }
    if (i >= 0 && i < adjacencyMatrix.size() && j > 0 && j < adjacencyMatrix.size()) {
        adjacencyMatrix[i][j] = true;
        adjacencyMatrix[j][i] = true;
    }
}

void Graph::removeEdge(int i, int j) {
    if (i >= 0 && i < adjacencyMatrix.size() && j > 0 && j < adjacencyMatrix.size()) {
        adjacencyMatrix[i][j] = false;
        adjacencyMatrix[j][i] = false;
    }
}

bool Graph::isEdge(int i, int j) {
    if (i >= 0 && i < adjacencyMatrix.size() && j > 0 && j < adjacencyMatrix.size())
        return adjacencyMatrix[i][j];
    else
        return false;
}

void Graph::setVertexCount(int vertexCount) {
    vector<vector<bool>> newAdjacencyMatrix;
    for(int i = 0; i < vertexCount; i++) {
        vector<bool> tmp;
        for(int j = 0; j < vertexCount; j++) {
            tmp.push_back(0);
        }
        newAdjacencyMatrix.push_back(tmp);
    }
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        for(int j = 0; j < adjacencyMatrix.size(); j++) {
            newAdjacencyMatrix[i][j] = adjacencyMatrix[i][j];
        }
    }
    adjacencyMatrix = newAdjacencyMatrix;
}
