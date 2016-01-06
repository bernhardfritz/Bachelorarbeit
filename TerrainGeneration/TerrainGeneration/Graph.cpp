//
//  Graph.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include <algorithm>
#include <stack>
#include <iostream>
#include "Graph.hpp"

using namespace std;

void Graph::addEdge(int i, int j) {
    cout << i << "->" << j << endl;
    if(i >= adjacencyMatrix.size()) {
        setVertexCount(i+1);
    }
    if(j >= adjacencyMatrix.size()) {
        setVertexCount(j+1);
    }
    if (i >= 0 && i < adjacencyMatrix.size() && j >= 0 && j < adjacencyMatrix.size()) {
        adjacencyMatrix[i][j] = true;
        adjacencyMatrix[j][i] = true;
    }
}

void Graph::removeEdge(int i, int j) {
    if (i >= 0 && i < adjacencyMatrix.size() && j >= 0 && j < adjacencyMatrix.size()) {
        adjacencyMatrix[i][j] = false;
        adjacencyMatrix[j][i] = false;
    }
}

bool Graph::isEdge(int i, int j) {
    if (i >= 0 && i < adjacencyMatrix.size() && j >= 0 && j < adjacencyMatrix.size())
        return adjacencyMatrix[i][j];
    else
        return false;
}

void Graph::setVertexCount(int vertexCount) {
    vector<vector<bool>> newAdjacencyMatrix;
    for(int i = 0; i < vertexCount; i++) {
        vector<bool> tmp;
        for(int j = 0; j < vertexCount; j++) {
            tmp.push_back(false);
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

vector<int> Graph::DFS() {
    vector<bool> visited;
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        visited.push_back(false);
    }
    stack<int> myStack;
    vector<int> result;
    myStack.push(0);
    visited[0] = true;
    result.push_back(myStack.top());
    while(!myStack.empty()) {
        for(int i = 0; i < adjacencyMatrix.size(); i++) {
            if(adjacencyMatrix[myStack.top()][i] && !visited[i]) {
                myStack.push(i);
                visited[i] = true;
                result.push_back(myStack.top());
                i = 0;
                continue;
            }
        }
        myStack.pop();
    }
    return result;
}

vector<vector<int>> Graph::getMinimalCycles() {
    vector<vector<int>> result;
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        vector<bool> visited;
        for(int j = 0; j < adjacencyMatrix.size(); j++) {
            visited.push_back(false);
        }
        vector<int> myStack;
        myStack.push_back(i);
        visited[i] = true;
        while(!myStack.empty()) {
            for(int j = 0; j < adjacencyMatrix.size(); j++) {
                if(adjacencyMatrix[myStack.back()][j] && !visited[j]) {
                    myStack.push_back(j);
                    visited[j] = true;
                    j = 0;
                    continue;
                }
            }
            if(adjacencyMatrix[myStack.back()][myStack.front()] && myStack.size() > 2) result.push_back(myStack);
            myStack.pop_back();
        }
    }
    for(int i = 0; i < result.size(); i++) {
        sort(result[i].begin(), result[i].end());
    }
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    
    vector<vector<int>> tmp;
    for(vector<int> cycle1 : result) {
        bool flag = true;
        for(vector<int> cycle2 : result) {
            if(cycle2 == cycle1) continue;
            if(includes(cycle1.begin(), cycle1.end(), cycle2.begin(), cycle2.end())) {
                flag = false;
            }
        }
        if(flag) tmp.push_back(cycle1);
    }
    result = tmp;
    
    return result;
}

void Graph::print() {
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        for(int j = 0; j < adjacencyMatrix.size(); j++) {
            cout << adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}
