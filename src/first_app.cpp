#include "first_app.hpp"

#include "kbd_controller.hpp"
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

struct SimplePushConstantData {
  glm::mat4 transform{1.0f};
  alignas(16) glm::vec3 color;
};

FirstApp::FirstApp() {
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{lgeDevice, lgeRenderer.getSwapChainRenderPass()};
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
      lgeRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
      lgeRenderer.endSwapChainRenderPass(commandBuffer);
      lgeRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lgeDevice.device());
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<LgeModel> lgeModel =
      LgeModel::createModelFromFile(lgeDevice, "../../models/colored_cube.obj");

  auto gameObject                  = LgeGameObject::createGameObject();
  gameObject.model                 = lgeModel;
  gameObject.transform.translation = {0.f, 0.f, 1.0f};
  gameObject.transform.scale       = {0.5f, 0.5f, 0.5f};
  gameObjects.push_back(std::move(gameObject));
}

} // namespace lge
