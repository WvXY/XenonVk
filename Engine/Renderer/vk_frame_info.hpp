#pragma once

#include <vulkan/vulkan.h>

#include "actor.hpp"
#include "actor_manager.hpp"
#include "entity_manager.hpp"
#include "vk_camera.hpp"

namespace xev {

#define MAX_LIGHTS 128

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};
};

struct GlobalUbo {
  glm::mat4 projection{1.f};
  glm::mat4 view{1.f};
  glm::mat4 inverseView{1.f};
  glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .4f};
  PointLight pointLights[MAX_LIGHTS];
  int pointLightCount{0};
};

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  XevCamera& camera;
  VkDescriptorSet globalDescriptorSet;
};

} // namespace xev
