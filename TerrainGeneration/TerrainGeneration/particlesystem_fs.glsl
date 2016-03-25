#version 410
in vec3 position_eye, normal_eye;

uniform mat4 view_mat;

// fixed point light properties
uniform vec3 light_direction;
uniform vec3 Ls; // specular light colour
uniform vec3 Ld; // diffuse light colour
uniform vec3 La; // ambient light colour

// surface reflectance
uniform vec3 Ks; // specular surface reflectance
uniform vec3 Kd; // diffuse surface reflectance
uniform vec3 Ka; // ambient surface reflectance
uniform float specular_exponent; // specular 'power'

layout(location = 0) out vec4 fragment_colour; // final colour of surface

void main () {
    // ambient intensity
    vec3 Ia = La * Ka;
    
    // diffuse intensity
    // raise light position to eye space
    /*vec3 light_position_eye = vec3 (view_mat * vec4 (light_position_world, 1.0));
     vec3 distance_to_light_eye = light_position_eye - position_eye;
     vec3 direction_to_light_eye = normalize (distance_to_light_eye);*/ // point light
    vec3 direction_to_light_eye = normalize(vec3(view_mat * vec4(vec3(light_direction), 0.0))); // directional light
    float dot_prod = dot (direction_to_light_eye, normal_eye);
    dot_prod = max (dot_prod, 0.0);
    vec3 Id = Ld * Kd * dot_prod; // final diffuse intensity
    
    // final colour
    fragment_colour = vec4 (Id + Ia, 1.0);
}