//
//  ofxKMarchingCubes.cpp
//  emptyExample
//
//  Created by Kamen Dimitrov on 12/8/11.
//  Based on: http://paulbourke.net/geometry/polygonise/
//



#include "ofxKMarchingCubes.h"
#include "ofxKMarchingCubesTables.h"



ofxKMarchingCubes::ofxKMarchingCubes() {
    gridSizeX_ = kGridSizeX;
    gridSizeY_ = kGridSizeY;
    gridSizeZ_ = kGridSizeZ;
    gridResolution_ = kGridResolution;
}

vec3 ofxKMarchingCubes::getCubeVertex(int num, int i, int j, int k) {
    return vec3();
}

float ofxKMarchingCubes::getCubeIsoValue(int num, int i, int j, int k) {
    return 0.0f;
}

//----------------------------------
void ofxKMarchingCubes::initGrid() {
    for(int x = 0; x < gridSizeX_; x++) {
        vector<vector<float> > yv;
        vector<vector<vec3> > yp;
        for(int y = 0; y < gridSizeY_; y++) {
            vector<float> zv;
            vector<vec3> zp;
            for(int z = 0; z < gridSizeZ_; z++) {
                zv.push_back(0); 
                zp.push_back(vec3(x*gridResolution_,y*gridResolution_,z*gridResolution_));
            }
            yv.push_back(zv);
            yp.push_back(zp);
        }
        gridIsoValues_.push_back(yv);
        gridPoints_.push_back(yp);
    }
}
//----------------------------------
void ofxKMarchingCubes::zeroGrid() {
    normals.clear();
    vertices.clear();
    for(int x=0; x < gridSizeX_; x++) {
        for(int y=0; y < gridSizeY_; y++) {
            for(int z = 0; z < gridSizeZ_; z++) {
                gridIsoValues_[x][y][z] = 0;
                gridPoints_[x][y][z] = vec3(x*gridResolution_,y*gridResolution_,z*gridResolution_);
            }
        }
    }    
}
//----------------------------------
void ofxKMarchingCubes::drawGridInPoints() {
//    ofMesh m;
//    m.setMode(OF_PRIMITIVE_POINTS);
//    for(int x=0; x < gridSizeX_; x++) {
//        for(int y=0; y < gridSizeY_; y++) {
//            for(int z = 0; z < gridSizeZ_; z++) {
//                if(gridIsoValues_[x][y][z] > 3) {
//                m.addVertex(gridPoints_[x][y][z]);
//                float c= 255 - gridIsoValues_[x][y][z]*255;
//                m.addColor(ofColor(c,c,c,c));
//                }
//            }
//        }
//    }
//    m.drawWireframe();
}

void ofxKMarchingCubes::drawGridTriangles(Metaball& metaball) {
    vector<unsigned int> indices;
    for(int i=0; i < numTriangles; i++) {
        indices.push_back(i*3);
        indices.push_back(i*3+1);
        indices.push_back(i*3+2);
    }
    
    printf("numTriangles %d\n", numTriangles);
    
    metaball.setVertices(vertices);
    metaball.setIndices(indices);
    metaball.setNormals(normals);
    metaball.update();
    
   /*
    ofSetColor(255, 255, 255);
    ofMesh nm;
    nm.setMode(OF_PRIMITIVE_LINES);
    for(int i=0;i < numTriangles; i++) {
        nm.addVertex(vertices[i*3]);
        nm.addVertex(vertices[i*3]+normals[i*3]*40);
        nm.addVertex(vertices[i*3+1]);
        nm.addVertex(vertices[i*3+1]+normals[i*3+1]*40);
        nm.addVertex(vertices[i*3+2]);
        nm.addVertex(vertices[i*3+2]+normals[i*3+2]*40);
        

    }
    nm.drawWireframe();
  */   
}
//----------------------------------
void ofxKMarchingCubes::addMetaBall(vec3 center, float charge) {
    vec3 diff;
    for(int x=0; x < gridSizeX_; x++) {
        for(int y=0; y < gridSizeY_; y++) {
            for(int z = 0; z < gridSizeZ_; z++) {
                diff = gridPoints_[x][y][z] - center;
                gridIsoValues_[x][y][z] += charge / sqrtf(pow<float>(diff.x,2) + pow<float>(diff.y,2) + pow<float>(diff.z,2));
                
            }
        }
    }    
}
//----------------------------------
void ofxKMarchingCubes::update(float threshold) {
    numTriangles = 0;
    vertices.clear();
    for(int x=0; x < gridSizeX_-1; x++) {
        for(int y=0; y < gridSizeY_-1; y++) {
            for(int z = 0; z < gridSizeZ_-1; z++) {
                marchingCubes(x, y, z, threshold);
            }
        }
    }     
}
//----------------------------------
int ofxKMarchingCubes::getNumTriangles() {
    return numTriangles;
}

