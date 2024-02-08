#pragma once

#include "lge_device.hpp"
#include "lge_game_object.hpp"
#include "lge_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lge {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(LgeDevice& device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem&)            = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  void renderGameObjects(
      VkCommandBuffer commandBuffer, std::vector<LgeGameObject>& gameObjects);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  LgeDevice& lgeDevice;

  std::unique_ptr<LgePipeline> lgePipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace lge
