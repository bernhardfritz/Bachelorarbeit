#version 410
in vec3 position_eye, normal_eye;

uniform mat4 view_mat;

// fixed point light properties
vec3 light_position_world  = vec3 (2.0, 2.0, 2.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // specular light colour
vec3 Ld = vec3 (0.7, 0.7, 0.7); // diffuse light colour
vec3 La = vec3 (0.2, 0.2, 0.2); // ambient light colour

// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // specular surface reflectance
vec3 Kd = vec3 (1.0, 1.0, 1.0); // diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // ambient surface reflectance
float specular_exponent = 100.0; // specular 'power'

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