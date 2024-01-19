#include "first_app.hpp"

#include "kbd_controller.hpp"
#include "lge_buffer.hpp"
#include "lge_camera.hpp"
#include "simple_render_system.hpp"

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

#define MAX_FRAME_TIME 0.5f

namespace lge {

struct GlobalUbo {
  glm::mat4 projectionView{1.f};
  glm::vec3 lightDirection{0.f, -3.f, -1.f};
};

FirstApp::FirstApp() {
  globalPool =
      LgeDescriptorPool::Builder(lgeDevice)
          .setMaxSets(LgeSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(
              VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LgeSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  std::vector<std::unique_ptr<LgeBuffer>> uboBuffers(LgeSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<LgeBuffer>(
        lgeDevice, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffers[i]->map();
  }

  auto globalSetLayout =
      LgeDescriptorSetLayout::Builder(lgeDevice)
          .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
          .build();
  std::vector<VkDescriptorSet> globalDescriptorSets(LgeSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    LgeDescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{
      lgeDevice, lgeRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  LgeCamera camera{};
  //    camera.setViewDirection({0.f, 0.f, 0.f}, {1.f, 0.f, 1.f});
  glm::vec3 cameraTarget = gameObjects[0].transform.translation;
  camera.setViewTarget({-1.f, -3.f, 1.f}, cameraTarget);

  auto viewerObject = LgeGameObject::createGameObject();
  KbdController kbdController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!lgeWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
            .count();
    currentTime = newTime;
    frameTime   = std::min(frameTime, MAX_FRAME_TIME);

    kbdController.moveInPlaneXZ(lgeWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(
        viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lgeRenderer.getAspectRatio();
    //    camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
    camera.setPerspectiveProjection(glm::radians(60.f), aspect, 0.1f, 10.f);

    if (auto commandBuffer = lgeRenderer.beginFrame()) {
      int frameIndex = lgeRenderer.getFrameIndex();
      FrameInfo frameInfo{
          frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

      // update global UBO
      GlobalUbo ubo{};
      ubo.projectionView = camera.getProjection() * camera.getView();
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      lgeRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
      lgeRenderer.endSwapChainRenderPass(commandBuffer);
      lgeRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lgeDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<std::string> modelPaths = {
      "../../models/bunny.obj", "../../models/cube.obj", "../../models/colored_cube.obj",
      "../../models/smooth_vase.obj", "../../models/flat_vase.obj"};
  float x = -2.f;
  for (std::string& modelPath : modelPaths) {
    std::shared_ptr<LgeModel> lgeModel =
        LgeModel::createModelFromFile(lgeDevice, modelPath);
    auto gameObject                  = LgeGameObject::createGameObject();
    gameObject.model                 = lgeModel;
    gameObject.transform.translation = {x, 0.f, 1.0f};
    gameObject.transform.scale       = glm::vec3{1.0f};
    gameObjects.push_back(std::move(gameObject));
    x += .8f;
  }
  gameObjects[0].transform.scale = {2.5f, 2.5f, 2.5f};  // bunny do not have uv coords so it is
                                                        // rendered as a dark silhouette
  gameObjects[1].transform.scale = {0.2f, 0.2f, 0.2f};
  gameObjects[2].transform.scale = {0.2f, 0.2f, 0.2f};
}

} // namespace lge
