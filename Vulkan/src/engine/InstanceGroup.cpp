#include "InstanceGroup.h"

InstanceGroup::InstanceGroup(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
    VkQueue graphicsQueue, const std::vector<InstanceData>& instances) {
    count = static_cast<uint32_t>(instances.size());
    VkDeviceSize bufferSize = sizeof(InstanceData) * count;

    // Use our Staging logic (similar to Mesh) to put instance data in Device Local memory
    VulkanBuffer stagingBuffer(device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer.map(bufferSize);
    stagingBuffer.copyTo((void*)instances.data(), bufferSize);
    stagingBuffer.unmap();

    instanceBuffer = std::make_unique<VulkanBuffer>(device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VulkanUtils::copyBuffer(device, commandPool, graphicsQueue,
        stagingBuffer.getBuffer(), instanceBuffer->getBuffer(), bufferSize);
}