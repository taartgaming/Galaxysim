#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include "VulkanBuffer.h"
#include "VulkanUtils.hpp"

class VulkanTexture {
public:
    VulkanTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool,
        VkQueue graphicsQueue, const std::string& path);
    ~VulkanTexture();

    void destroy();
    VkImageView getImageView() const { return imageView; }
    VkSampler getSampler() const { return sampler; }

private:
    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
        VkImageLayout newLayout, uint32_t mipLevels,
        VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);

    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight,
        uint32_t mipLevels, VkPhysicalDevice physicalDevice, VkDevice device,
        VkCommandPool commandPool, VkQueue graphicsQueue);

    void createSampler(VkPhysicalDevice physicalDevice);

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
    uint32_t mipLevels;
};