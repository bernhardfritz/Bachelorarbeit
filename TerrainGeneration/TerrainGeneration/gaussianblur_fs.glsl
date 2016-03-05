#version 410

in vec2 UV;
in vec2 blurUV[15];

layout (location = 0) out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

const float weights[15] = float[15](0.0044299121055113265,0.00895781211794,0.0215963866053,0.0443683338718,0.0776744219933,0.115876621105,0.147308056121,0.159576912161,0.147308056121,0.115876621105,0.0776744219933,0.0443683338718,0.0215963866053,0.00895781211794,0.0044299121055113265);

void main() {
    float z = texture(depthTexture, UV).r;       // fetch the z-value from our depth texture
    float n = 0.001;                             // the near plane
    float f = 2.0;                               // the far plane
    float c = (2.0 * n) / (f + n - z * (f - n)); // convert to linear values
    
    color = vec4(0);
    for (int i = 0; i < 15; i++) {
        color += texture(colorTexture, blurUV[i]) * weights[i];
    }
}