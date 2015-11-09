//
//  main.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

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

using namespace std;
using namespace glm;

GLFWwindow* window;

Camera camera(0.0f, 0.0f, 2.0f, 0.0f, -half_pi<float>(), 2.0f);
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
    if(mods & GLFW_MOD_SHIFT) {
        camera.setSpeed(4.0f);
    } else {
        camera.setSpeed(2.0f);
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

void update() {
    if(keyboard.getState(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);
    if(keyboard.getState(GLFW_KEY_W)) camera.moveForward(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_A)) camera.moveLeft(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_S)) camera.moveBackward(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_D)) camera.moveRight(elapsedSeconds);
    if(keyboard.getState(GLFW_KEY_SPACE)) camera.moveUp(elapsedSeconds);
}

int main() {
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
    //MeshLoader ml;
    //Mesh mesh = ml.loadMesh("bunny.obj");
    Heightmap mesh(20,20);
    
    Light light(vec3(2.0f, 2.0f, 2.0f));
    
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
        mat4 proj = perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
        
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
        
        glUseProgram(shaderManager.getShaderProgram());
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(proj));
        
        glUniform3fv(lightPosition, 1, value_ptr(light.getPosition()));
        glUniform3fv(lightSpecularIntensity, 1, value_ptr(light.getSpecularIntensity()));
        glUniform3fv(lightDiffuseIntensity, 1, value_ptr(light.getDiffuseIntensity()));
        glUniform3fv(lightAmbientIntensity, 1, value_ptr(light.getAmbientIntensity()));
        
        glUniform3fv(meshSpecularIntensity, 1, value_ptr(mesh.getMaterial()->getSpecularReflectance()));
        glUniform3fv(meshDiffuseIntensity, 1, value_ptr(mesh.getMaterial()->getDiffuseReflectance()));
        glUniform3fv(meshAmbientReflectance, 1, value_ptr(mesh.getMaterial()->getAmbientReflectance()));
        glUniform1f(meshShininess, mesh.getMaterial()->getShininess());
        
        
        mesh.draw();
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
