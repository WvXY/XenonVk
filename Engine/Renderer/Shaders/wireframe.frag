#version 460

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor * 1.5, 1.0);
    outColor = clamp(outColor, 0.0, 1.0);
}