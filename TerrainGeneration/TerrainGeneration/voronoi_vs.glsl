#version 410
layout (location = 0) in vec3 vertex_position;

uniform mat4 projection_mat, view_mat, model_mat;

void main () {
    vec3 position_eye = vec3 (view_mat * model_mat * vec4 (vertex_position, 1.0));
    gl_Position = projection_mat * vec4 (position_eye, 1.0);
}