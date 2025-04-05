#pragma once

#include "entity_manager.hpp"
#include "vk_device.hpp"
#include "vk_frame_info.hpp"
#include "vk_pipeline.hpp"

namespace xev {

class IRenderSystem {
public:
  IRenderSystem(XevDevice& device)
      : xevDevice{device}, entityManager{EntityManager::instance()} {}

  virtual ~IRenderSystem() = default;

  virtual void update(FrameInfo&, GlobalUbo&) {}
  virtual void getAtttributeDescriptions(PipelineConfigInfo&) {}

  virtual void render(FrameInfo& frameInfo)                                = 0;
  virtual void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) = 0;
  virtual void createPipeline(VkRenderPass renderPass)                     = 0;

protected:
  EntityManager& entityManager;

  XevDevice& xevDevice;
  std::unique_ptr<XevPipeline> xevPipeline;
  VkPipelineLayout pipelineLayout = nullptr;

  std::string vertShaderSrc;
  std::string fragShaderSrc;
};

} // namespace xev