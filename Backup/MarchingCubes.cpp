//
//  MarchingCubes.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 29/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "MarchingCubes.hpp"

//fGetOffset finds the approximate point of intersection of the surface
// between two points with the values fValue1 and fValue2
float MarchingCubes::fGetOffset(float fValue1, float fValue2, float fValueDesired)
{
    double fDelta = fValue2 - fValue1;
    
    if(fDelta == 0.0)
    {
        return 0.5;
    }
    return (fValueDesired - fValue1)/fDelta;
}

void MarchingCubes::vNormalizeVector(vec3 &rfVectorResult, vec3 &rfVectorSource)
{
    float fOldLength;
    float fScale;
    
    fOldLength = sqrtf( (rfVectorSource.x * rfVectorSource.x) +
                       (rfVectorSource.y * rfVectorSource.y) +
                       (rfVectorSource.z * rfVectorSource.z) );
    
    if(fOldLength == 0.0)
    {
        rfVectorResult.x = rfVectorSource.x;
        rfVectorResult.y = rfVectorSource.y;
        rfVectorResult.z = rfVectorSource.z;
    }
    else
    {
        fScale = 1.0/fOldLength;
        rfVectorResult.x = rfVectorSource.x*fScale;
        rfVectorResult.y = rfVectorSource.y*fScale;
        rfVectorResult.z = rfVectorSource.z*fScale;
    }
}

//fSample1 finds the distance of (fX, fY, fZ) from three moving points
float MarchingCubes::fSample1(float fX, float fY, float fZ, vector<vec3>& positions, float radius)
{
    double fResult = 0.0;
    double fDx, fDy, fDz;
    
    for(vec3 position : positions) {
        fDx = fX - position.x;
        fDy = fY - position.y;
        fDz = fZ - position.z;
        fResult += radius/(fDx*fDx + fDy*fDy + fDz*fDz);
    }
    
    return fResult;
}

//vGetNormal() finds the gradient of the scalar field at a point
//This gradient can be used as a very accurate vertx normal for lighting calculations
void MarchingCubes::vGetNormal(vec3 &rfNormal, float fX, float fY, float fZ, vector<vec3>& positions, float radius)
{
    rfNormal.x = fSample1(fX-0.01, fY, fZ, positions, radius) - fSample1(fX+0.01, fY, fZ, positions, radius);
    rfNormal.y = fSample1(fX, fY-0.01, fZ, positions, radius) - fSample1(fX, fY+0.01, fZ, positions, radius);
    rfNormal.z = fSample1(fX, fY, fZ-0.01, positions, radius) - fSample1(fX, fY, fZ+0.01, positions, radius);
    vNormalizeVector(rfNormal, rfNormal);
}

//vMarchCube1 performs the Marching Cubes algorithm on a single cube
void MarchingCubes::vMarchCube1(float fX, float fY, float fZ, float fScale, vector<vec3>& positions, float radius, vector<vec3>& vertices, vector<unsigned int>& indices, vector<vec3>& normals)
{
    int iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;
    float fOffset;
    vec3 sColor;
    float afCubeValue[8];
    vec3 asEdgeVertex[12];
    vec3 asEdgeNorm[12];
    
    //Make a local copy of the values at the cube's corners
    for(iVertex = 0; iVertex < 8; iVertex++)
    {
        afCubeValue[iVertex] = fSample1(fX + a2fVertexOffset[iVertex][0]*fScale,
                                       fY + a2fVertexOffset[iVertex][1]*fScale,
                                       fZ + a2fVertexOffset[iVertex][2]*fScale, positions, radius);
    }
    
    //Find which vertices are inside of the surface and which are outside
    iFlagIndex = 0;
    for(iVertexTest = 0; iVertexTest < 8; iVertexTest++)
    {
        if(afCubeValue[iVertexTest] <= fTargetValue)
            iFlagIndex |= 1<<iVertexTest;
    }
    
    //Find which edges are intersected by the surface
    iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];
    
    //If the cube is entirely inside or outside of the surface, then there will be no intersections
    if(iEdgeFlags == 0)
    {
        return;
    }
    
    //Find the point of intersection of the surface with each edge
    //Then find the normal to the surface at those points
    for(iEdge = 0; iEdge < 12; iEdge++)
    {
        //if there is an intersection on this edge
        if(iEdgeFlags & (1<<iEdge))
        {
            fOffset = fGetOffset(afCubeValue[ a2iEdgeConnection[iEdge][0] ],
                                 afCubeValue[ a2iEdgeConnection[iEdge][1] ], fTargetValue);
            
            asEdgeVertex[iEdge].x = fX + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][0]  +  fOffset * a2fEdgeDirection[iEdge][0]) * fScale;
            asEdgeVertex[iEdge].y = fY + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][1]  +  fOffset * a2fEdgeDirection[iEdge][1]) * fScale;
            asEdgeVertex[iEdge].z = fZ + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][2]  +  fOffset * a2fEdgeDirection[iEdge][2]) * fScale;
            
            vGetNormal(asEdgeNorm[iEdge], asEdgeVertex[iEdge].x, asEdgeVertex[iEdge].y, asEdgeVertex[iEdge].z, positions, radius);
        }
    }
    
    
    //Draw the triangles that were found.  There can be up to five per cube
    for(iTriangle = 0; iTriangle < 5; iTriangle++)
    {
        if(a2iTriangleConnectionTable[iFlagIndex][3*iTriangle] < 0)
            break;
        
        for(iCorner = 0; iCorner < 3; iCorner++)
        {
            iVertex = a2iTriangleConnectionTable[iFlagIndex][3*iTriangle+iCorner];
            indices.push_back(iVertex);
            normals.push_back(vec3(asEdgeNorm[iVertex].x, asEdgeNorm[iVertex].y, asEdgeNorm[iVertex].z));
            vertices.push_back(vec3(asEdgeVertex[iVertex].x, asEdgeVertex[iVertex].y, asEdgeVertex[iVertex].z));
        }
    }
}

//vMarchingCubes iterates over the entire dataset, calling vMarchCube on each cube
void MarchingCubes::vMarchingCubes(vector<vec3>& positions, float radius, Metaball& metaball)
{
    vector<vec3> vertices;
    vector<unsigned int> indices;
    vector<vec3> normals;
    int iX, iY, iZ;
    for(iX = 0; iX < iDataSetSize; iX++)
    for(iY = 0; iY < iDataSetSize; iY++)
    for(iZ = 0; iZ < iDataSetSize; iZ++)
    {
        vMarchCube1(iX*fStepSize, iY*fStepSize, iZ*fStepSize, fStepSize, positions, radius, vertices, indices, normals);
    }
    metaball.setVertices(vertices);
    metaball.setIndices(indices);
    metaball.setNormals(normals);
    metaball.update();
}