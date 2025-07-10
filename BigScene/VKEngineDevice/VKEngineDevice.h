#pragma once

#include "../MainWindow/MainWindow.h"


///std
#include <string>
#include <vector>



struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {///todo make in std::optional<uint32_t> or mb pohui
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool IsComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class VKEngineDevice {///bad name i rename it into EngineDevice in future
public:
    VKEngineDevice(MainWindow& window);
    ~VKEngineDevice();

    // Not copyable or movable
    VKEngineDevice(const VKEngineDevice&) = delete;
    void operator=(const VKEngineDevice&) = delete;
    VKEngineDevice(VKEngineDevice&&) = delete;
    VKEngineDevice& operator=(VKEngineDevice&&) = delete;

    VkCommandPool GetCommandPool() { return m_vkCommandPool; }
    VkDevice GetDevice() { return m_vkDevice; }
    VkSurfaceKHR GetSurface() { return m_vkSurface; }
    VkQueue GetGraphicsQueue() { return m_vkGraphicsQueue; }
    VkQueue GetPresentQueue() { return m_vkPresentQueue; }

    //SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_vkPhysicalDevice); }
    //uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    //QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_vkPhysicalDevice); }
   // VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Buffer Helper Functions
    //void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
   // VkCommandBuffer BeginSingleTimeCommands();
    //void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
   // void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    //void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

    //void CreateImageWithInfo(const VkImageCreateInfo& imageInfo,VkMemoryPropertyFlags properties,VkImage& image,VkDeviceMemory& imageMemory);

private:
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    //void CreateCommandPool();

    // helper functions
    bool IsDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void HasGLFWRequiredInstanceExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    int GetPhysicalGPUScore(VkPhysicalDevice device);

private:
#ifdef ENABLE_VULKAN_VALIDATION_LAYERS
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif

    VkInstance m_vkInstance;
    VkDebugUtilsMessengerEXT m_vkDebugMessenger;
    VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
    MainWindow& m_mainWindow;
    VkCommandPool m_vkCommandPool;

    VkDevice m_vkDevice;
    VkSurfaceKHR m_vkSurface;
    VkQueue m_vkGraphicsQueue;
    VkQueue m_vkPresentQueue;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};