#version 410

in vec2 UV;

out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D brightTexture;

void main() {
    float z = texture(depthTexture, UV).r;        // fetch the z-value from our depth texture
    float n = 0.001;                                // the near plane
    float f = 2.0;                               // the far plane
    float c = (2.0 * n) / (f + n - z * (f - n));  // convert to linear values
    
    color = texture(colorTexture, UV);
    vec4 bright = texture(brightTexture, UV);
    if(c < 1.0) color += 0.667 * bright;
    if(c >= 1.0) color += 0.333 * bright;
//    color = bright;
}