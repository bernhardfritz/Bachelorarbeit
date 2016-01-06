//
//  Graph.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class Graph {
private:
    vector<vector<bool>> adjacencyMatrix;
    void setVertexCount(int vertexCount);
public:
    void addEdge(int i, int j);
    void removeEdge(int i, int j);
    bool isEdge(int i, int j);
    vector<int> DFS();
    vector<vector<int>> getMinimalCycles();
    void print();
};

#endif /* Graph_hpp */
