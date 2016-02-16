//
//  main.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
#include "ShaderManager.hpp"
#include "Camera.hpp"
#include "Keyboard.hpp"
#include "MeshLoader.hpp"
#include "Mesh.hpp"
#include "Heightmap.hpp"
#include "Light.hpp"
#include "DiamondSquare.hpp"
#include "Fault.hpp"
#include "RMP.hpp"
#include "Cone.hpp"
#include "TextureLoader.hpp"

using namespace std;
using namespace glm;

GLFWwindow* window;

Camera camera(64.0f, 64.0f, 64.0f, 0.0f, 0.0f, 0.0f);
Keyboard keyboard;

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
    keyboard.update(key, action!=GLFW_RELEASE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos;
    static double lastY = ypos;
    float deltaX = xpos - lastX;
    float deltaY = -(ypos - lastY); // invert
    float sensitivity = 0.01;
    camera.incYaw(deltaX * sensitivity);
    camera.incPitch(deltaY * sensitivity);
    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if(camera.getSpeed()-yoffset > 0) camera.setSpeed(camera.getSpeed()-yoffset);
}

void update() {
    if(keyboard.getState(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);
    if(keyboard.getState(GLFW_KEY_W)) camera.moveForward(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_A)) camera.moveLeft(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_S)) camera.moveBackward(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_D)) camera.moveRight(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_SPACE)) camera.moveUp(elapsedSeconds);
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
    
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
    
    /* OTHER STUFF GOES HERE NEXT */
    TextureLoader tl;
    Texture t0 = tl.loadTexture("aztec_grass.png");
    t0.assignToSlot(0);
    Texture t1 = tl.loadTexture("ng_aztec_dirt.png");
    t1.assignToSlot(1);
    Texture t2 = tl.loadTexture("ng_mossy_cliff.png");
    t2.assignToSlot(2);
    Texture t3 = tl.loadTexture("ng_mountain_rocks.png");
    t3.assignToSlot(3);
    //MeshLoader ml;
    //Mesh obj = ml.loadMesh("bunny.obj");
    //obj.getMaterial()->setDiffuseReflectance(vec3(1.0f, 0.0f, 0.0f));
    //obj.getMaterial()->setSpecularReflectance(vec3(0.0f, 0.0f, 1.0f));
    //obj.getMaterial()->setShininess(1.0f);
    Heightmap hm(256, 256);
    hm.loadHeightmap("heightmap.png", 32.0f);
    //hm.getMaterial()->setSpecularReflectance(0.0f);
    //Heightmap hm(64, 64);
    //DiamondSquare::perform(hm, 50.0f);
    //Fault::perform(hm, 0.5f, 256);
    //RMP::perform(hm, 50, 3, 1);
    vector<Mesh> meshes;
    meshes.push_back(hm);
    /*
    for(int i = 0; i < 100; i++) {
        Cone tmp(100.0f, 50.0f, 64);
        tmp.translate(drand48()*100.0f, 0.0f, drand48()*100.0f);
        meshes.push_back(tmp);
    }*/
    
    Light light(vec3(50.0f, 100.0f, 50.0f));
    
    ShaderManager shaderManager("vertexshader.glsl", "fragmentshader.glsl");
    
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // nowireframe
    
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
    
    while(!glfwWindowShouldClose(window)) {
        updateFpsCounter(window);
        
        mat4 model = translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.f));
        mat4 view = lookAt(camera.getEye(), camera.getCenter(), camera.getUp());
        mat4 proj = perspective(45.0f, 4.0f/3.0f, 0.1f, 256.0f);
        
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int modelLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "model_mat");
        int viewLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "view_mat");
        int projLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "projection_mat");
        
        int lightPosition = glGetUniformLocation(shaderManager.getShaderProgram(), "light_position_world");
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
        
        glUseProgram(shaderManager.getShaderProgram());
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(proj));
        
        glUniform3fv(lightPosition, 1, value_ptr(light.getPosition()));
        glUniform3fv(lightSpecularIntensity, 1, value_ptr(light.getSpecularIntensity()));
        glUniform3fv(lightDiffuseIntensity, 1, value_ptr(light.getDiffuseIntensity()));
        glUniform3fv(lightAmbientIntensity, 1, value_ptr(light.getAmbientIntensity()));
        
        for(Mesh mesh : meshes) {
            glUniform3fv(meshSpecularIntensity, 1, value_ptr(mesh.getMaterial()->getSpecularReflectance()));
            glUniform3fv(meshDiffuseIntensity, 1, value_ptr(mesh.getMaterial()->getDiffuseReflectance()));
            glUniform3fv(meshAmbientReflectance, 1, value_ptr(mesh.getMaterial()->getAmbientReflectance()));
            glUniform1f(meshShininess, mesh.getMaterial()->getShininess());
            mesh.draw();
        }
        
        glUniform1i(layer0, 0);
        glUniform1i(layer1, 1);
        glUniform1i(layer2, 2);
        glUniform1i(layer3, 3);
        glUniform1i(layer4, 4);
        
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
        // update other events like input handling
        glfwPollEvents();
        
        update();
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    
    return 0;
}
