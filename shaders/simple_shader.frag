#version 460

#extension GL_EXT_fragment_shader_barycentric : require

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
  PointLight pointLights[16];
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

  // draw wireframe
  const vec3 barycentric = vec3(gl_BaryCoordEXT);
  const float closestEdge = min(barycentric.x, min(barycentric.y, barycentric.z));

  float edgeThreshold = 0.01;
  if (closestEdge < edgeThreshold) {
    outColor = vec4(0, 0, 0, 1);
  } else {
    outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
  }
}