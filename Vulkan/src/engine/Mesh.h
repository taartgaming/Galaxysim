#pragma once
#include "VulkanBuffer.h"
#include <vector>
#include <memory>

// Assuming Vertex struct is defined or included
class Mesh {
public:
    Mesh(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
        VkQueue graphicsQueue, const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices);

    ~Mesh() = default;

    void draw(VkCommandBuffer commandBuffer, VkBuffer instanceBuffer, uint32_t instanceCount);
    void destroy();

private:
    VkDevice device;
    std::unique_ptr<VulkanBuffer> vertexBuffer;
    std::unique_ptr<VulkanBuffer> indexBuffer;
    uint32_t indexCount;

    // Helper for the staging process
    void createBufferWithStaging(VkDevice device, VkPhysicalDevice physicalDevice,
        VkCommandPool commandPool, VkQueue graphicsQueue,
        void* data, VkDeviceSize size,
        VkBufferUsageFlags usage, std::unique_ptr<VulkanBuffer>& targetBuffer);
};