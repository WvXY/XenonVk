#pragma once

#include "lge_camera.hpp"
#include "lge_device.hpp"
#include "lge_frame_info.hpp"
#include "lge_game_object.hpp"
#include "lge_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lge {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(
      LgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem&)            = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  void renderGameObjects(FrameInfo& frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  LgeDevice& lgeDevice;

  std::unique_ptr<LgePipeline> lgePipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace lge