void ofxKMarchingCubes::getVec4(int index, int i, int j, int k,vec4 &v) {
    switch(index) {
        case 0:
            v.x = gridPoints_[i][j][k].x;
            v.y = gridPoints_[i][j][k].y;
            v.z = gridPoints_[i][j][k].z;
            v.a = gridIsoValues_[i][j][k];
        break;
        case 1:
            v.x = gridPoints_[i+1][j][k].x;
            v.y = gridPoints_[i+1][j][k].y;
            v.z = gridPoints_[i+1][j][k].z;
            v.a = gridIsoValues_[i+1][j][k];
            break;
        case 2:
            v.x = gridPoints_[i+1][j][k+1].x;
            v.y = gridPoints_[i+1][j][k+1].y;
            v.z = gridPoints_[i+1][j][k+1].z;
            v.a = gridIsoValues_[i+1][j][k+1];
            break;
        case 3:
            v.x = gridPoints_[i][j][k+1].x;
            v.y = gridPoints_[i][j][k+1].y;
            v.z = gridPoints_[i][j][k+1].z;
            v.a = gridIsoValues_[i][j][k+1];
            break;
        case 4:
            v.x = gridPoints_[i][j+1][k].x;
            v.y = gridPoints_[i][j+1][k].y;
            v.z = gridPoints_[i][j+1][k].z;
            v.a = gridIsoValues_[i][j+1][k];
            break;
        case 5:
            v.x = gridPoints_[i+1][j+1][k].x;
            v.y = gridPoints_[i+1][j+1][k].y;
            v.z = gridPoints_[i+1][j+1][k].z;
            v.a = gridIsoValues_[i+1][j+1][k];
            break;
        case 6:
            v.x = gridPoints_[i+1][j+1][k+1].x;
            v.y = gridPoints_[i+1][j+1][k+1].y;
            v.z = gridPoints_[i+1][j+1][k+1].z;
            v.a = gridIsoValues_[i+1][j+1][k+1];
            break;
        case 7:
            v.x = gridPoints_[i][j+1][k+1].x;
            v.y = gridPoints_[i][j+1][k+1].y;
            v.z = gridPoints_[i][j+1][k+1].z;
            v.a = gridIsoValues_[i][j+1][k+1];
            break;
        
    }
}

