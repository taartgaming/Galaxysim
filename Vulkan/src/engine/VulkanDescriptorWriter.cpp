#include "VulkanDescriptorWriter.h"

VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& layout, VulkanDescriptorPool& pool)
    : layout{ layout }, pool{ pool } {
}

VulkanDescriptorWriter& VulkanDescriptorWriter::writeBuffer(
    uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {

    auto& bindingDescription = layout.bindings[binding];

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

VulkanDescriptorWriter& VulkanDescriptorWriter::writeImage(
    uint32_t binding, VkDescriptorImageInfo* imageInfo) {

    auto& bindingDescription = layout.bindings[binding];

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

bool VulkanDescriptorWriter::build(VkDescriptorSet& set) {
    // 1. Allocate the set from the pool
    bool success = pool.allocateDescriptor(layout.getDescriptorSetLayout(), set);
    if (!success) return false;

    // 2. Point the writes to the newly allocated set
    overwrite(set);
    return true;
}

void VulkanDescriptorWriter::overwrite(VkDescriptorSet& set) {
    for (auto& write : writes) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(pool.device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
}
