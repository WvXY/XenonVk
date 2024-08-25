#version 460

#extension GL_EXT_fragment_shader_barycentric : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout(location = 0) out vec4 outColor;

struct PointLight {
    vec4 position;
    vec4 color;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 inverseView;
    vec4 ambientLightColor;// w is intensity
    PointLight pointLights[128];
    int pointLightCount;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 specularLight = vec3(0.0);
    vec3 surfaceNormal = normalize(fragNormalWorld);

    vec3 cameraPosWorld = vec3(ubo.inverseView[3]);
    vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

    for (int i = 0; i < ubo.pointLightCount; i++) {
        PointLight light = ubo.pointLights[i];
        vec3 lightDirection = light.position.xyz - fragPosWorld;
        float attenuation = 1.0 / dot(lightDirection, lightDirection);
        lightDirection = normalize(lightDirection);

        float cosAngle = max(dot(surfaceNormal, lightDirection), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;
        diffuseLight += cosAngle * intensity;

        vec3 halfAngle = normalize(lightDirection + viewDirection);
        float blinnTerm = dot(surfaceNormal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0, 1);
        blinnTerm = pow(blinnTerm, 512);
        specularLight += intensity * blinnTerm;
    }

    // draw wireframe (https://wunkolo.github.io/post/2022/07/gl_ext_fragment_shader_barycentric-wireframe/)
    const vec3 baryCoord = vec3(gl_BaryCoordEXT);
    const vec3 dBaryCoordX = dFdx(baryCoord);
    const vec3 dBaryCoordY = dFdy(baryCoord);
    const vec3 dBaryCoord  = sqrt(dBaryCoordX * dBaryCoordX + dBaryCoordY * dBaryCoordY);

    float edgeThreshold = 0.01;
    const float thickness = 10;// In pixels
    const vec3 remap = smoothstep(vec3(0.0), dBaryCoord * thickness, baryCoord);
    const float wireframe = min(remap.x, min(remap.y, remap.z));

    if (wireframe < edgeThreshold) {
        outColor = vec4(0, 0, 0, 1);
    } else {
        outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
    }
}