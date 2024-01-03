#include "first_app.hpp"

//std
#include <array>
#include <cassert>
#include <stdexcept>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace lge {

    struct SimplePushConstantData {
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    FirstApp::FirstApp() {
      loadModel();
      createPipelineLayout();
      recreateSwapChain();
      createCommandBuffers();
    }

    FirstApp::~FirstApp() {
      vkDestroyPipelineLayout(lgeDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
//      std::cout << "maxPushConstantSize: " << lgeDevice.properties.limits.maxPushConstantsSize << std::endl;

      while (!lgeWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
      }

      vkDeviceWaitIdle(lgeDevice.device());
    }

    void FirstApp::loadModel() {
      std::vector<LgeModel::Vertex> vertices{
              {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
              {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
              {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
      };
      lgeModel = std::make_unique<LgeModel>(lgeDevice, vertices);
    }

    void FirstApp::createPipelineLayout() {

      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      pushConstantRange.size = sizeof(SimplePushConstantData);

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pSetLayouts = nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 1;
      pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
      if (vkCreatePipelineLayout(
              lgeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout)
              != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
      }
    }

    void FirstApp::createPipeline() {
      assert(lgeSwapChain != nullptr && "Cannot create pipeline before swap chain");
      assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

      PipelineConfigInfo pipelineConfig{};
      LgePipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.pipelineLayout = pipelineLayout;
      pipelineConfig.renderPass = lgeSwapChain->getRenderPass();
      lgePipeline = std::make_unique<LgePipeline>(
              lgeDevice, vertShaderPath, fragShaderPath, pipelineConfig
      );
    }

    void FirstApp::recreateSwapChain() {
      auto extent = lgeWindow.getExtent();
      while (extent.width == 0 || extent.height == 0) {
        extent = lgeWindow.getExtent();
        glfwWaitEvents();
      }
      vkDeviceWaitIdle(lgeDevice.device());

      if (lgeSwapChain == nullptr) {
        lgeSwapChain = std::make_unique<LgeSwapChain>(lgeDevice, extent);
      } else {
        lgeSwapChain = std::make_unique<LgeSwapChain>(lgeDevice, extent, std::move(lgeSwapChain));
        if (lgeSwapChain->imageCount() != commandBuffers.size()) {
          freeCommandBuffers();
          createCommandBuffers();
        }
      }
      createPipeline();
    }

    void FirstApp::createCommandBuffers() {
      commandBuffers.resize(lgeSwapChain->imageCount());

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = lgeDevice.getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if (vkAllocateCommandBuffers(lgeDevice.device(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
      }
    }

    void FirstApp::freeCommandBuffers() {
      vkFreeCommandBuffers(
              lgeDevice.device(),
              lgeDevice.getCommandPool(),
              static_cast<uint32_t>(commandBuffers.size()),
              commandBuffers.data()
      );
      commandBuffers.clear();
    }

    void FirstApp::recordCommandBuffer(int imageIndex) {
      static int frame = 0;
      frame = (++frame) % 100;

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = lgeSwapChain->getRenderPass();
      renderPassInfo.framebuffer = lgeSwapChain->getFrameBuffer(imageIndex);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = lgeSwapChain->getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = static_cast<float>(lgeSwapChain->getSwapChainExtent().width);
      viewport.height = static_cast<float>(lgeSwapChain->getSwapChainExtent().height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      VkRect2D scissor{{0, 0}, lgeSwapChain->getSwapChainExtent()};
      vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
      vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

      lgePipeline->bind(commandBuffers[imageIndex]);
      lgeModel->bind(commandBuffers[imageIndex]);

      for (int j = 0; j < 4; j++) {
        SimplePushConstantData push{};
        push.offset = {-0.5f + frame * 0.02f, -0.4f + j * 0.25f};
        push.color = {0.0f, 0.0f, 0.2f + j * 0.2f};

        vkCmdPushConstants(
            commandBuffers[imageIndex],
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push);

        lgeModel->draw(commandBuffers[imageIndex]);
      }


      vkCmdEndRenderPass(commandBuffers[imageIndex]);
      if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
      }
    }

    void FirstApp::drawFrame() {
      uint32_t imageIndex;
      auto result = lgeSwapChain->acquireNextImage(&imageIndex);

      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
      }

      if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
      }

      recordCommandBuffer(imageIndex);
      result = lgeSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
      if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
        || lgeWindow.wasWindowResized()) {
        lgeWindow.resetWindowResizedFlag();
        recreateSwapChain();
        return;
      }

      if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
      }
    }

}
