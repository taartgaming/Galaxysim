#pragma once
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"

class VulkanDescriptorWriter {
public:
    VulkanDescriptorWriter(VulkanDescriptorSetLayout& layout, VulkanDescriptorPool& pool);

    VulkanDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
    VulkanDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

    bool build(VkDescriptorSet& set);
    void overwrite(VkDescriptorSet& set);

private:
    VulkanDescriptorSetLayout& layout;
    VulkanDescriptorPool& pool;
    std::vector<VkWriteDescriptorSet> writes;
};