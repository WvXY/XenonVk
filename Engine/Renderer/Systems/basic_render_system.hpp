#pragma once

#include "render_system_interface.hpp"

namespace xev {
class BasicRenderSystem : protected IRenderSystem {
public:
  BasicRenderSystem(
      XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~BasicRenderSystem() override;
  BasicRenderSystem(const BasicRenderSystem&)            = delete;
  BasicRenderSystem& operator=(const BasicRenderSystem&) = delete;

  void render(FrameInfo& frameInfo) override;

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) override;

  void createPipeline(VkRenderPass renderPass) override;

  std::string vertShaderSrc = "../spirv/basic_shader.vert.spv";
  std::string fragShaderSrc = "../spirv/basic_shader.frag.spv";
};
} // namespace xev
