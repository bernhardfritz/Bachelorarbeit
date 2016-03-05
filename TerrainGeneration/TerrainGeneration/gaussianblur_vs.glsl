#version 410

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform vec2 direction;

const float offsets[15] = float[15](-0.028, -0.024, -0.020, -0.016, -0.012, -0.008, -0.004, 0.000, 0.004, 0.008, 0.012, 0.016, 0.020, 0.024, 0.028);

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec2 blurUV[15];

void main(){
    gl_Position = vec4(vertexPosition_modelspace,1);
    UV = (vertexPosition_modelspace.xy+vec2(1,1))/2.0;
    for(int i = 0; i < 15; i++) {
        blurUV[i] = UV + vec2(direction.x * offsets[i], direction.y * offsets[i]);
    }
}