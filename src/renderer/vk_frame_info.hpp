#pragma once

#include <vulkan/vulkan.h>

#include "../primitive/vk_game_object.hpp"
#include "vk_camera.hpp"

namespace xev {

#define MAX_LIGHTS 16

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};
};

struct GlobalUbo {
  glm::mat4 projection{1.f};
  glm::mat4 view{1.f};
  glm::mat4 inverseView{1.f};
  glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .08f};
  PointLight pointLights[MAX_LIGHTS];
  int pointLightCount{0};
};

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  XevCamera& camera;
  VkDescriptorSet globalDescriptorSet;
  XevGameObject::Map& gameObjects;
};

} // namespace xev
