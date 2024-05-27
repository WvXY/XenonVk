#pragma once

#include "vk_camera.hpp"
#include "vk_device.hpp"
#include "vk_frame_info.hpp"
#include "vk_game_object.hpp"
#include "vk_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace xev {
class PointLightSystem {
public:
  PointLightSystem(
      XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();
  PointLightSystem(const PointLightSystem&) = delete;
  PointLightSystem& operator=(const PointLightSystem&) = delete;

  void update(FrameInfo& frameInfo, GlobalUbo& globalUbo);
  void render(FrameInfo& frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  void getAttributeDescriptions(PipelineConfigInfo& configInfo);

  XevDevice& xevDevice;

  std::string vertShaderSrc = "../shaders/point_light.vert.spv";
  std::string fragShaderSrc = "../shaders/point_light.frag.spv";

  std::unique_ptr<XevPipeline> xevPipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace xev
