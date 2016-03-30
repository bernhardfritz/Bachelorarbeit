//
//  ofxKMarchingCubes.h
//  emptyExample
//
//  Created by Kamen Dimitrov on 12/8/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef emptyExample_ofxKMarchingCubes_h
#define emptyExample_ofxKMarchingCubes_h

#include <vector>
#include <glm/glm.hpp>
#include "Metaball.hpp"

#define kGridResolution 20
#define kGridSizeX 50
#define kGridSizeY 50
#define kGridSizeZ 50

using namespace std;
using namespace glm;

class ofxKMarchingCubes {
public:

    
    vector<vector<vector<float > > > gridIsoValues_; 
    vector<vector<vector<vec3> > > gridPoints_;
     
    float gridSizeX_;
    float gridSizeY_;
    float gridSizeZ_;
    float gridResolution_;
    
    
    ofxKMarchingCubes();
    void initGrid();
    void drawGridInPoints();
    void drawGridTriangles(Metaball& metaball);
    void zeroGrid();
    
    // misc functions
    int getNumTriangles();
    
    // the heart of Marching Cubes
    void update(float threshold);
    void marchingCubes(int cubeX,int cubeY,int cubeZ, float threshold);
    void vertexInterpolation(float threshold, vec3 p1, vec3 p2, float valp1, float valp2, vec3& vertex);
    void v4Interpolation(float threshold,vec4 v1, vec4 v2,vec4& vertex);
    //Iso Values
    void addMetaBall(vec3 center, float charge);
    
    vec3 getCubeVertex(int num, int i, int j, int k);
    float getCubeIsoValue(int num, int i, int j, int k);
    
    void getVec4(int index, int i, int j, int k,vec4 &v);
    void getGradient(int index, int i, int j, int k,vec4 &v);
private:

    vec3 vertList[12];
    vec4 gradients[12];
    vec3 gradList[12];
    vector<vec3> vertices;
    vector<vec3> colors;
    vector<vec3> normals;
    int numTriangles;
    
    
};


#endif
