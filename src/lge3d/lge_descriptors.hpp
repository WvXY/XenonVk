#pragma once

#include "lge_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace lge {

    class LgeDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(LgeDevice &lgeDevice) : lgeDevice{lgeDevice} {}

            Builder &addBinding(
                    uint32_t binding, VkDescriptorType descriptorType,
                    VkShaderStageFlags stageFlags,
                    uint32_t count = 1);

            std::unique_ptr<LgeDescriptorSetLayout> build() const;

        private:
            LgeDevice &lgeDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        LgeDescriptorSetLayout(
                LgeDevice &lgeDevice,
                std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

        ~LgeDescriptorSetLayout();

        LgeDescriptorSetLayout(const LgeDescriptorSetLayout &) = delete;

        LgeDescriptorSetLayout &
        operator=(const LgeDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout
        getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        LgeDevice &lgeDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class LgeDescriptorWriter;
    };

    class LgeDescriptorPool {
    public:
        class Builder {
        public:
            Builder(LgeDevice &lgeDevice) : lgeDevice{lgeDevice} {}

            Builder &
            addPoolSize(VkDescriptorType descriptorType, uint32_t count);

            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);

            Builder &setMaxSets(uint32_t count);

            std::unique_ptr<LgeDescriptorPool> build() const;

        private:
            LgeDevice &lgeDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        LgeDescriptorPool(
                LgeDevice &lgeDevice, uint32_t maxSets,
                VkDescriptorPoolCreateFlags poolFlags,
                const std::vector<VkDescriptorPoolSize> &poolSizes);

        ~LgeDescriptorPool();

        LgeDescriptorPool(const LgeDescriptorPool &) = delete;

        LgeDescriptorPool &operator=(const LgeDescriptorPool &) = delete;

        bool allocateDescriptor(
                const VkDescriptorSetLayout descriptorSetLayout,
                VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        LgeDevice &lgeDevice;
        VkDescriptorPool descriptorPool;

        friend class LgeDescriptorWriter;
    };

    class LgeDescriptorWriter {
    public:
        LgeDescriptorWriter(LgeDescriptorSetLayout &setLayout,
                            LgeDescriptorPool &pool);

        LgeDescriptorWriter &
        writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);

        LgeDescriptorWriter &
        writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);

        void overwrite(VkDescriptorSet &set);

    private:
        LgeDescriptorSetLayout &setLayout;
        LgeDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

} // namespace lge