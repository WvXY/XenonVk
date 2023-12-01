#include "lge_pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <filesystem>


namespace lge {
    LgePipeline::LgePipeline(
            LgeDevice &device,
            const std::string &vertex_shader_path,
            const std::string &fragment_shader_path,
            const PipelineConfigInfo &configInfo) : lgeDevice{device} {
      createGraphicsPipeline(vertex_shader_path, fragment_shader_path, configInfo);
    }


    std::vector<char> LgePipeline::readFile(const std::string &path) {

      std::ifstream file{path, std::ios::ate | std::ios::binary};

      if (!file.is_open()) {
        std::cout << "Error: File do not exist or could not open" << std::endl;
        throw std::runtime_error("Could not open file: " + path);
      }

      size_t fileSize = static_cast<size_t>(file.tellg());
      std::vector<char> buffer(fileSize);

      file.seekg(0);
      file.read(buffer.data(), fileSize);

      file.close();

      return buffer;
    }

    void LgePipeline::createGraphicsPipeline(
            const std::string &vertex_shader_path,
            const std::string &fragment_shader_path,
            const PipelineConfigInfo &configInfo) {

      auto vertex_shader_code = readFile(vertex_shader_path);
      auto fragment_shader_code = readFile(fragment_shader_path);

      std::cout << "vertex shader: " << vertex_shader_code.size() << " bytes" << std::endl;
      std::cout << "fragment shader: " << fragment_shader_code.size() << " bytes" << std::endl;
    }

    void LgePipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule) {
      VkShaderModuleCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      createInfo.codeSize = code.size();
      createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

      if (vkCreateShaderModule(lgeDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
      }
    }

    PipelineConfigInfo LgePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
      PipelineConfigInfo configInfo{};

      return configInfo;
    }
} // namespace lge