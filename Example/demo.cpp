#include "demo.hpp"

#include "basic_render_system.hpp"
#include "components.hpp"
#include "controller.hpp"
#include "point_light_system.hpp"
#include "time_manager.hpp"
#include "vk_buffer.hpp"
#include "vk_camera.hpp"

// std
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace xev {

DemoApp::DemoApp() {
  globalPool =
      XevDescriptorPool::Builder(xevDevice)
          .setMaxSets(XevSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(
              VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, XevSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();
  load();
}

DemoApp::~DemoApp() {}

void DemoApp::run() {
  std::vector<std::unique_ptr<XevBuffer>> uboBuffers(XevSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<XevBuffer>(
        xevDevice, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffers[i]->map();
  }

  auto globalSetLayout =
      XevDescriptorSetLayout::Builder(xevDevice)
          .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
          .build();
  std::vector<VkDescriptorSet> globalDescriptorSets(XevSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    XevDescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  BasicRenderSystem basicRenderSystem{
      xevDevice, xevRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  PointLightSystem pointLightSystem{
      xevDevice, xevRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};

  XevCamera camera{};
  glm::vec3 cameraTarget = {1.f, 0.f, 1.f};
  camera.setViewTarget({-1.f, -3.f, 1.f}, cameraTarget);

  XevController xevController{};

  // Manage view by ECS
  Entity viewEntity = entityManager.createEntity();
  TransformComponent viewTransform;
  viewTransform.translation = glm::vec3{0, 0, 0};
  entityManager.addComponent(viewEntity, viewTransform);

  // Time management
  timeManager.start();
  auto& frameTime = timeManager.getFrameTime();

  while (!xevWindow.shouldClose() &&
         !xevController.isPressed(xevWindow.getGLFWwindow(), GLFW_KEY_ESCAPE)) {
    glfwPollEvents();

    xevWindow.addInfoToTitle("FPS: " + std::to_string(timeManager.getFps()));

    // Input handling TODO: use component
    glm::vec2 mouseDelta = xevWindow.getMouseAccumDelta();
    xevController.mouseLook(
        xevWindow.getGLFWwindow(), frameTime, mouseDelta.x, mouseDelta.y, viewTransform);
    xevController.moveInPlaneXZ(xevWindow.getGLFWwindow(), frameTime, viewTransform);

    camera.setViewYXZ(viewTransform.translation, viewTransform.rotation);

    // Update Camera  TODO: use component
    float fov = camera.updateFov(xevWindow.getScrollDelta());
    //    camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
    camera.setPerspectiveProjection(
        glm::radians(fov), xevRenderer.getAspectRatio(), 0.1f, 1e10);

    fixedTick(frameTime);

    // TODO: emcapsulate this in a function
    if (auto commandBuffer = xevRenderer.beginFrame()) {
      int frameIndex = xevRenderer.getFrameIndex();
      FrameInfo frameInfo{
          frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

      // update global UBO
      GlobalUbo ubo{};
      ubo.projection  = camera.getProjection();
      ubo.view        = camera.getView();
      ubo.inverseView = camera.getInverseView();
      pointLightSystem.update(frameInfo, ubo);
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      xevRenderer.beginSwapChainRenderPass(commandBuffer);

      basicRenderSystem.render(frameInfo);
      pointLightSystem.render(frameInfo);

      xevRenderer.endSwapChainRenderPass(commandBuffer);
      xevRenderer.endFrame();
    }
    timeManager.step();
  }
  std::cout << "Average FPS: " << timeManager.getFps() << std::endl;

  vkDeviceWaitIdle(xevDevice.device());
}

void DemoApp::load() {
  std::shared_ptr<XevModel> xevModel;

  // {
  //   std::string modelPath = "MountainTerrain.obj";
  //   xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath + modelPath);
  //
  //   Entity terrainEntity = entityManager.createEntity();
  //
  //   TransformComponent terrainTransform;
  //   terrainTransform.scale       = glm::vec3{1.0f};
  //   terrainTransform.translation = glm::vec3{0, 0, 0};
  //   terrainTransform.rotation.x  = glm::radians(180.f); // TODO: auto rotation on load
  //   entityManager.addComponent(terrainEntity, terrainTransform);
  //
  //   // Set ModelComponent for the terrain (the model loaded earlier)
  //   ModelComponent terrainModel;
  //   terrainModel.model = xevModel;
  //   entityManager.addComponent(terrainEntity, terrainModel);
  // }
  //
  // {
  //   std::string modelPath = "Hogwarts.obj";
  //   xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath + modelPath);
  //   Entity hogwartsEntity = entityManager.createEntity();
  //
  //   TransformComponent hogwartsTransform;
  //   hogwartsTransform.scale       = glm::vec3{0.01f};
  //   hogwartsTransform.rotation.x  = glm::radians(180.f);
  //   hogwartsTransform.translation = glm::vec3{0, 0, 0};
  //   entityManager.addComponent(hogwartsEntity, hogwartsTransform);
  //
  //   ModelComponent hogwartsModel;
  //   hogwartsModel.model = xevModel;
  //   entityManager.addComponent(hogwartsEntity, hogwartsModel);
  // }

  { // Floor
    Entity floorEntity = entityManager.createEntity();
    xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath + "quad.obj");
    ModelComponent floorModel;
    floorModel.model = xevModel;
    entityManager.addComponent(floorEntity, floorModel);

    TransformComponent floorTransform;
    floorTransform.translation = glm::vec3{0, 10, 0};
    floorTransform.scale       = glm::vec3{1000.f};
    entityManager.addComponent(floorEntity, floorTransform);
  }

  { // Point light
    std::vector<glm::vec3> lightColors{{1.f, .1f, .1f}, {.1f, .1f, 1.f}, {.1f, 1.f, .1f},
                                       {1.f, 1.f, .1f}, {.1f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    for (int i = 0; i < lightColors.size(); i++) {
      Entity pointLightEntity = entityManager.createEntity();

      PointLightComponent pointLight;
      pointLight.color     = lightColors[i % lightColors.size()];
      pointLight.intensity = 100.f;
      entityManager.addComponent(pointLightEntity, pointLight);

      TransformComponent pointLightTransform;
      auto rotateLight = glm::rotate(
          glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(),
          {0.f, -1.f, 0.f});
      pointLightTransform.translation =
          rotateLight * glm::vec4(0.f, 0.f, -1.f, 10.f) * 10.f;
      entityManager.addComponent(pointLightEntity, pointLightTransform);
    }
  }
}

void DemoApp::tick(float dt) {

  float theta = dt * 2;
  glm::mat3 rot;
  rot[0] = {cos(theta), 0, sin(theta)};
  rot[1] = {0, 1, 0};
  rot[2] = {-sin(theta), 0, cos(theta)};

  for (auto entity :
       entityManager
           .getEntitiesWithComponents<PointLightComponent, TransformComponent>()) {
    auto& transform       = entityManager.getComponent<TransformComponent>(entity);
    transform.translation = rot * transform.translation;
  }
}

void DemoApp::fixedTick(float dt) {
  // maybe better implementation
  static float timeLag = 0;
  timeLag += dt;

  while (timeLag >= TimeManager::FIXED_TIME_STEP) {
    tick(TimeManager::FIXED_TIME_STEP);
    timeLag -= TimeManager::FIXED_TIME_STEP;
  }
}

} // namespace xev
