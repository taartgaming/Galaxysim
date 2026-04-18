#include "VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    : device(device) {

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, memory, 0);
}

VulkanBuffer::~VulkanBuffer() {
}

void VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
    vkMapMemory(device, memory, offset, size, 0, &mapped);
}

void VulkanBuffer::unmap() {
    vkUnmapMemory(device, memory);
    mapped = nullptr;
}
void VulkanBuffer::cleanup() {
    if (mapped) unmap();
    if (memory != VK_NULL_HANDLE) vkFreeMemory(device, memory, nullptr);
    if (buffer != VK_NULL_HANDLE) vkDestroyBuffer(device, buffer, nullptr);
}

void VulkanBuffer::copyTo(void* data, VkDeviceSize size) {
    if (!mapped) throw std::runtime_error("Attempted to copy to unmapped buffer!");
    memcpy(mapped, data, (size_t)size);
}

uint32_t VulkanBuffer::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
    VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}