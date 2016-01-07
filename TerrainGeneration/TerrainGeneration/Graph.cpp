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

vector<int> Graph::DFS(int start) {
    vector<bool> visited;
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        visited.push_back(false);
    }
    stack<int> myStack;
    vector<int> result;
    if(start >= adjacencyMatrix.size()) return result;
    myStack.push(start);
    visited[start] = true;
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

bool visited(int v, vector<int> path) {
    return find(path.begin(), path.end(), v) != path.end();
}

vector<int> normalize(vector<int> path) {
    rotate(path.begin(), min_element(path.begin(), path.end()), path.end());
    return path;
}

vector<int> invert(vector<int> path) {
    reverse(path.begin(), path.end());
    return normalize(path);
}

bool isNew(vector<int> path, vector<vector<int>> cycles) {
    return find(cycles.begin(), cycles.end(), path) == cycles.end();
}

vector<vector<int>> getSubCycles(vector<int> cycle, int n) {
    vector<vector<int>> subcycles;
    for(int i = 0; i < cycle.size(); i++) {
        vector<int> subcycle;
        for(int j = 0; j < n; j++) {
            subcycle.push_back(cycle[(i+j) % cycle.size()]);
        }
        subcycles.push_back(subcycle);
    }
    return subcycles;
    
}

vector<vector<int>> Graph::getMinimalCycles() {
    vector<vector<int>> result = findAllCycles();
    
    // remove cycles that contain other cycles
    vector<vector<int>> tmp2;
    for(vector<int> cycle1 : result) {
        vector<vector<int>> subcycles1 = getSubCycles(cycle1, 3);
        bool flag = true;
        for(vector<int> cycle2 : result) {
            if(cycle1 == cycle2 || cycle1.size() <= cycle2.size()) continue;
            vector<vector<int>> subcycles2 = getSubCycles(cycle2, 3);
            for(vector<int> subcycle1 : subcycles1) {
                for(vector<int> subcycle2 : subcycles2) {
                    if(subcycle1 == subcycle2) {
                        flag = false;
                    }
                    if(!flag) break;
                }
                if(!flag) break;
            }
        }
        if(flag) tmp2.push_back(cycle1);
    }
    
    /*vector<vector<int>> tmp3;
    for(vector<int> cycle1 : tmp2) {
        for(vector<int> cycle2 : result) {
            vector<int> sortedCycle = cycle2;
            sort(sortedCycle.begin(), sortedCycle.end());
            if(sortedCycle == cycle1) {
                tmp3.push_back(cycle2);
                break;
            }
        }
    }*/
    
    // rotate cycles so smallest is in front
    /*vector<vector<int>> tmp4;
    for(vector<int> cycle : tmp3) {
        int min = cycle.front();
        int index = 0;
        for(int i = 0; i < cycle.size(); i++) {
            if(cycle[i] < min) {
                min = cycle[i];
                index = i;
            }
        }
        rotate(cycle.begin(), cycle.begin()+index, cycle.end());
        tmp4.push_back(cycle);
    }
    result = tmp4;*/
    
    result = tmp2;
    
    return result;
}

void findNewCycles(vector<int> path, vector<vector<int>> graph, vector<vector<int>>& cycles) {
        int n = path[0];
        int x;
        vector<int> sub(path.size() + 1);
        
        for(int i = 0; i < graph.size(); i++) {
            for(int j = 0; j < 2; j++) {
                if(graph[i][j] == n) {
                    x = graph[i][(j + 1) % 2];
                    if(!visited(x, path)) {
                        sub[0] = x;
                        copy(path.begin(), path.end(), sub.begin()+1);
                        findNewCycles(sub, graph, cycles);
                    } else if((path.size() > 2) && (x == path[path.size() - 1])) {
                        vector<int> p = normalize(path);
                        vector<int> inv = invert(p);
                        if(isNew(p, cycles) && isNew(inv, cycles)) {
                            cycles.push_back(p);
                        }
                    }
                }
            }
        }
}

vector<vector<int>> Graph::findAllCycles()
{
    vector<vector<int>> cycles;
    vector<vector<int>> graph;
    
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        for(int j = i+1; j < adjacencyMatrix.size(); j++) {
            if(adjacencyMatrix[i][j]) {
                vector<int> edge;
                edge.push_back(i);
                edge.push_back(j);
                graph.push_back(edge);
            }
        }
    }
    
    for(int i = 0; i < graph.size(); i++) {
        vector<int> edge = graph[i];
        findNewCycles(edge, graph, cycles);
    }
    
    /*for(vector<int> cycle : cycles) {
        cout << cycle[0];
        for(int i = 1; i < cycle.size(); i++) {
            cout << "," << cycle[i];
        }
        cout << endl;
    }*/
    
    return cycles;
}

void Graph::print() {
    for(int i = 0; i < adjacencyMatrix.size(); i++) {
        for(int j = 0; j < adjacencyMatrix.size(); j++) {
            cout << adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}
