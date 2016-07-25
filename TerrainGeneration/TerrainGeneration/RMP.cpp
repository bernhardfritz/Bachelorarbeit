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
#include "Ray.hpp"
#include "Voronoi.hpp"

bool pnpoly(vec2 point, vector<vec2> polygon) {
    int nvert = (int)polygon.size();
    int i, j;
    float e = 0.001f;
    bool c = false;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if(fabs(polygon[i].y - polygon[j].y) <= e && fabs(polygon[j].y - point.y) <= e && (polygon[i].x > point.x) != (polygon[j].x >= point.x)) {
            return true;
        }
        if(polygon[i].y > point.y != polygon[j].y > point.y) {
            double d = (polygon[j].x-polygon[i].x) * (point.y-polygon[i].y) / (polygon[j].y-polygon[i].y) + polygon[i].x;
            if(fabs(point.x - d) <= e) return true;
            if(point.x < d) c = !c;
        }
    }
    return c;
}

void RMP::perform(Heightmap &heightmap, int n, int l, int r) {
    for(int i = 0; i < n; i++) {
        Graph g1;
        vector<Ray> rays;
        // add boundary lines to array
        rays.push_back(Ray(vec2(0, 0), vec2(heightmap.getColumns(), 0)));
        rays.push_back(Ray(vec2(heightmap.getColumns(), 0), vec2(heightmap.getColumns(), heightmap.getRows())));
        rays.push_back(Ray(vec2(heightmap.getColumns(), heightmap.getRows()), vec2(0, heightmap.getRows())));
        rays.push_back(Ray(vec2(0, heightmap.getRows()), vec2(0, 0)));
        // add random cutting lines to array
        for(int j = 0; j < l; j++) {
            vec2 v1(drand48() * heightmap.getColumns(), drand48() * heightmap.getRows());
            vec2 v2(drand48() * heightmap.getColumns(), drand48() * heightmap.getRows());
            rays.push_back(Ray(v1, v2));
        }
        /*rays.push_back(Ray(vec2(10,0), vec2(10,32)));
        rays.push_back(Ray(vec2(22,0), vec2(22,32)));
        rays.push_back(Ray(vec2(0,10), vec2(32,10)));
        rays.push_back(Ray(vec2(0,22), vec2(32,22)));*/
        
        vector<vec2> intersections;
        for(Ray ray1 : rays) {
            vector<vec2> intersectionsOfRay;
            for(Ray ray2 : rays) {
                if(ray1 == ray2) continue;
                vec2 intersection = ray1.getIntersection(ray2);
                if(intersection.x >= 0 && intersection.x <= heightmap.getColumns() &&
                   intersection.y >= 0 && intersection.y <= heightmap.getRows()) intersectionsOfRay.push_back(intersection);
            }
            ray1.sortIntersections(intersectionsOfRay);
            int prev = -1;
            for(vec2 intersectionOfRay : intersectionsOfRay) {
                if(intersectionOfRay.x == 10 && intersectionOfRay.y == 0) {
                    cout << "now" << endl;
                }
                bool cont = false;
                for(int j = 0; j < intersections.size(); j++) {
                    float dx = abs(intersections[j].x - intersectionOfRay.x);
                    float dy = abs(intersections[j].y - intersectionOfRay.y);
                    if(dx < 0.001 && dy < 0.001) {
                        if(prev != -1) g1.addEdge(prev, j);
                        prev = j;
                        cont = true;
                        break;
                    }
                }
                if(cont) continue;
                intersections.push_back(intersectionOfRay);
                int index = (int)intersections.size() - 1;
                if(prev != -1) g1.addEdge(prev, index);
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
        
        g1.print();
        
        cout << "DFS: ";
        vector<int> dfs1 = g1.DFS();
        for(int j = 0; j < dfs1.size(); j++) {
            cout << dfs1[j] << " ";
        }
        cout << endl;
        
        cout << "Cycles: " << endl;
        vector<vector<int>> cycles = g1.getMinimalCycles();
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
        
        /*int count = 0;
        for(vector<vec2> polygon : polygons) {
            for(int row = 0; row <= heightmap.getRows(); row++) {
                for(int column = 0; column <= heightmap.getColumns(); column++) {
                    if(isPointInPolygon(vec2(column, row), polygon)) heightmap.setHeightAt(column, row, count);
                }
            }
            count += 2;
        }*/
        
        // TODO: build graph with polygons represented as vertices. vertices are connected if polygons share edge.
        // TODO: perform probing algorithm
        
        Graph g2;
        for(int j = 0; j < cycles.size(); j++) {
            vector<int> cycle1 = cycles[j];
            for(int k = 0; k < cycles.size(); k++) {
                if(j == k) continue;
                vector<int> cycle2 = cycles[k];
                bool b = false;
                for(int l = 0; l < cycle1.size(); l++) {
                    for(int m = 0; m < cycle2.size(); m++) {
                        if((cycle1[l] == cycle2[m] && cycle1[(l+1) % cycle1.size()] == cycle2[(m+1) % cycle2.size()]) || (cycle1[l] == cycle2[(m+1) % cycle2.size()] && cycle1[(l+1) % cycle1.size()] == cycle2[m])) {
                            g2.addEdge(j, k);
                            b = true;
                            break;
                        }
                    }
                    if(b) break;
                }
            }
        }
        
        g2.print();
        
        vector<int> dfs2;
        for(int j = 0; j < polygons.size(); j++) {
            vector<vec2> polygon = polygons[j];
            if(pnpoly(vec2(heightmap.getColumns()/2, heightmap.getRows()/2), polygon)) {
                cout << "DFS: ";
                dfs2 = g2.DFS(j);
                for(int j = 0; j < dfs2.size(); j++) {
                    cout << dfs2[j] << " ";
                }
                cout << endl;
                break;
            }
        }
        
        float height = 0.01f;
        for(int j = 0; j < dfs2.size() && j < r; j++) {
            vector<vec2> polygon = polygons[dfs2[j]];
            for(int row = 0; row <= heightmap.getRows(); row++) {
                for(int column = 0; column <= heightmap.getColumns(); column++) {
                    if(pnpoly(vec2(column, row), polygon)) heightmap.setHeightAt(column, row, heightmap.getHeightAt(column, row) + height);
                }
            }
            height *= 0.9f;
        }
        
    }
    heightmap.calculateNormals();
}

void RMP::perform(Heightmap &heightmap, int n) {
    perform(heightmap, heightmap.getColumns()/2, heightmap.getRows()/2, 10, 0.005f, n);
}

void RMP::perform(Heightmap &heightmap, int x, int z, int spread, float delta, int iterations) {
    if(!(0 <= x && x <= heightmap.getColumns() && 0 <= z && z <= heightmap.getRows())) return;
    
    for(int i = 0; i < iterations; i++) {
        Voronoi voronoi(spread);
        voronoi.draw();
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                if(voronoi.isPositionInRegion(column, row, x, z, heightmap.getColumns(), heightmap.getRows())) {
                    heightmap.setHeightAt(column, row, heightmap.getHeightAt(column, row) + delta);
                }
            }
        }
        delta *= (1.0f - 1.0f/iterations);
    }
    
    heightmap.calculateNormals();
}

