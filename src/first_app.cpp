#include "first_app.hpp"

#include "point_light_system.hpp"
#include "simple_render_system.hpp"
#include "time_manager.hpp"
#include "vk_buffer.hpp"
#include "vk_camera.hpp"
#include "xev_controller.hpp"

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

FirstApp::FirstApp() {
  globalPool =
      XevDescriptorPool::Builder(xevDevice)
          .setMaxSets(XevSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(
              VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, XevSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
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

  SimpleRenderSystem simpleRenderSystem{
      xevDevice, xevRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  PointLightSystem pointLightSystem{
      xevDevice, xevRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};

  XevCamera camera{};
  //    camera.setViewDirection({0.f, 0.f, 0.f}, {1.f, 0.f, 1.f});
  glm::vec3 cameraTarget = gameObjects.at(0).transform.translation;
  camera.setViewTarget({-1.f, -3.f, 1.f}, cameraTarget);

  auto viewerObject                    = XevGameObject::createGameObject();
  viewerObject.transform.translation.z = -2.f;
  XevController xevController{};

  // Time management
  timeManager.start();
  auto& frameTime = timeManager.frameTime;

  while (!xevWindow.shouldClose()) {
    glfwPollEvents();

    xevWindow.addInfoToTitle("FPS: " + std::to_string(timeManager.getFps()));

    // Input handling
    glm::vec2 mouseDelta = xevWindow.getMouseAccumDelta();
    xevController.mouseLook(
        xevWindow.getGLFWwindow(), frameTime, mouseDelta.x, mouseDelta.y, viewerObject);
    xevController.moveInPlaneXZ(xevWindow.getGLFWwindow(), frameTime, viewerObject);

    camera.setViewYXZ(
        viewerObject.transform.translation, viewerObject.transform.rotation);

    // Update Camera
    float fov = camera.updateFov(xevWindow.getScrollDelta());
    //    camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
    camera.setPerspectiveProjection(
        glm::radians(fov), xevRenderer.getAspectRatio(), 0.1f, 100.f);

    // updateGameObjects(frameTime);
    fixedUpdateGameObjects(timeManager.timeLag);

    if (auto commandBuffer = xevRenderer.beginFrame()) {
      int frameIndex = xevRenderer.getFrameIndex();
      FrameInfo frameInfo{
          frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex],
          gameObjects};

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

      simpleRenderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);

      xevRenderer.endSwapChainRenderPass(commandBuffer);
      xevRenderer.endFrame();
    }
    timeManager.step();
  }

  vkDeviceWaitIdle(xevDevice.device());
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<XevModel> xevModel;
  //  {
  //    xevModel = XevModel::createModelFromFile(
  //        xevDevice, relativeModelPath + "HugeCity/hugeCity.obj");
  //    auto gameObject                  = XevGameObject::createGameObject();
  //    gameObject.model                 = xevModel;
  //    gameObject.transform.translation = {0.f, 1.0f, 3.0f};
  //    gameObject.transform.scale       = glm::vec3{0.0003f};
  //    gameObject.transform.rotation.x  = glm::pi<float>();
  //    gameObject.transform.rotation.y  = glm::pi<float>();
  //    gameObjects.emplace(gameObject.getId(), std::move(gameObject));
  //  }
  {
    xevModel = XevModel::createModelFromFile(
        //        xevDevice, relativeModelPath + "HugeCity/hugeCity.obj");
        xevDevice, relativeModelPath + "LowPolyIsometricRoom/Room #1.obj");
    auto gameObject                  = XevGameObject::createGameObject();
    gameObject.model                 = xevModel;
    gameObject.transform.translation = {0.f, 1.0f, 0.0f};
    gameObject.transform.scale       = glm::vec3{0.3f};
    gameObject.transform.rotation.x  = glm::pi<float>();
    gameObject.transform.rotation.y  = glm::four_over_pi<float>();
    gameObjects.emplace(gameObject.getId(), std::move(gameObject));
  }
  //  {
  //    std::vector<std::string> modelPaths = {
  //        "bunny.obj", "cube.obj", "colored_cube.obj", "smooth_vase.obj",
  //        "flat_vase.obj"};
  //
  //    float x = -2.f;
  //    for (std::string& modelPath : modelPaths) {
  //      xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath +
  //      modelPath); auto gameObject                  =
  //      XevGameObject::createGameObject(); gameObject.model                 = xevModel;
  //      gameObject.transform.translation = {x, 0.f, 0.0f};
  //      gameObject.transform.scale       = glm::vec3{1.0f};
  //      gameObjects.emplace(gameObject.getId(), std::move(gameObject));
  //      x += .8f;
  //    } // bunny do not have uv coords, so it is rendered as a dark silhouette
  //
  //    gameObjects.at(0).transform.scale      = {2.5f, 2.5f, 2.5f};
  //    gameObjects.at(0).transform.rotation.z = glm::pi<float>();
  //    gameObjects.at(1).transform.scale      = {0.2f, 0.2f, 0.2f};
  //    gameObjects.at(2).transform.scale      = {0.2f, 0.2f, 0.2f};
  //    gameObjects.at(3).transform.scale      = {2.f, 2.f, 2.f};
  //    gameObjects.at(3).transform.translation.y += .2f;
  //    gameObjects.at(4).transform.scale = {2.f, 2.f, 2.f};
  //    gameObjects.at(4).transform.scale = {4.f, 2.f, 4.f};
  //    gameObjects.at(4).transform.translation.y += .2f;
  //  }
  //
  //  { // Floor
  //    xevModel = XevModel::createModelFromFile(xevDevice, relativeModelPath +
  //    "quad.obj"); auto gameObject                  = XevGameObject::createGameObject();
  //    gameObject.model                 = xevModel;
  //    gameObject.transform.translation = {0, 0.2f, 0.0f};
  //    gameObject.transform.scale       = glm::vec3{10.0f};
  //    gameObjects.emplace(gameObject.getId(), std::move(gameObject));
  //  }
  { // Point light
    std::vector<glm::vec3> lightColors{{1.f, .1f, .1f}, {.1f, .1f, 1.f}, {.1f, 1.f, .1f},
                                       {1.f, 1.f, .1f}, {.1f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    for (int i = 0; i < lightColors.size(); i++) {
      auto pointLight  = XevGameObject::makePointLight(0.2f);
      pointLight.color = lightColors[i];
      auto rotateLight = glm::rotate(
          glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(),
          {0.f, -1.f, 0.f});
      pointLight.transform.translation =
          glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
      gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
  }
}

void FirstApp::updateGameObjects(float dt) {
  for (auto& obj : gameObjects) {
    obj.second.update(dt);
  }
}

void FirstApp::fixedUpdateGameObjects(float& timeLag) {
  while (timeLag >= TimeManager::FIXED_TIME_STEP) {

    // updatePhysics(TimeManager::FIXED_TIME_STEP);
    updateGameObjects(TimeManager::FIXED_TIME_STEP);

    timeLag -= TimeManager::FIXED_TIME_STEP;
  }
}

} // namespace xev