void ofxKMarchingCubes::getGradient(int index, int i, int j, int k,vec4 &v) {
   
    switch(index) {
        case 0:
            if(i != 0 && j != 0 && k != 0) { 
                v.x = gridIsoValues_[i-1][j][k] - gridIsoValues_[i+1][j][k];
                v.y = gridIsoValues_[i][j-1][k] - gridIsoValues_[i][j+1][k];
                v.z = gridIsoValues_[i][j][k-1] - gridIsoValues_[i][j][k+1];
                v.a = gridIsoValues_[i][j][k];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
            break;
            
        case 1:
            if(i < gridSizeX_-2 && j != 0 && k != 0) { 
                v.x = gridIsoValues_[i+1-1][j][k] - gridIsoValues_[i+1+1][j][k];
                v.y = gridIsoValues_[i+1][j-1][k] - gridIsoValues_[i+1][j+1][k];
                v.z = gridIsoValues_[i+1][j][k-1] - gridIsoValues_[i+1][j][k+1];
                v.a = gridIsoValues_[i+1][j][k];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
     
            break;
        case 2:
            if(i < gridSizeX_-2 && j != 0 && k < gridSizeZ_-2) { 
                v.x = gridIsoValues_[i+1-1][j][k+1] - gridIsoValues_[i+1+1][j][k+1];
                v.y = gridIsoValues_[i+1][j-1][k+1] - gridIsoValues_[i+1][j+1][k+1];
                v.z = gridIsoValues_[i+1][j][k+1-1] - gridIsoValues_[i+1][j][k+1+1];
                v.a = gridIsoValues_[i+1][j][k+1];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
           
            break;
        case 3:
            if(i != 0 && j != 0 && k < gridSizeZ_-2) { 
                v.x = gridIsoValues_[i-1][j][k+1] - gridIsoValues_[i+1][j][k+1];
                v.y = gridIsoValues_[i][j-1][k+1] - gridIsoValues_[i][j+1][k+1];
                v.z = gridIsoValues_[i][j][k+1-1] - gridIsoValues_[i][j][k+1+1];
                v.a = gridIsoValues_[i][j][k+1];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
          
            break;
        case 4:
            if(i != 0 && j < gridSizeY_-2 && k != 0) { 
                v.x = gridIsoValues_[i-1][j+1][k] - gridIsoValues_[i+1][j+1][k];
                v.y = gridIsoValues_[i][j+1-1][k] - gridIsoValues_[i][j+1+1][k];
                v.z = gridIsoValues_[i][j+1][k-1] - gridIsoValues_[i][j+1][k+1];
                v.a = gridIsoValues_[i][j+1][k];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
          
            break;
        case 5:
            if(i < gridSizeX_-2 && j <gridSizeY_-2 && k != 0) { 
                v.x = gridIsoValues_[i+1-1][j+1][k] - gridIsoValues_[i+1+1][j+1][k];
                v.y = gridIsoValues_[i+1][j+1-1][k] - gridIsoValues_[i+1][j+1+1][k];
                v.z = gridIsoValues_[i+1][j+1][k-1] - gridIsoValues_[i+1][j+1][k+1];
                v.a = gridIsoValues_[i+1][j+1][k];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
           
            break;
        case 6:
            if(i <gridSizeX_-2 && j <gridSizeY_-2 && k <gridSizeZ_-2) { 
                v.x = gridIsoValues_[i+1-1][j+1][k+1] - gridIsoValues_[i+1+1][j+1][k+1];
                v.y = gridIsoValues_[i+1][j+1-1][k+1] - gridIsoValues_[i+1][j+1+1][k+1];
                v.z = gridIsoValues_[i+1][j+1][k+1-1] - gridIsoValues_[i+1][j+1][k+1+1];
                v.a = gridIsoValues_[i+1][j+1][k+1];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
          
            break;
        case 7:
            if(i != 0 && j <gridSizeY_-2 && k < gridSizeZ_-2) { 
                v.x = gridIsoValues_[i-1][j+1][k+1] - gridIsoValues_[i+1][j+1][k+1];
                v.y = gridIsoValues_[i][j+1-1][k+1] - gridIsoValues_[i][j+1+1][k+1];
                v.z = gridIsoValues_[i][j+1][k+1-1] - gridIsoValues_[i][j+1][k+1+1];
                v.a = gridIsoValues_[i][j+1][k+1];
            } else {
                v.x = 0.0;
                v.y = 0.0;
                v.z = 0.0;
                v.a = 1.0;
            }
          
            break;
            
    }
}

//----------------------------------
void ofxKMarchingCubes::marchingCubes(int cubeX,int cubeY,int cubeZ,float threshold) {
    int cubeindex = 0;
    
    int i = cubeX;
    int j = cubeY;
    int k = cubeZ;
    

    vec4 vv[8];
    
    for(int ind=0;ind<8;ind++) {
        getVec4(ind, i, j, k,vv[ind]);
     
    }
    
    
    // check which vertices are inside the surface
    if(vv[0].a < threshold) cubeindex |= 1;
    if(vv[1].a < threshold) cubeindex |= 2;
    if(vv[2].a < threshold) cubeindex |= 4;
    if(vv[3].a < threshold) cubeindex |= 8;
    if(vv[4].a < threshold) cubeindex |= 16;
    if(vv[5].a < threshold) cubeindex |= 32;
    if(vv[6].a < threshold) cubeindex |= 64;
    if(vv[7].a < threshold) cubeindex |= 128;
    
   // ofLog() << cubeindex << endl;
    
    // cube is entirely out of the surface 
    if(edgeTable[cubeindex] == 0)
        return;
    
    /*
    // draw grid
    ofSetColor(100,100,100);
    ofLine(vv[0].vector,vv[1].vector);
    ofLine(vv[1].vector,vv[2].vector);
    ofLine(vv[2].vector,vv[3].vector);
    ofLine(vv[3].vector,vv[0].vector);
    ofLine(vv[4].vector,vv[5].vector);
    ofLine(vv[5].vector,vv[6].vector);
    ofLine(vv[6].vector,vv[7].vector);
    ofLine(vv[7].vector,vv[4].vector);
    ofLine(vv[5].vector,vv[1].vector);
    ofLine(vv[6].vector,vv[2].vector);
    ofLine(vv[7].vector,vv[3].vector);
    ofLine(vv[4].vector,vv[0].vector);
   
    */
    
    
    /* Find the vertices where the surface intersects the cube */
    
    // intersects with edge 0
    if(edgeTable[cubeindex] & 1) {
        vertexInterpolation(threshold, vec3(vv[0]), vec3(vv[1]), vv[0].a, vv[1].a, vertList[0]);
        
        getGradient(0, i, j, k, gradients[0]);
        getGradient(1, i, j, k, gradients[1]);
               
        vertexInterpolation(threshold, vec3(gradients[0]), vec3(gradients[1])
                            , gradients[0].a, gradients[1].a, gradList[0]);
    }
    
    // intersects with edge 1
    if(edgeTable[cubeindex] & 2) {
        vertexInterpolation(threshold, vec3(vv[1]), vec3(vv[2]), vv[1].a, vv[2].a, vertList[1]);
        
        getGradient(1, i, j, k, gradients[1]);
        getGradient(2, i, j, k, gradients[2]);
        
        vertexInterpolation(threshold, vec3(gradients[1]), vec3(gradients[2])
                            , gradients[1].a, gradients[2].a, gradList[1]);
    
     }

    // intersects with edge 2
    if(edgeTable[cubeindex] & 4) {
        vertexInterpolation(threshold, vec3(vv[2]), vec3(vv[3]), vv[2].a, vv[3].a, vertList[2]);
        
        getGradient(2, i, j, k, gradients[2]);
        getGradient(3, i, j, k, gradients[3]);
        
        vertexInterpolation(threshold, vec3(gradients[2]), vec3(gradients[3])
                            , gradients[2].a, gradients[3].a, gradList[2]);
    }

    // intersects with edge 3
    if(edgeTable[cubeindex] & 8) {
        
        vertexInterpolation(threshold, vec3(vv[3]), vec3(vv[0]), vv[3].a, vv[0].a, vertList[3]);
        
        getGradient(3, i, j, k, gradients[3]);
        getGradient(0, i, j, k, gradients[0]);
        
        vertexInterpolation(threshold, vec3(gradients[3]), vec3(gradients[0])
                            , gradients[3].a, gradients[0].a, gradList[3]);
    }

    // intersects with edge 4
    if(edgeTable[cubeindex] & 16) {
        vertexInterpolation(threshold, vec3(vv[4]), vec3(vv[5]), vv[4].a, vv[5].a, vertList[4]);
        
        getGradient(4, i, j, k, gradients[4]);
        getGradient(5, i, j, k, gradients[5]);
        
        vertexInterpolation(threshold, vec3(gradients[4]), vec3(gradients[5])
                            , gradients[4].a, gradients[5].a, gradList[4]);
    }

    // intersects with edge 5
    if(edgeTable[cubeindex] & 32) {
        vertexInterpolation(threshold, vec3(vv[5]), vec3(vv[6]), vv[5].a, vv[6].a, vertList[5]);
        
        getGradient(5, i, j, k, gradients[5]);
        getGradient(6, i, j, k, gradients[6]);
        
        vertexInterpolation(threshold, vec3(gradients[5]), vec3(gradients[6])
                            , gradients[5].a, gradients[6].a, gradList[5]);
    }

    // intersects with edge 6
    if(edgeTable[cubeindex] & 64) {
        vertexInterpolation(threshold, vec3(vv[6]), vec3(vv[7]), vv[6].a, vv[7].a, vertList[6]);
        
        getGradient(6, i, j, k, gradients[6]);
        getGradient(7, i, j, k, gradients[7]);
        
        vertexInterpolation(threshold, vec3(gradients[6]), vec3(gradients[7])
                            , gradients[6].a, gradients[7].a, gradList[6]);
    }

    // intersects with edge 7
    if(edgeTable[cubeindex] & 128) {
        vertexInterpolation(threshold, vec3(vv[7]), vec3(vv[4]), vv[7].a, vv[4].a, vertList[7]);
        
        getGradient(7, i, j, k, gradients[7]);
        getGradient(4, i, j, k, gradients[4]);
        
        vertexInterpolation(threshold, vec3(gradients[7]), vec3(gradients[4])
                            , gradients[7].a, gradients[4].a, gradList[7]);
    }

    // intersects with edge 8
    if(edgeTable[cubeindex] & 256) {
        vertexInterpolation(threshold, vec3(vv[0]), vec3(vv[4]), vv[0].a, vv[4].a, vertList[8]);
        
        getGradient(0, i, j, k, gradients[0]);
        getGradient(4, i, j, k, gradients[4]);
        
        vertexInterpolation(threshold, vec3(gradients[0]), vec3(gradients[4])
                            , gradients[0].a, gradients[4].a, gradList[8]);
    }

    // intersects with edge 9
    if(edgeTable[cubeindex] & 512) {
        vertexInterpolation(threshold, vec3(vv[1]), vec3(vv[5]), vv[1].a, vv[5].a, vertList[9]);
        
        getGradient(1, i, j, k, gradients[1]);
        getGradient(5, i, j, k, gradients[5]);
        
        vertexInterpolation(threshold, vec3(gradients[1]), vec3(gradients[5])
                            , gradients[1].a, gradients[5].a, gradList[9]);
    }

    // intersects with edge 10
    if(edgeTable[cubeindex] & 1024) {
        vertexInterpolation(threshold, vec3(vv[2]), vec3(vv[6]), vv[2].a, vv[6].a, vertList[10]);
        
        getGradient(2, i, j, k, gradients[2]);
        getGradient(6, i, j, k, gradients[6]);
        
        vertexInterpolation(threshold, vec3(gradients[2]), vec3(gradients[6])
                            , gradients[2].a, gradients[6].a, gradList[10]);
    }

    // intersects with edge 11
    if(edgeTable[cubeindex] & 2048) {
        vertexInterpolation(threshold, vec3(vv[3]), vec3(vv[7]), vv[3].a, vv[7].a, vertList[11]);
        
        getGradient(3, i, j, k, gradients[3]);
        getGradient(7, i, j, k, gradients[7]);
        
        vertexInterpolation(threshold, vec3(gradients[3]), vec3(gradients[7])
                            , gradients[3].a, gradients[7].a, gradList[11]);
    }
    
    /* create triangles */
    
  
    for(int ind=0;triTable[cubeindex][ind] != -1;ind+=3) {
        
        vertices.push_back(vertList[triTable[cubeindex][ind]]);
        vertices.push_back(vertList[triTable[cubeindex][ind+1]]);
        vertices.push_back(vertList[triTable[cubeindex][ind+2]]);
        
        normals.push_back(gradList[triTable[cubeindex][ind]]);
        normals.push_back(gradList[triTable[cubeindex][ind+1]]);
        normals.push_back(gradList[triTable[cubeindex][ind+2]]);
        
        numTriangles++;
    }
  
     
}

void ofxKMarchingCubes::vertexInterpolation(float threshold,
                                            vec3 p1, vec3 p2,
                                            float valp1, float valp2, 
                                            vec3& vertex)
{
    float mu;
    
  //  this is case where the resolution is too low anyway
    if (abs<float>(threshold-valp1) < 0.00001){
        vertex.x = p1.x;
        vertex.y = p1.y;
        vertex.z = p1.z;
		return;
	}
	if (abs<float>(threshold-valp2) < 0.00001){
        vertex.x = p2.x;
        vertex.y = p2.y;
        vertex.z = p2.z;
		return;
	}
	if (abs<float>(valp1-valp2) < 0.00001){
        vertex.x = p1.x;
        vertex.y = p1.x;
        vertex.z = p1.z;
		return;
	}
     
    
	mu = (threshold - valp1) / (valp2 - valp1);
    
	vertex.x = p1.x + mu * (p2.x - p1.x);
	vertex.y = p1.y + mu * (p2.y - p1.y);
	vertex.z = p1.z + mu * (p2.z - p1.z);    
}

void ofxKMarchingCubes::v4Interpolation(float threshold, 
                                            vec4 v1, vec4 v2, 
                                            vec4& vertex) 
{
    float mu;
    /*
     this is case where the resolution is too low anyway
     if (ABS(threshold-valp1) < 0.00001){
     vertex.set(p1.x, p1.y, p1.z);
     return;
     }
     if (ABS(threshold-valp2) < 0.00001){
     vertex.set(p2.x, p2.y, p2.z);
     return;
     }
     if (ABS(valp1-valp2) < 0.00001){
     vertex.set(p1.x, p1.x, p1.z);
     return;
     }
     */
    
	mu = (threshold - v1.a) / (v2.a - v1.a);
    
	vertex.x = v1.x + mu * (v2.x - v1.x);
	vertex.y = v1.y + mu * (v2.y - v1.y);
	vertex.z = v1.z + mu * (v2.z - v1.z);
}







