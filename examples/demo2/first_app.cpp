#include "first_app.hpp"
#include "simple_render_system.hpp"

// std
#include <array>
#include <cassert>
#include <iostream>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lge2 {

struct SimplePushConstantData {
  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

FirstApp::FirstApp() {
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{lgeDevice, lgeRenderer.getSwapChainRenderPass()};

  while (!lgeWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = lgeRenderer.beginFrame()) {
      lgeRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      lgeRenderer.endSwapChainRenderPass(commandBuffer);
      lgeRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lgeDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<LgeModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lgeModel = std::make_shared<LgeModel>(lgeDevice, vertices);

  for (int i = 0; i < 20; i++) {
    auto triangle  = LgeGameObject::createGameObject();
    triangle.model = lgeModel;
    triangle.color = glm::vec3(1.0f * sin(i), 1.f * cos(i), 1.0f * sin(i * 0.5f));
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale         = glm::vec2(.5f) + i * 0.05f;
    triangle.transform2d.rotation      = glm::two_pi<float>() * 0.25f * i / 5.0f;

    gameObjects.push_back(std::move(triangle));
  }
}

} // namespace lge
