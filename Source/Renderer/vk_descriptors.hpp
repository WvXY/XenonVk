#pragma once

#include "vk_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace xev {

class XevDescriptorSetLayout {
public:
  class Builder {
  public:
    Builder(XevDevice& xevDevice) : xevDevice{xevDevice} {}

    Builder& addBinding(
        uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags,
        uint32_t count = 1);

    std::unique_ptr<XevDescriptorSetLayout> build() const;

  private:
    XevDevice& xevDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  XevDescriptorSetLayout(
      XevDevice& xevDevice,
      std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

  ~XevDescriptorSetLayout();

  XevDescriptorSetLayout(const XevDescriptorSetLayout&) = delete;

  XevDescriptorSetLayout& operator=(const XevDescriptorSetLayout&) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
  XevDevice& xevDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class XevDescriptorWriter;
};

class XevDescriptorPool {
public:
  class Builder {
  public:
    Builder(XevDevice& xevDevice) : xevDevice{xevDevice} {}

    Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);

    Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);

    Builder& setMaxSets(uint32_t count);

    std::unique_ptr<XevDescriptorPool> build() const;

  private:
    XevDevice& xevDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets                      = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  XevDescriptorPool(
      XevDevice& xevDevice, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize>& poolSizes);
  ~XevDescriptorPool();
  XevDescriptorPool(const XevDescriptorPool&)            = delete;
  XevDescriptorPool& operator=(const XevDescriptorPool&) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
  void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
  void resetPool();

private:
  XevDevice& xevDevice;
  VkDescriptorPool descriptorPool;

  friend class XevDescriptorWriter;
};

class XevDescriptorWriter {
public:
  XevDescriptorWriter(XevDescriptorSetLayout& setLayout, XevDescriptorPool& pool);
  XevDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
  XevDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

  bool build(VkDescriptorSet& set);
  void overwrite(VkDescriptorSet& set);

private:
  XevDescriptorSetLayout& setLayout;
  XevDescriptorPool& pool;
  std::vector<VkWriteDescriptorSet> writes;
};

} // namespace xev
