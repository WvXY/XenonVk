#include "point_light_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>

namespace xev {

struct PointLightPushConstants {
  glm::vec4 position{};
  glm::vec4 color{};
  float radius{};
};

PointLightSystem::PointLightSystem(
    XevDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : IRenderSystem{device} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
  vkDestroyPipelineLayout(xevDevice.device(), pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size   = sizeof(PointLightPushConstants);

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

void PointLightSystem::getAttributeDescriptions(PipelineConfigInfo& configInfo) {
  configInfo.attributeDescriptions = std::vector<VkVertexInputAttributeDescription>{};
} // don't need any attributes

void PointLightSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  XevPipeline::defaultPipelineConfigInfo(pipelineConfig);
  getAttributeDescriptions(pipelineConfig);
  XevPipeline::enableAlphaBlending(pipelineConfig);
  pipelineConfig.renderPass     = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  xevPipeline                   = std::make_unique<XevPipeline>(
      xevDevice, vertShaderSrc, fragShaderSrc, pipelineConfig);
}

void PointLightSystem::update(FrameInfo& frameInfo, GlobalUbo& globalUbo) {
  auto rotateLight =
      glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.frameTime, {0.f, -1.f, 0.f});

  int lightIndex = 0;

  auto entitiesWithReqComps =
      entityManager.getEntitiesWithComponents<PointLightComponent, TransformComponent>();

  for (const auto& entity : entitiesWithReqComps) {
    auto& pointLightComponent = entityManager.getComponent<PointLightComponent>(entity);
    auto& transformComponent  = entityManager.getComponent<TransformComponent>(entity);

    assert(lightIndex < MAX_LIGHTS && "Too many point lights!");

    // gov.transform.translation =
    //     glm::vec3(rotateLight * glm::vec4(gov.transform.translation, 1.0f));

    auto& pointLight    = globalUbo.pointLights[lightIndex];
    pointLight.position = glm::vec4(transformComponent.translation, 1.0f);
    pointLight.color =
        glm::vec4(pointLightComponent.color, pointLightComponent.intensity);

    lightIndex++;
  }
  globalUbo.pointLightCount = lightIndex;
}

void PointLightSystem::render(FrameInfo& frameInfo) {
  std::map<float, Entity> sortedLights;

  // Get entities that have both PointLightComponent and TransformComponent
  for (auto& entity :
       entityManager
           .getEntitiesWithComponents<TransformComponent, PointLightComponent>()) {

    TransformComponent& transform =
        entityManager.getComponent<TransformComponent>(entity);
    PointLightComponent& pointLight =
        entityManager.getComponent<PointLightComponent>(entity);

    // Skip lights with no pointLight component or invalid transform
    if (pointLight.intensity <= 0) { continue; }

    // Calculate distance between camera and the light source
    float distance = glm::length(frameInfo.camera.getPosition() - transform.translation);
    sortedLights[distance] = entity;
  }

  // for rendering
  xevPipeline->bind(frameInfo.commandBuffer);
  vkCmdBindDescriptorSets(
      frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
      &frameInfo.globalDescriptorSet, 0, nullptr);

  // Sorted by distance (farthest to closest)
  for (auto it = sortedLights.rbegin(); it != sortedLights.rend(); ++it) {
    Entity entity = it->second;

    TransformComponent& transform =
        entityManager.getComponent<TransformComponent>(entity);
    PointLightComponent& pointLight =
        entityManager.getComponent<PointLightComponent>(entity);

    PointLightPushConstants push{};
    push.position = glm::vec4(transform.translation, 1.0f);
    push.color    = glm::vec4(pointLight.color, pointLight.intensity);
    push.radius   = transform.scale.x;

    vkCmdPushConstants(
        frameInfo.commandBuffer, pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(PointLightPushConstants), &push);

    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
  }
}

} // namespace xev
