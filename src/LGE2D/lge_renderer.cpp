#include "lge_renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lge {

LgeRenderer::LgeRenderer(LgeWindow& window, LgeDevice& device)
    : lgeWindow{window}, lgeDevice{device} {
  recreateSwapChain();
  createCommandBuffers();
}

LgeRenderer::~LgeRenderer() {
  freeCommandBuffers();
}

void LgeRenderer::recreateSwapChain() {
  auto extent = lgeWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = lgeWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(lgeDevice.device());

  if (lgeSwapChain == nullptr) {
    lgeSwapChain = std::make_unique<LgeSwapChain>(lgeDevice, extent);
  } else {
    std::shared_ptr<LgeSwapChain> oldSwapChain = std::move(lgeSwapChain);
    lgeSwapChain = std::make_unique<LgeSwapChain>(lgeDevice, extent, oldSwapChain);

    if (!oldSwapChain->compareSwapFormats(*lgeSwapChain.get())) {
      throw std::runtime_error("Swap chain image(or depth) format has changed!");
    }
  }
}

void LgeRenderer::createCommandBuffers() {
  commandBuffers.resize(LgeSwapChain::MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool        = lgeDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(lgeDevice.device(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void LgeRenderer::freeCommandBuffers() {
  vkFreeCommandBuffers(
      lgeDevice.device(), lgeDevice.getCommandPool(),
      static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
  commandBuffers.clear();
}

VkCommandBuffer LgeRenderer::beginFrame() {
  assert(!isFrameStarted && "Can't call beginFrame while already in progress");

  auto result = lgeSwapChain->acquireNextImage(&currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  isFrameStarted = true;

  auto commandBuffer = getCurrentCommandBuffer();
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  return commandBuffer;
}

void LgeRenderer::endFrame() {
  assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  auto result = lgeSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      lgeWindow.wasWindowResized()) {
    lgeWindow.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  isFrameStarted    = false;
  currentFrameIndex = (currentFrameIndex + 1) % LgeSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void LgeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(
      isFrameStarted &&
      "Can't call beginSwapChainRenderPass if frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't begin render pass on command buffer from a different frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass  = lgeSwapChain->getRenderPass();
  renderPassInfo.framebuffer = lgeSwapChain->getFrameBuffer(currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = lgeSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color           = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil    = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues    = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x        = 0.0f;
  viewport.y        = 0.0f;
  viewport.width    = static_cast<float>(lgeSwapChain->getSwapChainExtent().width);
  viewport.height   = static_cast<float>(lgeSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{
      {0, 0},
      lgeSwapChain->getSwapChainExtent()
  };
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void LgeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(
      isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't end render pass on command buffer from a different frame");
  vkCmdEndRenderPass(commandBuffer);
}

} // namespace lge