void RMP::perform(Heightmap &heightmap, int x0, int z0, int x1, int z1, int spread, float delta, int iterations) {
    if(!(0 <= x0 && 0 <= x1 && x0 <= heightmap.getColumns() && x1 <= heightmap.getColumns() && 0 <= z0 && 0 <= z1 && z0 <= heightmap.getRows() && z1 <= heightmap.getRows())) return;
    
    for(int i = 0; i < iterations; i++) {
        Voronoi voronoi(spread);
        voronoi.draw();
        
        int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
        int dz = -abs(z1-z0), sz = z0<z1 ? 1 : -1;
        int err = dx+dz, e2; /* error value e_xz */
        
        vector<vec3> colors;
        
        while(1){
            vec3 color = voronoi.getColorAtPosition(x0, z0, heightmap.getColumns(), heightmap.getRows());
            if (find(colors.begin(), colors.end(), color) == colors.end()) colors.push_back(color);
            if (x0==x1 && z0==z1) break;
            e2 = 2*err;
            if (e2 > dz) { err += dz; x0 += sx; } /* e_xz+e_x > 0 */
            if (e2 < dx) { err += dx; z0 += sz; } /* e_xz+e_z < 0 */
        }
        
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                for(vec3 color : colors) {
                    if(color == voronoi.getColorAtPosition(column, row, heightmap.getColumns(), heightmap.getRows())) {
                        //heightmap.setHeightAt(column, row, heightmap.getHeightAt(column, row) + delta);
                        heightmap.setHeightAt(column, row, heightmap.getHeightAt(column, row) + (glm::length(color)/(sqrtf(3.0f)*256.0f)) * delta);
                    }
                }
            }
        }
        
        delta *= (1.0f - 1.0f/iterations);
    }
    
    heightmap.calculateNormals();
 }