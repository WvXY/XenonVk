#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace xev {

struct SimplePushConstantData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(
    XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : xevDevice{device} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
  vkDestroyPipelineLayout(xevDevice.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size   = sizeof(SimplePushConstantData);

  std::vector<VkDescriptorSetLayout> layouts = {globalSetLayout};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
  pipelineLayoutInfo.pSetLayouts    = layouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges    = &pushConstantRange;
  if (vkCreatePipelineLayout(
          xevDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  XevPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass     = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  xevPipeline                   = std::make_unique<XevPipeline>(
      xevDevice, vertShaderSrc, fragShaderSrc, pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo) {
  xevPipeline->bind(frameInfo.commandBuffer);

  vkCmdBindDescriptorSets(
      frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
      &frameInfo.globalDescriptorSet, 0, nullptr);

  auto entitiesWithReqComps =
      frameInfo.entityManager
          .getEntitiesWithComponents<ModelComponent, TransformComponent>();

  for (const auto& entity : entitiesWithReqComps) {
    auto& modelComponent = frameInfo.entityManager.getComponent<ModelComponent>(entity);
    auto& transformComponent =
        frameInfo.entityManager.getComponent<TransformComponent>(entity);

    if (modelComponent.model == nullptr) continue;

    SimplePushConstantData push{};
    push.modelMatrix  = transformComponent.getMat4();
    push.normalMatrix = transformComponent.getNormalMat3();

    vkCmdPushConstants(
        frameInfo.commandBuffer, pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(SimplePushConstantData), &push);

    // Bind the model and draw
    modelComponent.model->bind(frameInfo.commandBuffer);
    modelComponent.model->draw(frameInfo.commandBuffer);
  }
}

} // namespace xev
