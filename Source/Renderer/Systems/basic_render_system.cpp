#include "basic_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

#include "entity_manager.hpp"

namespace xev {

struct SimplePushConstantData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

BasicRenderSystem::BasicRenderSystem(
    XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : IRenderSystem(device) {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}

BasicRenderSystem::~BasicRenderSystem() {
  vkDestroyPipelineLayout(xevDevice.device(), pipelineLayout, nullptr);
}

void BasicRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
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

void BasicRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  // Fill pipeline
  PipelineConfigInfo fillConfig{};
  XevPipeline::defaultPipelineConfigInfo(fillConfig);
  fillConfig.renderPass                    = renderPass;
  fillConfig.pipelineLayout                = pipelineLayout;
  fillConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  // fillConfig.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;

  fillPipeline =
      std::make_unique<XevPipeline>(xevDevice, vertShaderSrc, fragShaderSrc, fillConfig);

  // Wireframe pipeline
  PipelineConfigInfo wireConfig = fillConfig;
  wireConfig.dynamicStateEnables.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
  wireConfig.dynamicStateInfo.pDynamicStates = wireConfig.dynamicStateEnables.data();
  wireConfig.dynamicStateInfo.dynamicStateCount =
      static_cast<uint32_t>(wireConfig.dynamicStateEnables.size());

  wireConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
  wireConfig.rasterizationInfo.cullMode    = VK_CULL_MODE_NONE;
  wireConfig.rasterizationInfo.lineWidth   = 2.0f;

  // Optional: Use depth bias to prevent z-fighting
  wireConfig.rasterizationInfo.depthBiasEnable = VK_TRUE;
  // wireConfig.rasterizationInfo.depthBiasConstantFactor = 1.25f;
  // wireConfig.rasterizationInfo.depthBiasSlopeFactor = 1.75f;

  wireframePipeline = std::make_unique<XevPipeline>(
      xevDevice, wireVertShaderSrc, wireFragShaderSrc, wireConfig);
}

void BasicRenderSystem::render(FrameInfo& frameInfo) {
  auto entitiesWithReqComps =
      entityManager.getEntitiesWithComponents<ModelComponent, TransformComponent>();

  // First pass: Filled rendering
  fillPipeline->bind(frameInfo.commandBuffer);
  vkCmdBindDescriptorSets(
      frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
      &frameInfo.globalDescriptorSet, 0, nullptr);

  for (const auto& entity : entitiesWithReqComps) {
    auto& modelComponent     = entityManager.getComponent<ModelComponent>(entity);
    auto& transformComponent = entityManager.getComponent<TransformComponent>(entity);
    if (modelComponent.model == nullptr) continue;

    SimplePushConstantData push{};
    push.modelMatrix  = transformComponent.getMat4();
    push.normalMatrix = transformComponent.getNormalMat3();

    vkCmdPushConstants(
        frameInfo.commandBuffer, pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(SimplePushConstantData), &push);

    modelComponent.model->bind(frameInfo.commandBuffer);
    modelComponent.model->draw(frameInfo.commandBuffer);
  }

  // Second pass: Wireframe rendering
  wireframePipeline->bind(frameInfo.commandBuffer);
  vkCmdBindDescriptorSets(
      frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
      &frameInfo.globalDescriptorSet, 0, nullptr);

  for (const auto& entity : entitiesWithReqComps) {
    auto& modelComponent     = entityManager.getComponent<ModelComponent>(entity);
    auto& transformComponent = entityManager.getComponent<TransformComponent>(entity);
    if (modelComponent.model == nullptr) continue;

    SimplePushConstantData push{};
    push.modelMatrix  = transformComponent.getMat4();
    push.normalMatrix = transformComponent.getNormalMat3();

    vkCmdPushConstants(
        frameInfo.commandBuffer, pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(SimplePushConstantData), &push);

    modelComponent.model->bind(frameInfo.commandBuffer);
    modelComponent.model->draw(frameInfo.commandBuffer);
  }
}

} // namespace xev
