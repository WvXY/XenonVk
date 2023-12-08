#include "lge_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <filesystem>
#include <cassert>


namespace lge {
    LgePipeline::LgePipeline(
            LgeDevice &device,
            const std::string &vertex_shader_path,
            const std::string &fragment_shader_path,
            const PipelineConfigInfo &configInfo) : lgeDevice{device} {
      createGraphicsPipeline(vertex_shader_path, fragment_shader_path, configInfo);
    }

    LgePipeline::~LgePipeline() {
      vkDestroyShaderModule(lgeDevice.device(), vertShaderModule, nullptr);
      vkDestroyShaderModule(lgeDevice.device(), fragShaderModule, nullptr);
      vkDestroyPipeline(lgeDevice.device(), graphicsPipeline, nullptr);
    }

    void LgePipeline::bind(VkCommandBuffer commandBuffer) {
      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
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

      assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
        "cannot create graphics pipeline:: no pipelineLayout provided in configInfo");

      assert(configInfo.renderPass != VK_NULL_HANDLE &&
        "cannot create graphics pipeline:: no renderPass provided in configInfo");

      auto vertex_shader_code = readFile(vertex_shader_path);
      auto fragment_shader_code = readFile(fragment_shader_path);

      createShaderModule(vertex_shader_code, &vertShaderModule);
      createShaderModule(fragment_shader_code, &fragShaderModule);

      VkPipelineShaderStageCreateInfo shaderStages[2];
      shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
      shaderStages[0].module = vertShaderModule;
      shaderStages[0].pName = "main";
      shaderStages[0].flags = 0;
      shaderStages[0].pNext = nullptr;
      shaderStages[0].pSpecializationInfo = nullptr;
      shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      shaderStages[1].module = fragShaderModule;
      shaderStages[1].pName = "main";
      shaderStages[1].flags = 0;
      shaderStages[1].pNext = nullptr;
      shaderStages[1].pSpecializationInfo = nullptr;

      VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      vertexInputInfo.vertexAttributeDescriptionCount = 0;
      vertexInputInfo.vertexBindingDescriptionCount = 0;
      vertexInputInfo.pVertexAttributeDescriptions = nullptr;
      vertexInputInfo.pVertexBindingDescriptions = nullptr;

      VkPipelineViewportStateCreateInfo viewportInfo{};
      viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportInfo.viewportCount = 1;
      viewportInfo.pViewports = &configInfo.viewport;
      viewportInfo.scissorCount = 1;
      viewportInfo.pScissors = &configInfo.scissor;

      VkGraphicsPipelineCreateInfo pipelineInfo{};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = 2;
      pipelineInfo.pStages = shaderStages;
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
      pipelineInfo.pViewportState = &viewportInfo;
      pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
      pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
      pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
      pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
      pipelineInfo.pDynamicState = nullptr;

      pipelineInfo.layout = configInfo.pipelineLayout;  // Not used yet
      pipelineInfo.renderPass = configInfo.renderPass;  // Not used yet
      pipelineInfo.subpass = configInfo.subpass;        // Not used yet

      pipelineInfo.basePipelineIndex = -1;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

      if (vkCreateGraphicsPipelines(
              lgeDevice.device(),
              VK_NULL_HANDLE,
              1,
              &pipelineInfo,
              nullptr,
              &graphicsPipeline
              ) != VK_SUCCESS)
      {
        throw std::runtime_error("failed to create graphics pipeline");
      }


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

      configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

      configInfo.viewport.x = 0.0f;
      configInfo.viewport.y = 0.0f;
      configInfo.viewport.width = static_cast<float>(width);
      configInfo.viewport.height = static_cast<float>(height);
      configInfo.viewport.minDepth = 0.0f;
      configInfo.viewport.maxDepth = 1.0f;

      configInfo.scissor.offset = {0, 0};
      configInfo.scissor.extent = {width, height};

      configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
      configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
      configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
      configInfo.rasterizationInfo.lineWidth = 1.0f;
      configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
      configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
      configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
      configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
      configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
      configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

      configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
      configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
      configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
      configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
      configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
      configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

      configInfo.colorBlendAttachment.colorWriteMask =
              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
              VK_COLOR_COMPONENT_A_BIT;
      configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
      configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
      configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
      configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
      configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
      configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
      configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

      configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
      configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
      configInfo.colorBlendInfo.attachmentCount = 1;
      configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
      configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
      configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
      configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
      configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

      configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
      configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
      configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
      configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
      configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
      configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
      configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
      configInfo.depthStencilInfo.front = {};  // Optional
      configInfo.depthStencilInfo.back = {};   // Optional

      return configInfo;
    }
} // namespace lge