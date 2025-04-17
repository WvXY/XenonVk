#pragma once

#include "vk_descriptors.hpp"
#include "vk_device.hpp"
#include "vk_renderer.hpp"
#include "vk_window.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace xev {

class XevImGui {
public:
  XevImGui(const XevImGui&) = delete;
  XevImGui(XevWindow& window, XevDevice& device, XevRenderer& renderer);

  ~XevImGui() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  // For initialization
  void setupContext();
  void initImGuiForVulkan(VkDescriptorPool descriptorPool);

  // For loop
  void beginFrame() {
    // glfwPollEvents();  // same as glfwPollEvents(), so just do noting.
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(&showDemoWindow);
  }
  void endFrame(VkCommandBuffer commandBuffer) {
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
  }

  bool showDemoWindow = true;

private:
  ImGui_ImplVulkan_InitInfo init_info = {};

  XevWindow& window;
  XevDevice& device;
  XevRenderer& renderer;
};

} // namespace xev