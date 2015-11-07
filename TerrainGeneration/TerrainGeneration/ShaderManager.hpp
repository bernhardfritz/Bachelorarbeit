//
//  ShaderManager.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef ShaderManager_hpp
#define ShaderManager_hpp

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <string>

using namespace std;

class ShaderManager {
private:
    string vsString;
    const char* vsSource;
    string fsString;
    const char* fsSource;
    GLuint vs;
    GLuint fs;
    GLuint shaderProgram;
    
    string readFile(string filename);
    void printShaderInfoLog(GLuint shaderIndex);
    bool checkForCompileErrors(GLuint shaderIndex);
    void printProgramInfoLog(GLuint program);
    bool checkForLinkingErrors(GLuint program);
    const char* GL_type_to_string(GLenum type);
    void printAll(GLuint program);
    bool isValid(GLuint program);
    void createVertexshader(string vsString);
    void createFragmentshader(string fsString);
    void createShaderProgram(string vsString, string fsString);
    
public:
    ShaderManager(string vsFilename, string fsFilename);
    GLuint getShaderProgram(void);
};

#endif /* ShaderManager_hpp */
