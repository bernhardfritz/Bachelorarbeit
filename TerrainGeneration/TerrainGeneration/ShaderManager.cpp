//
//  ShaderManager.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "ShaderManager.hpp"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

ShaderManager::ShaderManager(string vsFilename, string fsFilename) {
    createShaderProgram(readFile(vsFilename), readFile(fsFilename));
}

string ShaderManager::readFile(string filename) {
    ifstream t(filename);
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void ShaderManager::printShaderInfoLog(GLuint shaderIndex) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetShaderInfoLog(shaderIndex, maxLength, &actualLength, log);
    printf("shader info log for GL index %u:\n%s\n", shaderIndex, log);
}
    
bool ShaderManager::checkForCompileErrors(GLuint shaderIndex) {
    // check for compile errors
    int params = -1;
    glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf (stderr, "ERROR: GL shader index %i did not compile\n", shaderIndex);
        printShaderInfoLog (shaderIndex);
        return false; // or exit or something
    }
    return true;
}
    
void ShaderManager::printProgramInfoLog(GLuint program) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetProgramInfoLog(program, maxLength, &actualLength, log);
    printf("program info log for GL index %u:\n%s", program, log);
}
    
bool ShaderManager::checkForLinkingErrors(GLuint program) {
    // check if link was successful
    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if(GL_TRUE != params) {
        fprintf(stderr, "ERROR: could not link shader program GL index %u\n", program);
        printProgramInfoLog(program);
        return false;
    }
    return true;
}
    
const char* ShaderManager::GL_type_to_string(GLenum type) {
    switch (type) {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_CUBE: return "samplerCube";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}
    
void ShaderManager::printAll(GLuint program) {
    printf ("--------------------\nshader program %i info:\n", program);
    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    printf("GL_LINK_STATUS = %i\n", params);
    
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
    printf("GL_ATTACHED_SHADERS = %i\n", params);
        
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
    printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);
    for(GLuint i = 0; i < (GLuint)params; i++) {
        char name[64];
        int maxLength = 64;
        int actualLength = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib(program, i, maxLength, &actualLength, &size, &type, name);
        if(size > 1) {
            for(int j = 0; j < size; j++) {
                char longName[64];
                sprintf(longName, "%s[%i]", name, j);
                int location = glGetAttribLocation(program, longName);
                printf("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), longName, location);
            }
        } else {
                int location = glGetAttribLocation(program, name);
                printf("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
        }
    }
    
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
    printf("GL_ACTIVE_UNIFORMS = %i\n", params);
    for(GLuint i = 0; i < (GLuint)params; i++) {
        char name[64];
        int maxLength = 64;
        int actualLength = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(program, i, maxLength, &actualLength, &size, &type, name);
        if(size > 1) {
            for(int j = 0; j < size; j++) {
                char longName[64];
                sprintf(longName, "%s[%i]", name, j);
                int location = glGetUniformLocation(program, longName);
                printf("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), longName, location);
            }
        } else {
            int location = glGetUniformLocation(program, name);
            printf("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
        }
    }
        
    printProgramInfoLog(program);
}
    
bool ShaderManager::isValid(GLuint program) {
    glValidateProgram(program);
    int params = -1;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
    printf("program %i GL_VALIDATE_STATUS = %i\n", program, params);
    if(GL_TRUE != params) {
        printProgramInfoLog(program);
        return false;
    }
    return true;
}

void ShaderManager::createVertexshader(string vsString) {
    this->vsString = vsString;
    vsSource = vsString.c_str();
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);
    checkForCompileErrors(vs);
}

void ShaderManager::createFragmentshader(string fsString) {
    this->fsString = fsString;
    fsSource = fsString.c_str();
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    checkForCompileErrors(fs);
}

void ShaderManager::createShaderProgram(string vsString, string fsString) {
    createVertexshader(vsString);
    createFragmentshader(fsString);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);
    checkForLinkingErrors(shaderProgram);
    printAll(shaderProgram);
    isValid(shaderProgram);
}

GLuint ShaderManager::getShaderProgram() {
    return shaderProgram;
}