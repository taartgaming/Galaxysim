#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>

class VulkanBuffer {
public:
    VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size,
        VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~VulkanBuffer();

    // Prevent accidental copying of GPU resources
    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;

    VkBuffer getBuffer() const { return buffer; }
    VkDeviceMemory getMemory() const { return memory; }

    void map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();
    void copyTo(void* data, VkDeviceSize size);

private:
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
        VkMemoryPropertyFlags properties);

    VkDevice device;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    void* mapped = nullptr;
};