#version 410
// surface reflectance
uniform vec3 Kd; // diffuse surface reflectance

out vec4 fragment_colour; // final colour of surface

void main () {
    vec3 Id = Kd; // final diffuse intensity
    
    // final colour
    fragment_colour = vec4 (Id, 1.0);
}