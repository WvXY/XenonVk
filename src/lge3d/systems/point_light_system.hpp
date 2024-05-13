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
class PointLightSystem {
public:
  PointLightSystem(
      LgeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

  ~PointLightSystem();

  PointLightSystem(const PointLightSystem&) = delete;

  PointLightSystem& operator=(const PointLightSystem&) = delete;

  void update(FrameInfo& frameInfo, GlobalUbo& globalUbo);

  void render(FrameInfo& frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

  void createPipeline(VkRenderPass renderPass);

  LgeDevice& lgeDevice;

  std::string vertShaderSrc = "../../shaders/point_light.vert.spv";
  std::string fragShaderSrc = "../../shaders/point_light.frag.spv";

  std::unique_ptr<LgePipeline> lgePipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace lge
