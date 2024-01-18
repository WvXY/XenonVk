#pragma once

#include "lge_camera.hpp"

#include <vulkan/vulkan.h>

namespace lge {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LgeCamera& camera;
};

} // namespace lge
