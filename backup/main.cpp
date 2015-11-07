#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

double previousSeconds;
int frameCount;
/* we will use this function to update the window title with a frame rate */
void updateFpsCounter(GLFWwindow* window) {
  double currentSeconds;
  double elapsedSeconds;
  currentSeconds = glfwGetTime();
  elapsedSeconds = currentSeconds - previousSeconds;
  /* limit text updates to 4 per second */
  if(elapsedSeconds > 0.25) {
    previousSeconds = currentSeconds;
    char tmp[128];
    double fps = (double)frameCount / elapsedSeconds;
    sprintf(tmp, "opengl @ fps: %.2f", fps);
    glfwSetWindowTitle(window, tmp);
    frameCount = 0;
  }
  frameCount++;
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

string readFile(string filename) {
  ifstream file(filename);
  stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
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

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if(!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

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
  GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  GLfloat colors[] = {
    1.0f, 0.0f,  0.0f,
    0.0f, 1.0f,  0.0f,
    0.0f, 0.0f,  1.0f
  };

  GLuint pointsVBO = 0;
  glGenBuffers(1, &pointsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);

  GLuint colorsVBO = 0;
  glGenBuffers(1, &colorsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof (colors), colors, GL_STATIC_DRAW);

  GLuint VAO = 0;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  string vertString = readFile("vertexshader.glsl");
  const char* vertexShader = vertString.c_str();

  string fragString = readFile("fragmentshader.glsl");
  const char* fragmentShader = fragString.c_str();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertexShader, NULL);
  glCompileShader(vs);
  checkForCompileErrors(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragmentShader, NULL);
  glCompileShader(fs);
  checkForCompileErrors(fs);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, fs);
  glAttachShader(shaderProgram, vs);
  glLinkProgram(shaderProgram);
  checkForLinkingErrors(shaderProgram);
  printAll(shaderProgram);
  isValid(shaderProgram);

  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // nowireframe

  glEnable(GL_CULL_FACE); // cull face
  glCullFace(GL_BACK); // cull back face
  glFrontFace(GL_CW); // GL_CCW for counter clock-wise

  while(!glfwWindowShouldClose(window)) {
    updateFpsCounter(window);

    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    // update other events like input handling
    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose (window, 1);
    }
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
