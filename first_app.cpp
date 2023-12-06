#include "first_app.hpp"

namespace lge {

    FirstApp::FirstApp() {
      createPipelineLayout();
      createPipeline();
      createCommandBuffers();
    }

    FirstApp::~FirstApp() {
      vkDestroyPipelineLayout(lgeDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
      while (!lgeWindow.shouldClose()) {
        glfwPollEvents();
      }
    }

    void FirstApp::createPipelineLayout() {
      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pSetLayouts = nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 0;
      pipelineLayoutInfo.pPushConstantRanges = nullptr;
      if (vkCreatePipelineLayout(
              lgeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout)
          != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
      }
    }

    void FirstApp::createPipeline() {
      auto pipelineConfig =
              LgePipeline::defaultPipelineConfigInfo(lgeSwapChain.width(), lgeSwapChain.height());
      pipelineConfig.renderPass = lgeSwapChain.getRenderPass();
      pipelineConfig.pipelineLayout = pipelineLayout;
      lgePipeline = std::make_unique<LgePipeline>(
              lgeDevice,
              rootPath + vertShaderPath,
              rootPath + fragShaderPath,
              pipelineConfig
      );
    }

    void FirstApp::createCommandBuffers() {}

    void FirstApp::drawFrame() {}

}
