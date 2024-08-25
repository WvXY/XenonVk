#version 460

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

struct PointLight {
    vec4 position;
    vec4 color;
    float radius;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 inverseView;
    vec4 ambientLightColor;  // w is intensity
    PointLight pointLights[128];
    int pointLightCount;
} ubo;

layout(push_constant) uniform Push {
    vec4 position;
    vec4 color;
    float radius;
} push;

const float PI = 3.14159265359;

void main() {
    float dis = sqrt(dot(fragOffset, fragOffset));
    if (dis >= 1.0) { discard; }
    outColor = vec4(push.color.rgb, 0.5 *(cos(dis * PI) + 1.0));
}
