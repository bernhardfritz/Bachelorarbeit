//
//  Voronoi.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 18/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Voronoi.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Voronoi::Voronoi(int n) {
    float radius = 2.0f/n;
    float sqrt2 = sqrt(2.0f);
    for(float z = -1.0f; z < 1.0f; z+=radius) {
        for(float x = -1.0f; x < 1.0f; x+=radius) {
            Cone cone(radius * sqrt2, 1.0f, 64);
            cone.rotate(-half_pi<float>(), 0.0f, 0.0f);
            cone.translate(x + drand48() * radius, z + drand48() * radius, 0.0f);
            cones.push_back(cone);
        }
    }
}

Voronoi::~Voronoi() {
    if(buffer != NULL) free(buffer);
}

void Voronoi::draw() {
    static ShaderManager shaderManager("voronoi_vs.glsl", "voronoi_fs.glsl");
    glFrontFace(GL_CW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderManager.getShaderProgram());
    int meshDiffuseIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "Kd");
    for(Cone cone : cones) {
        glUniform3fv(meshDiffuseIntensity, 1, value_ptr(cone.getMaterial()->getDiffuseReflectance()));
        cone.draw();
    }
    glFrontFace(GL_CCW);
}


bool Voronoi::isPixelInRegion(int px, int py, int rx, int ry) {
    if(once) {
        buffer = (unsigned char*)malloc (640 * 480 * 3);
        glReadPixels (0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        once = false;
    }
    unsigned char pr = buffer[py*640*3 + px*3];
    unsigned char pg = buffer[py*640*3 + px*3 + 1];
    unsigned char pb = buffer[py*640*3 + px*3 + 2];
    unsigned char rr = buffer[ry*640*3 + rx*3];
    unsigned char rg = buffer[ry*640*3 + rx*3 + 1];
    unsigned char rb = buffer[ry*640*3 + rx*3 + 2];
    return pr == rr && pg == rg && pb == rb;
}

bool Voronoi::isPositionInRegion(int pcolumn, int prow, int rcolumn, int rrow, int columns, int rows) {
    return isPixelInRegion(((float)pcolumn/columns)*639, ((float)prow/rows)*479, ((float)rcolumn/columns)*639, ((float)rrow/rows)*479);
}