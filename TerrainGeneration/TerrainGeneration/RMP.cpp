//
//  RMP.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include "RMP.hpp"
#include "Graph.hpp"
#include "Line.hpp"

bool isPointInPolygon(vec2 point, vector<vec2> polygon) {
    int j = (int)polygon.size() - 1;
    bool oddNodes=false;
    
    for(int i = 0; i < polygon.size(); i++) {
        if((polygon[i].y < point.y && polygon[j].y >= point.y) || (polygon[j].y < point.y && polygon[i].y >= point.y)) {
            if(polygon[i].x + (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) * (polygon[j].x - polygon[i].x) < point.x) {
                oddNodes = !oddNodes;
            }
        }
        j = i;
    }
    
    return oddNodes;
}

void RMP::perform(Heightmap &heightmap, int n, int l, int r) {
    for(int i = 0; i < n; i++) {
        Graph g;
        vector<Line> lines;
        // add boundary lines to array
        lines.push_back(Line(vec2(0, 0), vec2(heightmap.getColumns(), 0)));
        lines.push_back(Line(vec2(heightmap.getColumns(), 0), vec2(heightmap.getColumns(), heightmap.getRows())));
        lines.push_back(Line(vec2(heightmap.getColumns(), heightmap.getRows()), vec2(0, heightmap.getRows())));
        lines.push_back(Line(vec2(0, heightmap.getRows()), vec2(0, 0)));
        // add random cutting lines to array
        for(int j = 0; j < l; j++) {
            vec2 v1(drand48() * heightmap.getColumns(), drand48() * heightmap.getRows());
            vec2 v2(drand48() * heightmap.getColumns(), drand48() * heightmap.getRows());
            lines.push_back(Line(v1, v2));
        }
        
        vector<vec2> intersections;
        for(Line line1 : lines) {
            vector<vec2> intersectionsOfLine;
            for(Line line2 : lines) {
                if(line1 == line2) continue;
                vec2 intersection = line1.getIntersection(line2);
                if(intersection.x >= 0 && intersection.x <= heightmap.getColumns() &&
                   intersection.y >= 0 && intersection.y <= heightmap.getRows()) intersectionsOfLine.push_back(intersection);
            }
            line1.sortIntersections(intersectionsOfLine);
            int prev = -1;
            for(vec2 intersectionOfLine : intersectionsOfLine) {
                bool cont = false;
                for(int j = 0; j < intersections.size(); j++) {
                    float dx = abs(intersections[j].x - intersectionOfLine.x);
                    float dy = abs(intersections[j].y - intersectionOfLine.y);
                    if(dx < 0.001 && dy < 0.001) {
                        if(prev != -1) g.addEdge(prev, j);
                        prev = j;
                        cont = true;
                        break;
                    }
                }
                if(cont) continue;
                intersections.push_back(intersectionOfLine);
                int index = (int)intersections.size() - 1;
                if(prev != -1) g.addEdge(prev, index);
                prev = index;
            }
        }
        
        /*for(vec2 intersection : intersections) {
            heightmap.setHeightAt(floor(intersection.x), floor(intersection.y), heightmap.getHeightAt(floor(intersection.x), floor(intersection.y))+1);
        }*/
        
        cout << intersections.size() << " intersections:" << endl;
        for(int j = 0; j < intersections.size(); j++) {
            cout << "[" << j << "] = " << intersections[j].x << ", " << intersections[j].y << endl;
        }
        
        g.print();
        
        cout << "DFS: ";
        vector<int> dfs = g.DFS();
        for(int j = 0; j < dfs.size(); j++) {
            cout << dfs[j] << " ";
        }
        cout << endl;
        
        cout << "Cycles: " << endl;
        vector<vector<int>> cycles = g.getMinimalCycles();
        for(int j = 0; j < cycles.size(); j++) {
            for(int k = 0; k < cycles[j].size(); k++) {
                cout << cycles[j][k] << " ";
            }
            cout << endl;
        }
        
        vector<vector<vec2>> polygons;
        for(vector<int> cycle : cycles) {
            vector<vec2> polygon;
            for(int index : cycle) {
                polygon.push_back(intersections[index]);
            }
            polygons.push_back(polygon);
        }
        
        int count = 0;
        for(vector<vec2> polygon : polygons) {
            for(int row = 0; row <= heightmap.getRows(); row++) {
                for(int column = 0; column <= heightmap.getColumns(); column++) {
                    if(isPointInPolygon(vec2(column, row), polygon)) heightmap.setHeightAt(column, row, count);
                }
            }
            count += 2;
        }
        
        // TODO: build graph with polygons represented as vertices. vertices are connected if polygons share edge.
        // TODO: perform probing algorithm
    }
    heightmap.init();
}