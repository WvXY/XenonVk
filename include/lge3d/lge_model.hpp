#pragma once

#include "lge_buffer.hpp"
#include "lge_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace lge {
class LgeModel {
public:
  struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 uv{};

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

    bool operator==(const Vertex& other) const {
      return position == other.position && color == other.color &&
             normal == other.normal && uv == other.uv;
    }
  };

  struct Builder {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

    void loadModel(const std::string& filepath);
  };

  LgeModel(LgeDevice& device, const LgeModel::Builder& builder);
  ~LgeModel();
  LgeModel(const LgeModel&)            = delete;
  LgeModel& operator=(const LgeModel&) = delete;

  static std::unique_ptr<LgeModel>
  createModelFromFile(LgeDevice& device, const std::string& filepath);

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffers(const std::vector<Vertex>& vertices);
  void createIndexBuffers(const std::vector<uint32_t>& indices);

  LgeDevice& lgeDevice;

  std::unique_ptr<LgeBuffer> vertexBuffer;
  uint32_t vertexCount;

  bool hasIndexBuffer = false;
  std::unique_ptr<LgeBuffer> indexBuffer;
  uint32_t indexCount;
};
} // namespace lge
