#version 410

in vec2 UV;

layout (location = 0) out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

void main() {
    float z = texture(depthTexture, UV).r;       // fetch the z-value from our depth texture
    float n = 0.001;                             // the near plane
    float f = 2.0;                               // the far plane
    float c = (2.0 * n) / (f + n - z * (f - n)); // convert to linear values
    
    vec4 sum = vec4(0);
    for(int i = -2; i <= 2; i++) {
        for(int j = -2; j <= 2; j++) {
            vec2 offset = vec2(i, j) * 0.005;
            sum += texture(colorTexture, UV + offset);
        }
    }
    
    color = sum/25;
}