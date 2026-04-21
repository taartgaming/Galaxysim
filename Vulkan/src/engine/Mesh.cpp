#include "Mesh.h"
#include "VulkanUtils.hpp"

Mesh::Mesh(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
    VkQueue graphicsQueue, const std::vector<Vertex>& vertices,
    const std::vector<uint32_t>& indices) : device(device) {

    indexCount = static_cast<uint32_t>(indices.size());
    VkDeviceSize vBufferSize = sizeof(Vertex) * vertices.size();
    VkDeviceSize iBufferSize = sizeof(uint32_t) * indices.size();

    // Create Vertex Buffer with staging
    createBufferWithStaging(device, physicalDevice, commandPool, graphicsQueue,
        (void*)vertices.data(), vBufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBuffer);

    // Create Index Buffer with staging
    createBufferWithStaging(device, physicalDevice, commandPool, graphicsQueue,
        (void*)indices.data(), iBufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBuffer);
}

void Mesh::draw(VkCommandBuffer commandBuffer, VkBuffer instanceBuffer, uint32_t instanceCount) {
    VkBuffer vertBuffers[] = { vertexBuffer->getBuffer() };
    VkDeviceSize vertOffsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertBuffers, vertOffsets);

    if (instanceBuffer != VK_NULL_HANDLE) {
        VkBuffer instBuffers[] = { instanceBuffer };
        VkDeviceSize instOffsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 1, 1, instBuffers, instOffsets);
    }

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, 0, 0, 0);
}

void Mesh::destroy() {
    if (vertexBuffer) vertexBuffer->cleanup();
    if (indexBuffer) indexBuffer->cleanup();
}

// Inside Mesh.cpp

void Mesh::createBufferWithStaging(VkDevice device, VkPhysicalDevice physicalDevice,
    VkCommandPool commandPool, VkQueue graphicsQueue,
    void* data, VkDeviceSize size,
    VkBufferUsageFlags usage, std::unique_ptr<VulkanBuffer>& targetBuffer) {

    // 1. Create a staging buffer (Host Visible)
    VulkanBuffer stagingBuffer(device, physicalDevice, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // 2. Map and copy data to staging
    stagingBuffer.map(size);
    stagingBuffer.copyTo(data, size);
    stagingBuffer.unmap();

    // 3. Create the actual target buffer (Device Local)
    targetBuffer = std::make_unique<VulkanBuffer>(device, physicalDevice, size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VulkanUtils::copyBuffer(device, commandPool, graphicsQueue,
        stagingBuffer.getBuffer(), targetBuffer->getBuffer(), size);
    stagingBuffer.cleanup();

}
