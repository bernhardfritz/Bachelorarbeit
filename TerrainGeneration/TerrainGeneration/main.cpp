//
//  main.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sys/time.h>
#include "ShaderManager.hpp"
#include "Camera.hpp"
#include "Keyboard.hpp"
#include "MeshLoader.hpp"
#include "Mesh.hpp"
#include "Heightmap.hpp"
#include "DirectionalLight.hpp"
#include "DiamondSquare.hpp"
#include "Fault.hpp"
#include "RMP.hpp"
#include "Cone.hpp"
#include "TextureLoader.hpp"
#include "Cube.hpp"
#include "Skybox.hpp"
#include "Voronoi.hpp"
#include "Fog.hpp"
#include "ThermalErosion.hpp"
#include "Water.hpp"
#include "OpenSimplexNoise.hpp"
#include "HydraulicErosion.hpp"
#include "Framebuffer.hpp"
#include "Quad.hpp"
#include "MousePicker.hpp"
#include "Mouse.hpp"
#include "AdvancedWater.hpp"

using namespace std;
using namespace glm;

GLFWwindow* window;

Camera camera(0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.20f);
mat4 view;
mat4 proj;
MousePicker mousePicker;
DirectionalLight light(1.24f, 1.22f);
Keyboard keyboard;
Mouse mouse;
float windowRatio = 4.0f/3.0f;
Framebuffer* passthroughFramebuffer;
Framebuffer* brightpassFramebuffer;
Framebuffer* horizontalBlurFramebuffer;
Framebuffer* verticalBlurFramebuffer;
Framebuffer* bloomFramebuffer;

int frameCount;
double elapsedSeconds;
/* we will use this function to update the window title with a frame rate */
void updateFpsCounter(GLFWwindow* window) {
    static double timer = 0.0;
    static double previousSeconds = glfwGetTime();
    double currentSeconds = glfwGetTime();
    elapsedSeconds = currentSeconds - previousSeconds;
    previousSeconds = currentSeconds;
    timer += elapsedSeconds;
    /* limit text updates to 4 per second */
    if(timer > 0.25) {
        char tmp[128];
        double fps = (double)frameCount / timer;
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frameCount = 0;
        timer = 0.0;
    }
    frameCount++;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    windowRatio = (float)width / height;
}

void window_focus_callback(GLFWwindow* window, int focused){
    if (focused) {
        // The window gained input focus
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else {
        // The window lost input focus
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    passthroughFramebuffer->resize(width, height);
    brightpassFramebuffer->resize(width, height);
    horizontalBlurFramebuffer->resize(width, height);
    verticalBlurFramebuffer->resize(width, height);
    bloomFramebuffer->resize(width, height);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static bool once1 = false;
    static bool once2 = true;
    if(mods & GLFW_MOD_SHIFT) {
        if(!once1) {
            camera.setSpeed(camera.getSpeed() * 2.0f);
            once1 = true;
            once2 = false;
        }
    } else {
        if(!once2) {
            camera.setSpeed(camera.getSpeed() / 2.0f);
            once1 = false;
            once2 = true;
        }
    }
    if(mods & GLFW_MOD_CONTROL) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    keyboard.update(key, action!=GLFW_RELEASE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos;
    static double lastY = ypos;
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        float deltaX = xpos - lastX;
        float deltaY = -(ypos - lastY); // invert
        float sensitivity = 0.01;
        camera.incYaw(deltaX * sensitivity);
        camera.incPitch(deltaY * sensitivity);
    }
    lastX = xpos;
    lastY = ypos;
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    mousePicker.update(floor(xpos), floor(ypos), width, height, proj, view);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    mouse.update(button, action==GLFW_PRESS);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    yoffset/=100.0;
    if(camera.getSpeed()-yoffset > 0 && camera.getSpeed()-yoffset < 0.25f) camera.setSpeed(camera.getSpeed()-yoffset);
}

int thetaOrPhi = -1;

void update() {
    if(keyboard.getState(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);
    if(keyboard.getState(GLFW_KEY_W)) camera.moveForward(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_A)) camera.moveLeft(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_S)) camera.moveBackward(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_D)) camera.moveRight(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_SPACE)) camera.moveUp(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_T)) thetaOrPhi = 0;
    if(keyboard.getState(GLFW_KEY_P)) thetaOrPhi = 1;
    if(keyboard.getState(GLFW_KEY_MINUS)) {
        switch(thetaOrPhi) {
            case 0: light.setTheta(light.getTheta() - radians(1.0f)); break;
            case 1: light.setPhi(light.getPhi() - radians(1.0f)); break;
        }
    }
    if(keyboard.getState(GLFW_KEY_EQUAL)) {
        switch(thetaOrPhi) {
            case 0: light.setTheta(light.getTheta() + radians(1.0f)); break;
            case 1: light.setPhi(light.getPhi() + radians(1.0f)); break;
        }
    }
}

