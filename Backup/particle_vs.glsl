#version 410

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square

uniform mat4 view_mat;
uniform mat4 projection_mat;
// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 particlecolor;

void main()
{
    float particleSize = xyzs.w; // because we encoded it this way.
    vec3 particleCenter_worldspace = xyzs.xyz;
    vec3 CameraRight_worldspace = vec3(view_mat[0][0], view_mat[1][0], view_mat[2][0]);
    vec3 CameraUp_worldspace = vec3(view_mat[0][1], view_mat[1][1], view_mat[2][1]);
    mat4 VP = projection_mat * view_mat;
    
    vec3 vertexPosition_worldspace =
    particleCenter_worldspace
    + CameraRight_worldspace * squareVertices.x * particleSize
    + CameraUp_worldspace * squareVertices.y * particleSize;
    
    // Output position of the vertex
    gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);
    
    // UV of the vertex. No special space for this one.
    UV = squareVertices.xy + vec2(0.5, 0.5);
    particlecolor = color;
}

