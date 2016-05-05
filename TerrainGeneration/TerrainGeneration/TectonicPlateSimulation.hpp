//
//  TectonicPlateSimulation.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/05/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef TectonicPlateSimulation_hpp
#define TectonicPlateSimulation_hpp

#include <stdio.h>
#include <vector>
#include <Box2D/Box2D.h>
#include "Heightmap.hpp"

class TectonicPlateSimulation {
private:
    b2Vec2 gravity;
    b2World world;
    b2Body* kinematicBodies[4];
    int32 bodyIgnore = 0;
    int32 bodyConsider = 1;
    
    void Cut(b2Vec2 from, b2Vec2 to);
    void randomCut(int n);
    
public:
    TectonicPlateSimulation(Heightmap& heightmap);
    void Step();
};

class MyRayCastCallback : public b2RayCastCallback
{
public:
    int m_count;
    std::vector<b2Vec2> m_points;
    std::vector<b2Vec2> m_normals;
    std::vector<b2Body*> m_bodies;
    std::vector<b2PolygonShape*> m_polygonShapes;
    
    MyRayCastCallback() : m_count(0) {}
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
};

class MyContactListener : public b2ContactListener
{
private:
    Heightmap& heightmap;
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void line(int x0, int y0, int x1, int y1, float delta);
    
public:
    MyContactListener(Heightmap& heightmap) : heightmap(heightmap) {}
};

#endif /* TectonicPlateSimulation_hpp */
