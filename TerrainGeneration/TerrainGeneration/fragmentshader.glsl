#version 410
in vec3 position_eye, normal_eye;
in vec2 texture_coordinates;
in vec3 position;
in vec3 normal;
in vec3 reflect_dir;
in vec3 look_at;

uniform mat4 view_mat;

// fixed point light properties
uniform vec3 light_direction;
uniform vec3 Ls; // specular light colour
uniform vec3 Ld; // diffuse light colour
uniform vec3 La; // ambient light colour

uniform float fog_near;
uniform float fog_far;
uniform vec3 fog_color;

// surface reflectance
uniform vec3 Ks; // specular surface reflectance
uniform vec3 Kd; // diffuse surface reflectance
uniform vec3 Ka; // ambient surface reflectance
uniform float specular_exponent; // specular 'power'

layout(location = 0) out vec4 fragment_colour; // final colour of surface

uniform sampler2D layer0;
uniform sampler2D layer1;
uniform sampler2D layer2;
uniform sampler2D layer3;
uniform sampler2D layer4;

uniform int textured;
uniform int is_water;
uniform float time;
uniform float max_height;
uniform float min_height;

uniform samplerCube cube_texture;

void main () {
    vec4 color0 = texture(layer0, texture_coordinates); // sand
    vec4 color1 = texture(layer1, texture_coordinates); // grass
    vec4 color2 = texture(layer2, texture_coordinates); // rock
    vec4 color3 = texture(layer3, texture_coordinates); // snow
    vec4 color4 = texture(layer4, texture_coordinates+vec2(-time, -time)); // water
    
    float difference = max_height - min_height;
    float delta = difference / 4.0;
    float threshold0 = delta * 1;
    float threshold1 = delta * 2;
    float threshold2 = delta * 3;
    
    float t0 = 0.75;
    float t1 = 0.5;
    float t2 = 0.25;
    float d = 0.25;
    
    vec4 texel = vec4(0.0, 0.0, 0.0, 0.0);
    if(position.y < threshold0) texel = color0;
    if(position.y >= threshold0 && position.y < threshold0 + delta) texel = mix(color0, color1, (position.y - threshold0) / delta);
    if(position.y >= threshold0 + delta && position.y < threshold1) texel = color1;
    if(position.y >= threshold1 && position.y < threshold1 + delta) texel = mix(color1, color2, (position.y - threshold1) / delta);
    if(position.y >= threshold1 + delta) texel = color2;
    if(position.y >= threshold2 && position.y < threshold2 + delta) texel = mix(color2, color3, (position.y - threshold2) / delta);
    if(position.y >= threshold2 + delta) texel = color3;
    
    if(position.y >= threshold1 && position.y < threshold2) {
        if(normal.y >= t0) texel = mix(texel, color1, (threshold2 - position.y)/(threshold2 - threshold1));
        if(normal.y < t0 && normal.y >= t0 - d) texel = mix(texel, mix(color1, texel, (t0 - normal.y)/d), (threshold2 - position.y)/(threshold2 - threshold1));
    }
    
    if(position.y >= threshold0 && position.y < threshold1 + delta) {
        if(normal.y < t1 && normal.y >= t1 - d) texel = mix(mix(texel, color2, (t1 - normal.y)/d), texel, (threshold1 + delta - position.y)/(threshold1 + delta - threshold0));
        if(normal.y < t1 - d) texel = mix(color2, texel, (threshold1 + delta - position.y)/(threshold1 + delta - threshold0));
    }
    
    if(position.y >= threshold1) {
        if(normal.y >= t1) texel = mix(texel, color3, min(1.0,(position.y - threshold1)/delta));
        if(t1 - d <= normal.y && normal.y < t1) texel = mix(texel, mix(color3, texel, (t1 - normal.y)/d), min(1.0, (position.y - threshold1)/delta));
    }
    
    if(position.y >= threshold1 && position.y < threshold2 + delta) {
        if(normal.y < t1 && normal.y >= t1 - d) texel = mix(mix(texel, color2, (t1 - normal.y)/d), texel, (threshold2 + delta - position.y)/(threshold2 + delta - threshold1));
        if(normal.y < t1 - d) texel = mix(color2, texel, (threshold2 + delta - position.y)/(threshold2 + delta - threshold1));
    }
    
    if(is_water == 1) {
        texel = mix(color4, texture(cube_texture, reflect_dir), 0.75);
    }
    
    /*if(normal.y >= t0) texel = color0;
    if(normal.y < t0 && normal.y >= t0 - d) texel = mix(color0, color1, (t0 - normal.y)/d);
    if(normal.y < t0 - d && normal.y >= t1) texel = color1;
    if(normal.y < t1 && normal.y >= t1 - d) texel = mix(color1, color2, (t1 - normal.y)/d);
    if(normal.y < t1 - d && normal.y >= t2) texel = color2;
    if(normal.y < t2 && normal.y >= t2 - d) texel = mix(color2, color3, (t2 - normal.y)/d);
    if(normal.y < t2 - d) texel = color3;*/
    
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
    if(textured == 0) fragment_colour = vec4 (Is + Id + Ia, 1.0);
    else {
        fragment_colour = vec4 (Is + Id + Ia, texel.a);
        if(is_water == 1) fragment_colour.a = clamp(1.333 + look_at.y, 0.9, 1);
    }
    
    // work out distance from camera to point
    float dist = length (-position_eye);
    // get a fog factor (thickness of fog) based on the distance
    float fog_fac = (dist - fog_near) / (fog_far - fog_near);
    // constrain the fog factor between 0 and 1
    fog_fac = clamp (fog_fac, 0.0, 1.0);
    
    // blend the fog colour with the lighting colour, based on the fog factor
    fragment_colour.rgb = mix (fragment_colour.rgb, fog_color, fog_fac);
}