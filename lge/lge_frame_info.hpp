#pragma once

#include <vulkan/vulkan.h>

#include "lge_camera.hpp"
#include "lge_game_object.hpp"

namespace lge {

#define MAX_LIGHTS 16

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};
};

struct GlobalUbo {
  glm::mat4 projection{1.f};
  glm::mat4 view{1.f};
  glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .06f};
  PointLight pointLights[MAX_LIGHTS];
  int pointLightCount{0};
};

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LgeCamera& camera;
  VkDescriptorSet globalDescriptorSet;
  LgeGameObject::Map& gameObjects;
};

} // namespace lge
