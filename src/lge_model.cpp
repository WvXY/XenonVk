#include "lge_model.hpp"

#include <cassert>
#include <cstring>

namespace lge {
LgeModel::LgeModel(LgeDevice& device, const LgeModel::Builder& builder)
    : lgeDevice{device} {
  createVertexBuffers(builder.vertices);
  createIndexBuffers(builder.indices);
}

LgeModel::~LgeModel() {
  vkDestroyBuffer(lgeDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(lgeDevice.device(), vertexBufferMemory, nullptr);

  if (hasIndexBuffer) {
    vkDestroyBuffer(lgeDevice.device(), indexBuffer, nullptr);
    vkFreeMemory(lgeDevice.device(), indexBufferMemory, nullptr);
  }
}

void LgeModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

  if (hasIndexBuffer) {
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
  }
}

void LgeModel::draw(VkCommandBuffer commandBuffer) {
  if (hasIndexBuffer) {
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    return;
  } else {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
  }
}

void LgeModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be at least 3");

  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  lgeDevice.createBuffer(
      bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer, stagingBufferMemory);

  void* data;
  vkMapMemory(lgeDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(lgeDevice.device(), stagingBufferMemory);

  lgeDevice.createBuffer(
      bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

  lgeDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

  vkDestroyBuffer(lgeDevice.device(), stagingBuffer, nullptr);
  vkFreeMemory(lgeDevice.device(), stagingBufferMemory, nullptr);
}

void LgeModel::createIndexBuffers(const std::vector<uint32_t>& indices) {
  indexCount = static_cast<uint32_t>(indices.size());
  hasIndexBuffer = indexCount > 0;
  if (!hasIndexBuffer) return;

  VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  lgeDevice.createBuffer(
      bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer, stagingBufferMemory);

  void* data;
  vkMapMemory(lgeDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(lgeDevice.device(), stagingBufferMemory);

  lgeDevice.createBuffer(
      bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

  lgeDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

  vkDestroyBuffer(lgeDevice.device(), stagingBuffer, nullptr);
  vkFreeMemory(lgeDevice.device(), stagingBufferMemory, nullptr);
}

std::vector<VkVertexInputBindingDescription> LgeModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
LgeModel::Vertex::getAttributeDescriptions() {
  return {// location, binding, format, offset
          {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
          {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}};
}
} // namespace lge
