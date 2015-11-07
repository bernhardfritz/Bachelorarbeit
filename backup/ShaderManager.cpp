class ShaderManager() {
  string readFile(string filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

  void printShaderInfoLog(GLuint shaderIndex) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetShaderInfoLog(shaderIndex, maxLength, &actualLength, log);
    printf("shader info log for GL index %u:\n%s\n", shaderIndex, log);
  }

  bool checkForCompileErrors(GLuint shaderIndex) {
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

  void printProgramInfoLog(GLuint program) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetProgramInfoLog(program, maxLength, &actualLength, log);
    printf("program info log for GL index %u:\n%s", program, log);
  }

  bool checkForLinkingErrors(GLuint program) {
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

  const char* GL_type_to_string(GLenum type) {
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

  void printAll(GLuint program) {
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

  bool isValid(GLuint program) {
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
}
