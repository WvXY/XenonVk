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

    // Input handling
    glm::vec2 mouseDelta = xevWindow.getMouseAccumDelta();
    xevController.mouseLook(
        xevWindow.getGLFWwindow(), frameTime, mouseDelta.x, mouseDelta.y, viewTransform);
    xevController.moveInPlaneXZ(xevWindow.getGLFWwindow(), frameTime, viewTransform);

    camera.setViewYXZ(viewTransform.translation, viewTransform.rotation);

    // Update Camera
    float fov = camera.updateFov(xevWindow.getScrollDelta());
    //    camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
    camera.setPerspectiveProjection(
        glm::radians(fov), xevRenderer.getAspectRatio(), 0.1f, 1e10);

    // updateGameObjects(frameTime);
    fixedTick(timeManager.getTimeLag());

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
  {
    std::string modelPath = "MountainTerrain.obj";
    xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath + modelPath);

    Entity terrainEntity = entityManager.createEntity();

    TransformComponent terrainTransform;
    terrainTransform.scale       = glm::vec3{1.0f};
    terrainTransform.translation = glm::vec3{0, 0, 0};
    terrainTransform.rotation.x  = glm::radians(180.f); // TODO: auto rotation on load
    entityManager.addComponent(terrainEntity, terrainTransform);

    // Set ModelComponent for the terrain (the model loaded earlier)
    ModelComponent terrainModel;
    terrainModel.model = xevModel;
    entityManager.addComponent(terrainEntity, terrainModel);
  }

  {
    std::string modelPath = "Hogwarts.obj";
    xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath + modelPath);
    Entity hogwartsEntity = entityManager.createEntity();

    TransformComponent hogwartsTransform;
    hogwartsTransform.scale       = glm::vec3{0.01f};
    hogwartsTransform.rotation.x  = glm::radians(180.f);
    hogwartsTransform.translation = glm::vec3{0, 0, 0};
    entityManager.addComponent(hogwartsEntity, hogwartsTransform);

    ModelComponent hogwartsModel;
    hogwartsModel.model = xevModel;
    entityManager.addComponent(hogwartsEntity, hogwartsModel);
  }

  // { // Floor
  //   xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath +
  //   "quad.obj"); auto gameObject                  = XevGameObject::createGameObject();
  //   gameObject.model                 = xevModel;
  //   gameObject.transform.translation = {0, 0.2f, 0.0f};
  //   gameObject.transform.scale       = glm::vec3{10.0f};
  //   gameObjects.emplace(gameObject.getId(), std::move(gameObject));
  // }

  // { // Point light
  //   std::vector<glm::vec3> lightColors{{1.f, .1f, .1f}, {.1f, .1f, 1.f}, {.1f, 1.f,
  //   .1f},
  //                                      {1.f, 1.f, .1f}, {.1f, 1.f, 1.f},
  //                                      {1.f, 1.f, 1.f}};
  //
  //   for (int i = 0; i < lightColors.size(); i++) {
  //     auto pointLight  = XevGameObject::makePointLight(1000.f);
  //     pointLight.color = lightColors[i % lightColors.size()];
  //     auto rotateLight = glm::rotate(
  //         glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(),
  //         {0.f, -1.f, 0.f});
  //     pointLight.transform.translation =
  //         rotateLight * vec4(0.f, 0.f, -1.f, 10.f) * 100.f;
  //     gameObjects.emplace(pointLight.getId(), std::move(pointLight));
  //   }
  // }
  // { // Sun
  //   auto sunLight =
  //       XevGameObject::makePointLight(1000.f, 1000.f, vec3{255, 221, 64} / 255.f);
  //   sunLight.transform.translation = {2400.f, -3000.f, 6000.f};
  //   gameObjects.emplace(sunLight.getId(), std::move(sunLight));
  // }
}

void DemoApp::tick(float dt) {
  for (auto& kv : actorManager.getActors()) {
    auto& gameObject = kv.second;
    if (entityManager.hasComponent<TransformComponent>(gameObject.getEntityId())) {
      auto& transform =
          entityManager.getComponent<TransformComponent>(gameObject.getEntityId());

      transform.translation += glm::vec3{0.f, 0.f, 0.1f} * dt;

      // TODO: use systems to manage update(physics, etc)
    }
  }
}

void DemoApp::fixedTick(float& timeLag) {
  while (timeLag >= TimeManager::FIXED_TIME_STEP) {

    // updatePhysics(TimeManager::FIXED_TIME_STEP);
    tick(TimeManager::FIXED_TIME_STEP);

    timeLag -= TimeManager::FIXED_TIME_STEP;
  }
}

} // namespace xev
