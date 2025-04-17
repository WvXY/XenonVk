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

  // TODO: encapsulate single pipeline initialization
  std::unique_ptr<XevPipeline> fillPipeline;
  std::string vertShaderSrc = "../spirv/basic_shader.vert.spv";
  std::string fragShaderSrc = "../spirv/basic_shader.frag.spv";

  std::unique_ptr<XevPipeline> wireframePipeline;
  std::string wireVertShaderSrc = "../spirv/wireframe.vert.spv";
  std::string wireFragShaderSrc = "../spirv/wireframe.frag.spv";
};
} // namespace xev
