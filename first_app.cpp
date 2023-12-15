#include "first_app.hpp"

//std
#include <array>
#include <stdexcept>

namespace lge {

    FirstApp::FirstApp() {
      loadModel();
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
        drawFrame();
      }

      vkDeviceWaitIdle(lgeDevice.device());
    }

    void FirstApp::loadModel() {
      std::vector<LgeModel::Vertex> vertices{
              {{0.0f, -0.5f}},
              {{0.5f, 0.5f}},
              {{-0.5f, 0.5f}}
      };
      lgeModel = std::make_unique<LgeModel>(lgeDevice, vertices);
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

    void FirstApp::createCommandBuffers() {
      commandBuffers.resize(lgeSwapChain.imageCount());

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = lgeDevice.getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if (vkAllocateCommandBuffers(lgeDevice.device(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
      }

      for (int i=0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
          throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = lgeSwapChain.getRenderPass();
        renderPassInfo.framebuffer = lgeSwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = lgeSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);



        lgePipeline->bind(commandBuffers[i]);
        lgeModel->bind(commandBuffers[i]);
        lgeModel->draw(commandBuffers[i]);

        vkCmdEndRenderPass(commandBuffers[i]);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
          throw std::runtime_error("failed to record command buffer!");
        }
      }
    }

    void FirstApp::drawFrame() {
      uint32_t imageIndex;
      auto result = lgeSwapChain.acquireNextImage(&imageIndex);
      if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
      }

      result = lgeSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
      if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
      }
    }

}
