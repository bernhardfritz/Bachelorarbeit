#version 410

in vec2 UV;

layout (location = 0) out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

const float luminance = 0.5;
const float middleGrey = 0.5;
const float threshold = 0.4;

void main(){
    float z = texture(depthTexture, UV).r;        // fetch the z-value from our depth texture
    float n = 0.001;                                // the near plane
    float f = 2.0;                               // the far plane
    float c = (2.0 * n) / (f + n - z * (f - n));  // convert to linear values
    
    color = texture(colorTexture, UV);
    
    color *= (middleGrey/luminance);
    color *= ( 1.0 + (color/(threshold * threshold) ) );
    color -= 0.5;
    color /= (1.0 + color);
}