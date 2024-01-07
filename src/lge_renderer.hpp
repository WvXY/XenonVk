#pragma once

#include <cassert>
#include <memory>
#include <vector>

#include "lge_device.hpp"
#include "lge_swap_chain.hpp"
#include "lge_window.hpp"

namespace lge {
class LgeRenderer {
 public:
  LgeRenderer(LgeWindow& window, LgeDevice& device);
  ~LgeRenderer();
  LgeRenderer(const LgeRenderer&) = delete;
  LgeRenderer& operator=(const LgeRenderer&) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return lgeSwapChain->getRenderPass();
  }
  bool isFrameInProgress() const { return isFrameStarted; }

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted &&
           "Cannot get frame index when frame not in progress");
    return currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

 private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  LgeWindow& lgeWindow;
  LgeDevice& lgeDevice;
  std::unique_ptr<LgeSwapChain> lgeSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex{0};
  int currentFrameIndex{0};
  bool isFrameStarted{false};
};
}  // namespace lge
