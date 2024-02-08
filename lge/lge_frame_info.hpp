#pragma once

#include <vulkan/vulkan.h>

#include "lge_camera.hpp"
#include "lge_game_object.hpp"


namespace lge {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LgeCamera& camera;
  VkDescriptorSet globalDescriptorSet;
  LgeGameObject::Map& gameObjects;
};

} // namespace lge
