#pragma once

#include "vk_game_object.hpp"
#include "vk_camera.hpp"
#include "vk_device.hpp"
#include "vk_frame_info.hpp"
#include "vk_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace xev {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(
      XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();
  SimpleRenderSystem(const SimpleRenderSystem&) = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  void renderGameObjects(FrameInfo& frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

  void createPipeline(VkRenderPass renderPass);

  XevDevice& xevDevice;

  std::string vertShaderSrc = "../shaders/simple_shader.vert.spv";
  std::string fragShaderSrc = "../shaders/simple_shader.frag.spv";

  std::unique_ptr<XevPipeline> xevPipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace xev
