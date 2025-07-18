#include "VKSwapChain.h"



VKSwapChain::VKSwapChain(VKEngineDevice& engine_device, VkExtent2D window_extent) : m_engineDevice(engine_device), m_window_extent(window_extent){
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateDepthResources();
	CreateFramebuffers();
	CreateSyncObjects();
}
VKSwapChain::~VKSwapChain() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(m_engineDevice.GetDevice(), m_vkImageAvailable_semaphores[i], nullptr);
        vkDestroySemaphore(m_engineDevice.GetDevice(), m_vkRenderFinished_semaphores[i], nullptr);
        vkDestroyFence(m_engineDevice.GetDevice(), m_vkInFlight_fences[i], nullptr);
    }

    for (auto framebuffer : m_vkSwapChain_frameBuffers) {
        vkDestroyFramebuffer(m_engineDevice.GetDevice(), framebuffer, nullptr);
    }

    for (int i = 0; i < m_vkDepth_images.size(); i++) {
        vkDestroyImageView(m_engineDevice.GetDevice(), m_vkDepth_imageViews[i], nullptr);
        vkDestroyImage(m_engineDevice.GetDevice(), m_vkDepth_images[i], nullptr);
        vkFreeMemory(m_engineDevice.GetDevice(), m_vkDepth_imageMemorys[i], nullptr);
    }

    vkDestroyRenderPass(m_engineDevice.GetDevice(), m_vkRenderPass, nullptr);

    for (auto imageView : m_vkSwapChain_imageViews) {
        vkDestroyImageView(m_engineDevice.GetDevice(), imageView, nullptr);
    }
    m_vkSwapChain_imageViews.clear();

    if (m_vkSwapChain != nullptr) {
        vkDestroySwapchainKHR(m_engineDevice.GetDevice(), m_vkSwapChain, nullptr);
        m_vkSwapChain = nullptr;
    }
}


void VKSwapChain::CreateSwapChain() {
    SwapChainSupportDetails swapChainSupport = m_engineDevice.GetSwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_engineDevice.GetSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = m_engineDevice.FindPhysicalQueueFamilies();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_engineDevice.GetDevice(), &createInfo, nullptr, &m_vkSwapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    // we only specified a minimum number of images in the swap chain, so the implementation is
    // allowed to create a swap chain with more. That's why we'll first query the final number of
    // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
    // retrieve the handles.
    vkGetSwapchainImagesKHR(m_engineDevice.GetDevice(), m_vkSwapChain, &imageCount, nullptr);
    m_vkSwapChain_images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_engineDevice.GetDevice(), m_vkSwapChain, &imageCount, m_vkSwapChain_images.data());

    m_vkSwapChain_imageFormat = surfaceFormat.format;
    m_vkSwapChain_extent = extent;
}
void VKSwapChain::CreateImageViews() {
    m_vkSwapChain_imageViews.resize(m_vkSwapChain_images.size());
    for (size_t i = 0; i < m_vkSwapChain_images.size(); i++) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_vkSwapChain_images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_vkSwapChain_imageFormat;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_engineDevice.GetDevice(), &viewInfo, nullptr, &m_vkSwapChain_imageViews[i]) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create texture image view!");
        }
    }
}
void VKSwapChain::CreateRenderPass() {
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_vkSwapChain_imageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstSubpass = 0;
    dependency.dstStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(m_engineDevice.GetDevice(), &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create render pass!");
    }
}
void VKSwapChain::CreateDepthResources() {
    VkFormat depthFormat = FindDepthFormat();
    VkExtent2D swapChainExtent = m_vkSwapChain_extent;

    m_vkDepth_images.resize(GetImageCount());
    m_vkDepth_imageMemorys.resize(GetImageCount());
    m_vkDepth_imageViews.resize(GetImageCount());

    for (int i = 0; i < m_vkDepth_images.size(); i++) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = swapChainExtent.width;
        imageInfo.extent.height = swapChainExtent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = depthFormat;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.flags = 0;

        m_engineDevice.CreateImageWithInfo(
            imageInfo,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_vkDepth_images[i],
            m_vkDepth_imageMemorys[i]);

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_vkDepth_images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = depthFormat;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_engineDevice.GetDevice(), &viewInfo, nullptr, &m_vkDepth_imageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create texture image view!");
        }
    }
}
void VKSwapChain::CreateFramebuffers() {
    m_vkSwapChain_frameBuffers.resize(GetImageCount());
    for (size_t i = 0; i < GetImageCount(); i++) {
        std::array<VkImageView, 2> attachments = { m_vkSwapChain_imageViews[i], m_vkDepth_imageViews[i] };

        VkExtent2D swapChainExtent = m_vkSwapChain_extent;
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_engineDevice.GetDevice(), &framebufferInfo, nullptr, &m_vkSwapChain_frameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}
void VKSwapChain::CreateSyncObjects() {
    m_vkImageAvailable_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_vkRenderFinished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_vkInFlight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    m_vkImagesInFlight_fences.resize(GetImageCount(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(m_engineDevice.GetDevice(), &semaphoreInfo, nullptr, &m_vkImageAvailable_semaphores[i]) !=
            VK_SUCCESS ||
            vkCreateSemaphore(m_engineDevice.GetDevice(), &semaphoreInfo, nullptr, &m_vkRenderFinished_semaphores[i]) !=
            VK_SUCCESS ||
            vkCreateFence(m_engineDevice.GetDevice(), &fenceInfo, nullptr, &m_vkInFlight_fences[i]) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}


VkSurfaceFormatKHR VKSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}
VkPresentModeKHR VKSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            std::cout << "Present mode: Mailbox" << std::endl;
            return availablePresentMode;
        }
    }

    // for (const auto &availablePresentMode : availablePresentModes) {
    //   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
    //     std::cout << "Present mode: Immediate" << std::endl;
    //     return availablePresentMode;
    //   }
    // }

    std::cout << "Present mode: V-Sync" << std::endl;
    return VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D VKSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = m_window_extent;
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}


VkFormat VKSwapChain::FindDepthFormat() {
    return m_engineDevice.FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
