#pragma once

#include "../VKEngineDevice/VKEngineDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std lib headers
#include <string>
#include <vector>
#include <iostream>
#include <array>



class VKSwapChain {
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    VKSwapChain(VKEngineDevice& engine_device, VkExtent2D window_extent);
    ~VKSwapChain();

    VKSwapChain(const VKSwapChain&) = delete;
    void operator=(const VKSwapChain&) = delete;

    VkFramebuffer GetFrameBuffer(int index) { return m_vkSwapChain_frameBuffers[index]; }
    VkRenderPass GetRenderPass() { return m_vkRenderPass; }
    VkImageView GetImageView(int index) { return m_vkSwapChain_imageViews[index]; }
    size_t GetImageCount() { return m_vkSwapChain_images.size(); }
    VkFormat GetSwapChainImageFormat() { return m_vkSwapChain_imageFormat; }
    VkExtent2D GetSwapChainExtent() { return m_vkSwapChain_extent; }
    uint32_t GetWidth() { return m_vkSwapChain_extent.width; }
    uint32_t GetHeight() { return m_vkSwapChain_extent.height; }

    float ExtentAspectRatio() {return (float)m_vkSwapChain_extent.width / (float)m_vkSwapChain_extent.height;}
    VkFormat FindDepthFormat();

    VkResult AcquireNextImage(uint32_t* imageIndex);
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

private:
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDepthResources();
    void CreateFramebuffers();
    void CreateSyncObjects();

    // Helper functions
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
    VkFormat m_vkSwapChain_imageFormat;
    VkExtent2D m_vkSwapChain_extent;

    std::vector<VkFramebuffer> m_vkSwapChain_frameBuffers;
    VkRenderPass m_vkRenderPass;

    std::vector<VkImage> m_vkDepth_images;
    std::vector<VkDeviceMemory> m_vkDepth_imageMemorys;
    std::vector<VkImageView> m_vkDepth_imageViews;
    std::vector<VkImage> m_vkSwapChain_images;
    std::vector<VkImageView> m_vkSwapChain_imageViews;

    VKEngineDevice& m_engineDevice;
    VkExtent2D m_window_extent;

    VkSwapchainKHR m_vkSwapChain;

    std::vector<VkSemaphore> m_vkImageAvailable_semaphores;
    std::vector<VkSemaphore> m_vkRenderFinished_semaphores;
    std::vector<VkFence> m_vkInFlight_fences;
    std::vector<VkFence> m_vkImagesInFlight_fences;
    size_t m_currentFrame_index = 0;
};