int main() {
    srand48(time(NULL));
    // start GL context and O/S window using the GLFW helper library
    if(!glfwInit()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if(!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // nowireframe
    
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
    
    /* OTHER STUFF GOES HERE NEXT */
    TextureLoader tl;
    Texture t0 = tl.loadTexture("sand2.png");
    t0.assignToSlot(0);
    Texture t1 = tl.loadTexture("grass2.png");
    t1.assignToSlot(1);
    Texture t2 = tl.loadTexture("rock2.png");
    t2.assignToSlot(2);
    Texture t3 = tl.loadTexture("snow.png");
    t3.assignToSlot(3);
    Texture t4 = tl.loadTexture("water3.png");
    t4.assignToSlot(4);
    //MeshLoader ml;
    //Mesh obj = ml.loadMesh("bunny.obj");
    //obj.getMaterial()->setDiffuseReflectance(vec3(1.0f, 0.0f, 0.0f));
    //obj.getMaterial()->setSpecularReflectance(vec3(0.0f, 0.0f, 1.0f));
    //obj.getMaterial()->setShininess(1.0f);
    Heightmap heightmap(256, 256);
    OpenSimplexNoise osn;
    for(int row = 0; row <= heightmap.getRows(); row++) {
        for(int column = 0; column <= heightmap.getColumns(); column++) {
            heightmap.setHeightAt(column, row, 0.001f*osn.eval(column*0.1, row*0.1));
        }
    }
    heightmap.calculateNormals();
    
    heightmap.loadHeightmap("terrain.png", 0.2f);
    //hm.getMaterial()->setSpecularReflectance(0.0f);
    //Heightmap hm(128, 128);
    //Fault::perform(hm, 1.0f, 1024);
    //DiamondSquare::perform(hm, 0.2f);
    //RMP::perform(hm, 50, 3, 1);
    //RMP::perform(hm, 100);
    //ThermalErosion::perform(hm, 2.0f, 0.001f, 1000);
    vector<Mesh*> meshes;
    meshes.push_back(&heightmap);
    
    /*for(int i = 0; i < 100; i++) {
        Cone tmp(100.0f, 50.0f, 64);
        tmp.translate(drand48()*100.0f, 10.0f, drand48()*100.0f);
        meshes.push_back(tmp);
    }*/
    
    camera.setY(heightmap.getMaxHeight());
    
    Fog fog(0.67f, 1.5f);
    
    Skybox skybox;
    
    ShaderManager shaderManager("vertexshader.glsl", "fragmentshader.glsl", false);
    glUseProgram(shaderManager.getShaderProgram());
    
    //Water water(heightmap.getColumns(), heightmap.getRows(), heightmap.getAverageHeight(), 100.0f, 0.0005f);
    AdvancedWater water(heightmap);
    meshes.push_back(&water);
    
    int modelLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "model_mat");
    int viewLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "view_mat");
    int projLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "projection_mat");
    
    int lightDirection = glGetUniformLocation(shaderManager.getShaderProgram(), "light_direction");
    int lightSpecularIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "Ls");
    int lightDiffuseIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "Ld");
    int lightAmbientIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "La");
    
    int meshSpecularIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "Ks");
    int meshDiffuseIntensity = glGetUniformLocation(shaderManager.getShaderProgram(), "Kd");
    int meshAmbientReflectance = glGetUniformLocation(shaderManager.getShaderProgram(), "Ka");
    int meshShininess = glGetUniformLocation(shaderManager.getShaderProgram(), "specular_exponent");
    
    int layer0 = glGetUniformLocation(shaderManager.getShaderProgram(), "layer0");
    int layer1 = glGetUniformLocation(shaderManager.getShaderProgram(), "layer1");
    int layer2 = glGetUniformLocation(shaderManager.getShaderProgram(), "layer2");
    int layer3 = glGetUniformLocation(shaderManager.getShaderProgram(), "layer3");
    int layer4 = glGetUniformLocation(shaderManager.getShaderProgram(), "layer4");
    
    glUniform1i(layer0, 0);
    glUniform1i(layer1, 1);
    glUniform1i(layer2, 2);
    glUniform1i(layer3, 3);
    glUniform1i(layer4, 4);
    
    int fogNear = glGetUniformLocation(shaderManager.getShaderProgram(), "fog_near");
    int fogFar = glGetUniformLocation(shaderManager.getShaderProgram(), "fog_far");
    int fogColor = glGetUniformLocation(shaderManager.getShaderProgram(), "fog_color");
    
    int textured = glGetUniformLocation(shaderManager.getShaderProgram(), "textured");
    int is_water = glGetUniformLocation(shaderManager.getShaderProgram(), "is_water");
    int time = glGetUniformLocation(shaderManager.getShaderProgram(), "time");
    
    int max_height = glGetUniformLocation(shaderManager.getShaderProgram(), "max_height");
    int min_height = glGetUniformLocation(shaderManager.getShaderProgram(), "min_height");
    
    glActiveTexture (GL_TEXTURE10);
    glBindTexture (GL_TEXTURE_CUBE_MAP, skybox.get_cube_map_texture());
    int cube_texture = glGetUniformLocation(shaderManager.getShaderProgram(), "cube_texture");
    glUniform1i(cube_texture, 10);
    
    int eye = glGetUniformLocation(shaderManager.getShaderProgram(), "eye");
    
    shaderManager.validate();
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    passthroughFramebuffer = new Framebuffer(width, height);
    brightpassFramebuffer = new Framebuffer(width, height);
    horizontalBlurFramebuffer = new Framebuffer(width, height);
    verticalBlurFramebuffer = new Framebuffer(width, height);
    bloomFramebuffer = new Framebuffer(width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    Quad quad;

    while(!glfwWindowShouldClose(window)) {
        passthroughFramebuffer->bind();
        {
            updateFpsCounter(window);
        
            view = lookAt(camera.getEye(), camera.getCenter(), camera.getUp());
            proj = perspective(45.0f, windowRatio, 0.001f, 2.0f);
        
            // wipe the drawing surface clear
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            skybox.draw(value_ptr(translate(mat4(1.0f), camera.getEye())), value_ptr(view), value_ptr(proj));
            
            glUseProgram(shaderManager.getShaderProgram());
            
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(proj));
            
            glUniform3fv(eye, 1, value_ptr(camera.getEye()));
            
            glUniform3fv(lightDirection, 1, value_ptr(light.getDirection()));
            glUniform3fv(lightSpecularIntensity, 1, value_ptr(light.getSpecularIntensity()));
            glUniform3fv(lightDiffuseIntensity, 1, value_ptr(light.getDiffuseIntensity()));
            glUniform3fv(lightAmbientIntensity, 1, value_ptr(light.getAmbientIntensity()));
            
            glUniform1f(fogNear, fog.getNear());
            glUniform1f(fogFar, fog.getFar());
            glUniform3fv(fogColor, 1, value_ptr(fog.getColor()));
            
            glUniform1f(max_height, heightmap.getMaxHeight());
            glUniform1f(min_height, heightmap.getMinHeight());
            
            struct timeval tp;
            gettimeofday(&tp, NULL);
            long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            glUniform1f(time, (ms%10000)/10000.0f);
            
            for(Mesh* mesh : meshes) {
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(mesh->getModelMatrix()));
                glUniform3fv(meshSpecularIntensity, 1, value_ptr(mesh->getMaterial()->getSpecularReflectance()));
                glUniform3fv(meshDiffuseIntensity, 1, value_ptr(mesh->getMaterial()->getDiffuseReflectance()));
                glUniform3fv(meshAmbientReflectance, 1, value_ptr(mesh->getMaterial()->getAmbientReflectance()));
                glUniform1f(meshShininess, mesh->getMaterial()->getShininess());
                glUniform1i(textured, mesh->isTextured() ? 1 : 0);
                if(mesh == &water) {
                    glUniform1i(is_water, 1);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }
                else glUniform1i(is_water, 0);
                mesh->draw();
                glDisable(GL_BLEND);
            }
        }
        passthroughFramebuffer->unbind();
        
        brightpassFramebuffer->bind();
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Create and compile our GLSL program from the shaders
            static ShaderManager brightpassShaderManager("brightpass_vs.glsl","brightpass_fs.glsl");
            glUseProgram (brightpassShaderManager.getShaderProgram());
            
            glActiveTexture(GL_TEXTURE11);
            glBindTexture(GL_TEXTURE_2D, passthroughFramebuffer->getColorTexture());
            GLuint texID = glGetUniformLocation(brightpassShaderManager.getShaderProgram(), "colorTexture");
            glUniform1i(texID, 11);
            
            glActiveTexture(GL_TEXTURE12);
            glBindTexture(GL_TEXTURE_2D, passthroughFramebuffer->getDepthTexture());
            GLuint depID = glGetUniformLocation(brightpassShaderManager.getShaderProgram(), "depthTexture");
            glUniform1i(depID, 12);
            
            quad.draw();
            
        }
        brightpassFramebuffer->unbind();
        
        horizontalBlurFramebuffer->bind();
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Create and compile our GLSL program from the shaders
            static ShaderManager horizontalBlurShaderManager("gaussianblur_vs.glsl","gaussianblur_fs.glsl");
            glUseProgram (horizontalBlurShaderManager.getShaderProgram());
            
            glActiveTexture(GL_TEXTURE11);
            glBindTexture(GL_TEXTURE_2D, brightpassFramebuffer->getColorTexture());
            GLuint texID = glGetUniformLocation(horizontalBlurShaderManager.getShaderProgram(), "colorTexture");
            glUniform1i(texID, 11);
            
            glActiveTexture(GL_TEXTURE12);
            glBindTexture(GL_TEXTURE_2D, passthroughFramebuffer->getDepthTexture());
            GLuint depID = glGetUniformLocation(horizontalBlurShaderManager.getShaderProgram(), "depthTexture");
            glUniform1i(depID, 12);
            
            GLuint dirID = glGetUniformLocation(horizontalBlurShaderManager.getShaderProgram(), "direction");
            glUniform2fv(dirID, 1, value_ptr(vec2(1.0f, 0.0f)));
            
            quad.draw();
        }
        horizontalBlurFramebuffer->unbind();
        
        verticalBlurFramebuffer->bind();
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Create and compile our GLSL program from the shaders
            static ShaderManager verticalBlurShaderManager("gaussianblur_vs.glsl","gaussianblur_fs.glsl");
            glUseProgram (verticalBlurShaderManager.getShaderProgram());
            
            glActiveTexture(GL_TEXTURE11);
            glBindTexture(GL_TEXTURE_2D, horizontalBlurFramebuffer->getColorTexture());
            GLuint texID = glGetUniformLocation(verticalBlurShaderManager.getShaderProgram(), "colorTexture");
            glUniform1i(texID, 11);
            
            glActiveTexture(GL_TEXTURE12);
            glBindTexture(GL_TEXTURE_2D, passthroughFramebuffer->getDepthTexture());
            GLuint depID = glGetUniformLocation(verticalBlurShaderManager.getShaderProgram(), "depthTexture");
            glUniform1i(depID, 12);
            
            GLuint dirID = glGetUniformLocation(verticalBlurShaderManager.getShaderProgram(), "direction");
            glUniform2fv(dirID, 1, value_ptr(vec2(0.0f, 1.0f)));
            
            quad.draw();
        }
        verticalBlurFramebuffer->unbind();
        
        // Create and compile our GLSL program from the shaders
        static ShaderManager bloomShaderManager("bloom_vs.glsl","bloom_fs.glsl");
        glUseProgram (bloomShaderManager.getShaderProgram());
        
        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, passthroughFramebuffer->getColorTexture());
        GLuint texID = glGetUniformLocation(bloomShaderManager.getShaderProgram(), "colorTexture");
        glUniform1i(texID, 11);
        
        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, passthroughFramebuffer->getDepthTexture());
        GLuint depID = glGetUniformLocation(bloomShaderManager.getShaderProgram(), "depthTexture");
        glUniform1i(depID, 12);
        
        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, verticalBlurFramebuffer->getColorTexture());
        GLuint brightID = glGetUniformLocation(bloomShaderManager.getShaderProgram(), "brightTexture");
        glUniform1i(brightID, 13);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad.draw();
        
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
        
        // update other events like input handling
        glfwPollEvents();
        
        update();
        
        //water.setWaveLevel(heightmap.getAverageHeight());
        water.step(elapsedSeconds);
        
        if(mouse.getState(GLFW_MOUSE_BUTTON_1)) {
            vec3 intersection = mousePicker.getIntersection(camera, heightmap);
            int column = round(intersection.x * heightmap.getColumns());
            int row = round(intersection.z * heightmap.getRows());
            //heightmap.setHeightAt(column, row, heightmap.getHeightAt(column, row)+0.01);
            //heightmap.calculateNormals();
            RMP::perform(heightmap, column, row, 10, 0.005f, 1);
            //        printf("%s\n", mousePicker.triangle_intersects(camera, heightmap) ? "true" : "false");
            
        }
        
        if(keyboard.getState(GLFW_KEY_R)) {
            RMP::perform(heightmap, 1);
        }
        
        if(keyboard.getState(GLFW_KEY_E)) {
            HydraulicErosion::perform(heightmap, 100);
            ThermalErosion::perform(heightmap, 50);
        }
        if(keyboard.getState(GLFW_KEY_T)) {
            ThermalErosion::perform(heightmap, 100);
        }
        if(keyboard.getState(GLFW_KEY_H)) {
            HydraulicErosion::perform(heightmap, 100);
        }
    }
    
    delete passthroughFramebuffer;
    delete brightpassFramebuffer;
    delete horizontalBlurFramebuffer;
    delete verticalBlurFramebuffer;
    delete bloomFramebuffer;
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    
    return 0;
}
