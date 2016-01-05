//
//  RMP.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "RMP.hpp"
#include "Graph.hpp"
#include "Line.hpp"

bool compare(vec2 v1, vec2 v2) {
    return v1.x < v2.x;
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
                vec2 intersection = line1.intersection(line2);
                if(intersection.x >= 0 && intersection.x <= heightmap.getColumns() &&
                   intersection.y >= 0 && intersection.y <= heightmap.getRows()) intersectionsOfLine.push_back(intersection);
            }
            sort(intersectionsOfLine.begin(), intersectionsOfLine.end(), compare);
            int prev = -1;
            for(vec2 intersectionOfLine : intersectionsOfLine) {
                bool cont = false;
                for(int j = 0; j < intersections.size(); j++) {
                    float dx = abs(intersections[j].x - intersectionOfLine.x);
                    float dy = abs(intersections[j].y - intersectionOfLine.y);
                    if(dx < 0.001 &&  dy < 0.001) {
                        if(prev != -1) g.addEdge(prev, j);
                        prev = j;
                        cont = true;
                        break;
                    }
                }
                if(cont) continue;
                int index = (int)intersections.size() - 1;
                intersections.push_back(intersectionOfLine);
                if(prev != -1) g.addEdge(prev, index);
                prev = index;
            }
        }
        
        for(vec2 intersection : intersections) {
            heightmap.setHeightAt(floor(intersection.x), floor(intersection.y), heightmap.getHeightAt(floor(intersection.x), floor(intersection.y))+1);
        }
    }
    heightmap.init();
}