#version 410
in vec3 position_eye, normal_eye;

uniform mat4 view_mat;

// fixed point light properties
uniform vec3 light_position_world;
uniform vec3 Ls; // specular light colour
uniform vec3 Ld; // diffuse light colour
uniform vec3 La; // ambient light colour

// surface reflectance
uniform vec3 Ks; // specular surface reflectance
uniform vec3 Kd; // diffuse surface reflectance
uniform vec3 Ka; // ambient surface reflectance
uniform float specular_exponent; // specular 'power'

out vec4 fragment_colour; // final colour of surface

void main () {
    // ambient intensity
    vec3 Ia = La * Ka;
    
    // diffuse intensity
    // raise light position to eye space
    vec3 light_position_eye = vec3 (view_mat * vec4 (light_position_world, 1.0));
    vec3 distance_to_light_eye = light_position_eye - position_eye;
    vec3 direction_to_light_eye = normalize (distance_to_light_eye);
    float dot_prod = dot (direction_to_light_eye, normal_eye);
    dot_prod = max (dot_prod, 0.0);
    vec3 Id = Ld * Kd * dot_prod; // final diffuse intensity
    
    // specular intensity
    vec3 surface_to_viewer_eye = normalize (-position_eye);
    vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
    float dot_prod_specular = dot (half_way_eye, normal_eye);
    dot_prod_specular = max (dot_prod_specular, 0.0);
    float specular_factor = pow (dot_prod_specular, specular_exponent);
    vec3 Is = Ls * Ks * specular_factor; // final specular intensity
    
    // final colour
    fragment_colour = vec4 (Is + Id + Ia, 1.0);
}