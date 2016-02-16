#version 410
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texture; // per vertex texture coordinates

uniform mat4 projection_mat, view_mat, model_mat;
out vec3 position_eye, normal_eye;
out vec2 texture_coordinates;

void main () {
    position_eye = vec3 (view_mat * model_mat * vec4 (vertex_position, 1.0));
    normal_eye = vec3 (view_mat * model_mat * vec4 (vertex_normal, 0.0));
    texture_coordinates = vertex_texture;
    gl_Position = projection_mat * vec4 (position_eye, 1.0);
}