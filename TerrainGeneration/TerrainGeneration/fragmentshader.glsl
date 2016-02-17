#version 410
in vec3 position_eye, normal_eye;
in vec2 texture_coordinates;
in vec3 position;

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

uniform sampler2D layer0;
uniform sampler2D layer1;
uniform sampler2D layer2;
uniform sampler2D layer3;

uniform float threshold0;
uniform float threshold1;
uniform float threshold2;
uniform float delta;

uniform int textured;

void main () {
    vec4 color0 = texture(layer0, texture_coordinates);
    vec4 color1 = texture(layer1, texture_coordinates);
    vec4 color2 = texture(layer2, texture_coordinates);
    vec4 color3 = texture(layer3, texture_coordinates);
    
    vec4 texel = vec4(0.0, 0.0, 0.0, 0.0);
    float half_delta = delta / 2.0;
    if(position.y < threshold0 - half_delta) texel = color0;
    if(position.y >= threshold0 - half_delta && position.y < threshold0 + half_delta) texel = mix(color1, color0, (threshold0 + half_delta - position.y) / delta);
    if(position.y >= threshold0 + half_delta && position.y < threshold1 - half_delta) texel = color1;
    if(position.y >= threshold1 - half_delta && position.y < threshold1 + half_delta) texel = mix(color2, color1, (threshold1 + half_delta - position.y) / delta);
    if(position.y >= threshold1 + half_delta && position.y < threshold2 - half_delta) texel = color2;
    if(position.y >= threshold2 - half_delta && position.y < threshold2 + half_delta) texel = mix(color3, color2, (threshold2 + half_delta - position.y) / delta);
    if(position.y >= threshold2 + half_delta) texel = color3;
    //vec4 texel = color1;
    
    // ambient intensity
    vec3 Ia = La * Ka;
    
    // diffuse intensity
    // raise light position to eye space
    vec3 light_position_eye = vec3 (view_mat * vec4 (light_position_world, 1.0));
    vec3 distance_to_light_eye = light_position_eye - position_eye;
    vec3 direction_to_light_eye = normalize (distance_to_light_eye);
    float dot_prod = dot (direction_to_light_eye, normal_eye);
    dot_prod = max (dot_prod, 0.0);
    vec3 Id;
    if(textured == 0) Id = Ld * Kd * dot_prod; // final diffuse intensity
    else Id = Ld * texel.rgb * dot_prod; // final diffuse intensity
    
    // specular intensity
    vec3 surface_to_viewer_eye = normalize (-position_eye);
    vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
    float dot_prod_specular = dot (half_way_eye, normal_eye);
    dot_prod_specular = max (dot_prod_specular, 0.0);
    float specular_factor = pow (dot_prod_specular, specular_exponent);
    vec3 Is = Ls * Ks * specular_factor; // final specular intensity
    
    // final colour
    fragment_colour = vec4 (Is + Id + Ia, texel.a);
}