#pragma once

#include "lge_device.h"

#include <string>
#include <vector>

namespace lge {

    struct PipelineConfigInfo {};

    class LgePipeline {
        public:
        LgePipeline(
                LgeDevice& device,
                const std::string& vertex_shader_path,
                const std::string& fragment_shader_path,
                const PipelineConfigInfo& configInfo);
        ~LgePipeline() {}

        LgePipeline(const LgePipeline&) = delete;
        void operator=(const LgePipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
        static std::vector<char> readFile(const std::string& path);

        void createGraphicsPipeline(
                const std::string& vertex_shader_path,
                const std::string& fragment_shader_path,
                const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        LgeDevice& lgeDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };

}