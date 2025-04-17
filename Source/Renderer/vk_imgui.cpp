#include "vk_imgui.hpp"

xev::XevImGui::XevImGui(XevWindow& window, XevDevice& device, XevRenderer& renderer)
    : window{window}, device{device}, renderer{renderer} {
  setupContext();
}

void xev::XevImGui::setupContext() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
}

void xev::XevImGui::initImGuiForVulkan(VkDescriptorPool descriptorPool) {
  ImGui_ImplGlfw_InitForVulkan(window.getGLFWwindow(), true);
  init_info.Instance       = device.getVkInstance();
  init_info.PhysicalDevice = device.getPhysicalDevice();
  init_info.Device         = device.device();
  init_info.QueueFamily    = device.findPhysicalQueueFamilies().presentFamily;
  init_info.Queue          = device.presentQueue();
  init_info.RenderPass     = renderer.getSwapChainRenderPass();
  // init_info.PipelineCache   = YOUR_PIPELINE_CACHE; // optional
  init_info.DescriptorPool = descriptorPool;
  // init_info.Subpass         = 0;   // optional
  init_info.MinImageCount = 2;
  init_info.ImageCount    = 2;
  init_info.MSAASamples   = VK_SAMPLE_COUNT_1_BIT;
  // init_info.Allocator       = YOUR_ALLOCATOR;   // optional
  // init_info.CheckVkResultFn = check_vk_result;  // optional

  ImGui_ImplVulkan_Init(&init_info);
  // ImGui_ImplVulkan_CreateFontsTexture();
}