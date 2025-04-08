#pragma once

#include "render_system_interface.hpp"

namespace xev {
class PointLightSystem : protected IRenderSystem {
public:
  PointLightSystem(
      XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem() override;
  PointLightSystem(const PointLightSystem&)            = delete;
  PointLightSystem& operator=(const PointLightSystem&) = delete;

  void update(FrameInfo& frameInfo, GlobalUbo& globalUbo) override;
  void render(FrameInfo& frameInfo) override;

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) override;
  void createPipeline(VkRenderPass renderPass) override;

  void getAttributeDescriptions(PipelineConfigInfo& configInfo);

  std::unique_ptr<XevPipeline> xevPipeline;
  std::string vertShaderSrc = "../spirv/point_light.vert.spv";
  std::string fragShaderSrc = "../spirv/point_light.frag.spv";
};
} // namespace xev
