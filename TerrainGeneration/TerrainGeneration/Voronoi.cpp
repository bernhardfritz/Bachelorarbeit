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
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h> // GLFW helper library
#include "Utils.hpp"

Voronoi::Voronoi(int n) : camera(Camera(0.5f, 1.0f, 0.5f, -half_pi<float>(), 0.0f, 0.0f)) {
    // stratified is not that random
    /*int gridsize = n;
    float gridlength = 1.0f / gridsize;
    float gridlengthhalf = gridlength / 2.0f;
    float sqrt2 = sqrt(2.0f);
    for(float z = 0; z < gridsize; z++) {
        for(float x = 0; x < gridsize; x++) {
            Cone* cone = new Cone(gridlength * sqrt2, 1.0f, 64);
            cone->setPosition(x * gridlength + gridlengthhalf + drand48() * gridlengthhalf, 0.0f, z * gridlength + gridlengthhalf + drand48() * gridlengthhalf);
            cones.push_back(cone);
        }
    }*/
    for(int i = 0; i < n; i++) {
        Cone* cone = new Cone(1.0f, 1.0f, 64);
        cone->setPosition(drand48(), 0.0f, drand48());
        cones.push_back(cone);
    }
    view = lookAt(camera.getEye(), camera.getCenter(), camera.getUp());
}

Voronoi::~Voronoi() {
    if(buffer != NULL) delete buffer;
    for(Cone* cone : cones) {
        delete cone;
    }
}

void Voronoi::draw() {
    static ShaderManager shaderManager("voronoi_vs.glsl", "voronoi_fs.glsl");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderManager.getShaderProgram());
    
    //mat4 proj = perspective(45.0f, windowRatio, 0.001f, 2.0f);
    mat4 proj = ortho(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f);
    
    int modelLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "model_mat");
    int viewLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "view_mat");
    int projLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "projection_mat");

    int meshDiffuseIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "Kd");
    
    for(Cone* cone : cones) {
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(cone->getModelMatrix()));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(proj));
        glUniform3fv(meshDiffuseIntensity, 1, value_ptr(cone->getMaterial()->getDiffuseReflectance()));
        cone->draw();
    }
}

bool Voronoi::isPixelInRegion(int px, int py, int rx, int ry) {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    
    if(buffer == NULL) {
        buffer = new unsigned char[width * height * 3];
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        //Utils::screenshot();
    }
    
    unsigned char pr = buffer[(py * width + px) * 3 + 0];
    unsigned char pg = buffer[(py * width + px) * 3 + 1];
    unsigned char pb = buffer[(py * width + px) * 3 + 2];
    unsigned char rr = buffer[(ry * width + rx) * 3 + 0];
    unsigned char rg = buffer[(ry * width + rx) * 3 + 1];
    unsigned char rb = buffer[(ry * width + rx) * 3 + 2];
    
    return pr == rr && pg == rg && pb == rb;
}

bool Voronoi::isPositionInRegion(int pcolumn, int prow, int rcolumn, int rrow, int columns, int rows) {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    return isPixelInRegion(((float)pcolumn/columns)*(width-1), ((float)prow/rows)*(height-1), ((float)rcolumn/columns)*(width-1), ((float)rrow/rows)*(height-1));
}