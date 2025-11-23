/*
 * =============================================================================
 *
 * Copyright (c) 2025 Cooper Herlihy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * =============================================================================
 */

#ifndef HARMONY_GRAPHICS_H
#define HARMONY_GRAPHICS_H

#include "harmony.h"

#include <vulkan/vulkan.h>

typedef struct HarmonyPlatform HarmonyPlatform;

/**
 * Creates platform specific internal resources for audio, window, etc.
 *
 * Parameters
 * - allocator Where to allocate memory from
 * Returns
 * - An opaque pointer to the platform's resources
 */
HarmonyPlatform *harmony_platform_init(const HarmonyAllocator *allocator);

/**
 * Destroys the platform specific internal resources
 *
 * Parameters
 * - allocator Where to free memory from
 * - platform The platform resources
 */
void harmony_platform_shutdown(const HarmonyAllocator *allocator, HarmonyPlatform *platform);

/**
 * A key on the keyboard or button on the mouse
 */
typedef enum HarmonyKey {
    HARMONY_KEY_NONE = 0,
    HARMONY_KEY_ESCAPE,
    HARMONY_KEY_SPACE,
    HARMONY_KEY_ENTER,
    HARMONY_KEY_TAB,
    HARMONY_KEY_DELETE,
    HARMONY_KEY_BACKSPACE,
    HARMONY_KEY_LMOUSE,
    HARMONY_KEY_RMOUSE,
    HARMONY_KEY_MMOUSE,
    HARMONY_KEY_UP,
    HARMONY_KEY_DOWN,
    HARMONY_KEY_LEFT,
    HARMONY_KEY_RIGHT,
    HARMONY_KEY_Q,
    HARMONY_KEY_W,
    HARMONY_KEY_E,
    HARMONY_KEY_R,
    HARMONY_KEY_T,
    HARMONY_KEY_Y,
    HARMONY_KEY_U,
    HARMONY_KEY_I,
    HARMONY_KEY_O,
    HARMONY_KEY_P,
    HARMONY_KEY_A,
    HARMONY_KEY_S,
    HARMONY_KEY_D,
    HARMONY_KEY_F,
    HARMONY_KEY_G,
    HARMONY_KEY_H,
    HARMONY_KEY_J,
    HARMONY_KEY_K,
    HARMONY_KEY_L,
    HARMONY_KEY_Z,
    HARMONY_KEY_X,
    HARMONY_KEY_C,
    HARMONY_KEY_V,
    HARMONY_KEY_B,
    HARMONY_KEY_N,
    HARMONY_KEY_M,
    HARMONY_KEY_LSHIFT,
    HARMONY_KEY_RSHIFT,
    HARMONY_KEY_LCTRL,
    HARMONY_KEY_RCTRL,
    HARMONY_KEY_LALT,
    HARMONY_KEY_RALT,
    HARMONY_KEY_COUNT,
} HarmonyKey;

typedef struct HarmonyWindowInternals HarmonyWindowInternals;

/**
 * A window
 *
 * Note, the values stored in this struct are not to be changed manually, they
 * are updated by processing events, and can be read by the application
 */
typedef struct HarmonyWindow {
    /**
     * Platform dependent resources internal to the window
     */
    HarmonyWindowInternals *platform_internals;
    /**
     * The current width of the window
     */
    u32 width;
    /**
     * The current height width of the window
     */
    u32 height;
    /**
     * The current mouse x position
     */
    f32 mouse_pos_x;
    /**
     * The current mouse y position
     */
    f32 mouse_pos_y;
    /**
     * The change in mouse x position
     */
    f32 mouse_delta_x;
    /**
     * The change in mouse y position
     */
    f32 mouse_delta_y;
    /**
     * Whether the window was resized
     */
    bool was_resized;
    /**
     * Whether the window has been closed by the close button or window manager
     */
    bool was_closed;
    /**
     * Which keys are being held down
     */
    bool keys_down[HARMONY_KEY_COUNT];
    /**
     * Which keys were pressed
     */
    bool keys_pressed[HARMONY_KEY_COUNT];
    /**
     * Which keys were released
     */
    bool keys_released[HARMONY_KEY_COUNT];
} HarmonyWindow;

/**
 * Configuration for a window
 */
typedef struct HarmonyWindowConfig {
    /**
     * The title of the window, must not be NULL
     */
    const char* title;
    /**
     * Whether the window should be windowed or fullscreen
     */
    bool windowed;
    /**
     * The width in pixels if windowed, otherwise ignored
     */
    u32 width;
    /**
     * The height in pixels if windowed, otherwise ignored
     */
    u32 height;
} HarmonyWindowConfig;

/**
 * Creates a window
 *
 * Parameters
 * - allocator Where to get memory from, must not be NULL
 * - platform The platform resources, must not be NULL
 * - config The window configuration, must not be NULL
 * Returns
 * - The created window
 */
HarmonyWindow harmony_window_create(
    const HarmonyAllocator *allocator,
    const HarmonyPlatform *platform,
    const HarmonyWindowConfig *config);

/**
 * Destroys a window
 *
 * Parameters
 * - allocator Where to free memory from, must not be nULL
 * - platform The platform resources, must not be NULL
 * - window The window, must not be NULL
 */
void harmony_window_destroy(const HarmonyAllocator *allocator, const HarmonyPlatform *platform, HarmonyWindow *window);

/**
 * Processes all events since the last call to process events or startup
 *
 * Must be called every frame before querying input
 * Processes all events, so all windows must be given
 * Updates the each window's input state
 *
 * Parameters
 * - harmony The Harmony context, must not be NULL
 * - windows All open windows
 * - window_count The number of windows
 */
void harmony_window_process_events(const HarmonyPlatform *platform, HarmonyWindow **windows, u32 window_count);

/**
 * Gets the size of the window
 *
 * Parameters
 * - window The window, must not be NULL
 * - width A pointer to store the width, must not be NULL
 * - height A pointer to store the height, must not be NULL
 */
inline void harmony_window_get_size(const HarmonyWindow *window, u32 *width, u32 *height) {
    harmony_assert(window != NULL);
    harmony_assert(width != NULL);
    harmony_assert(height != NULL);
    *width = window->width;
    *height = window->height;
}

/**
 * Checks if the window was closed via close button or window manager
 *
 * harmony_window_close() is not automatically called when this function returns
 * true, and may be called manually
 *
 * Parameters
 * - window The window, must not be NULL
 * Returns
 * - whether the window was closed
 */
inline bool harmony_window_was_closed(const HarmonyWindow *window) {
    harmony_assert(window != NULL);
    return window->was_closed;
}

/**
 * Checks if the window was resized
 *
 * Parameters
 * - window The window, must not be NULL
 * Returns
 * - whether the window was resized
 */
inline bool harmony_window_was_resized(const HarmonyWindow *window) {
    harmony_assert(window != NULL);
    return window->was_resized;
}

/**
 * Gets the most recent mouse position
 *
 * Parameters
 * - window The window, must not be NULL
 * - x A pointer to store the x position, must not be NULL
 * - y A pointer to store the y position, must not be NULL
 */
inline void harmony_window_get_mouse_pos(const HarmonyWindow *window, f32 *x, f32 *y) {
    harmony_assert(window != NULL);
    harmony_assert(x != NULL);
    harmony_assert(y != NULL);
    *x = window->mouse_pos_x;
    *y = window->mouse_pos_y;
}

/**
 * Gets the most recent mouse delta
 *
 * Parameters
 * - window The window, must not be NULL
 * - x A pointer to store the x delta, must not be NULL
 * - y A pointer to store the y delta, must not be NULL
 */
inline void harmony_window_get_mouse_delta(const HarmonyWindow *window, f32 *x, f32 *y) {
    harmony_assert(window != NULL);
    harmony_assert(x != NULL);
    harmony_assert(y != NULL);
    *x = window->mouse_delta_x;
    *y = window->mouse_delta_y;
}

/**
 * Checks if a key is being held down
 *
 * Parameters
 * - window The window, must not be NULL
 * - key The key to check
 * Returns
 * - whether the key is being held down
 */
inline bool harmony_window_is_key_down(const HarmonyWindow *window, HarmonyKey key) {
    harmony_assert(window != NULL);
    harmony_assert((u32)key < harmony_countof(window->keys_down));
    return window->keys_down[key];
}

/**
 * Checks if a key was pressed this frame
 *
 * Parameters
 * - window The window, must not be NULL
 * - key The key to check
 * Returns
 * - whether the key was pressed this frame
 */
inline bool harmony_window_was_key_pressed(const HarmonyWindow *window, HarmonyKey key) {
    harmony_assert(window != NULL);
    harmony_assert((u32)key < harmony_countof(window->keys_pressed));
    return window->keys_pressed[key];
}

/**
 * Checks if a key was released this frame
 *
 * Parameters
 * - window The window, must not be NULL
 * - key The key to check
 * Returns
 * - whether the key was released this frame
 */
inline bool harmony_window_was_key_released(const HarmonyWindow *window, HarmonyKey key) {
    harmony_assert(window != NULL);
    harmony_assert((u32)key < harmony_countof(window->keys_released));
    return window->keys_released[key];
}

#define HARMONY_MAKE_VULKAN_FUNC(name) PFN_##name name;

/**
 * The vulkan function pointers that can be used
 */
typedef struct HarmonyVulkanFuncs {
    HARMONY_MAKE_VULKAN_FUNC(vkGetInstanceProcAddr)
    HARMONY_MAKE_VULKAN_FUNC(vkGetDeviceProcAddr)

    HARMONY_MAKE_VULKAN_FUNC(vkEnumerateInstanceExtensionProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkEnumerateInstanceLayerProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateInstance)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyInstance)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateDebugUtilsMessengerEXT)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyDebugUtilsMessengerEXT)

    HARMONY_MAKE_VULKAN_FUNC(vkEnumeratePhysicalDevices)
    HARMONY_MAKE_VULKAN_FUNC(vkEnumerateDeviceExtensionProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceQueueFamilyProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceFeatures)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateDevice)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyDevice)
    HARMONY_MAKE_VULKAN_FUNC(vkDeviceWaitIdle)

    HARMONY_MAKE_VULKAN_FUNC(vkDestroySurfaceKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceSurfaceFormatsKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceSurfacePresentModesKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateSwapchainKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroySwapchainKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkGetSwapchainImagesKHR)
    HARMONY_MAKE_VULKAN_FUNC(vkAcquireNextImageKHR)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateSemaphore)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroySemaphore)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateFence)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyFence)
    HARMONY_MAKE_VULKAN_FUNC(vkResetFences)
    HARMONY_MAKE_VULKAN_FUNC(vkWaitForFences)

    HARMONY_MAKE_VULKAN_FUNC(vkGetDeviceQueue)
    HARMONY_MAKE_VULKAN_FUNC(vkQueueWaitIdle)
    HARMONY_MAKE_VULKAN_FUNC(vkQueueSubmit2)
    HARMONY_MAKE_VULKAN_FUNC(vkQueuePresentKHR)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateCommandPool)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyCommandPool)
    HARMONY_MAKE_VULKAN_FUNC(vkAllocateCommandBuffers)
    HARMONY_MAKE_VULKAN_FUNC(vkFreeCommandBuffers)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateDescriptorPool)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyDescriptorPool)
    HARMONY_MAKE_VULKAN_FUNC(vkResetDescriptorPool)
    HARMONY_MAKE_VULKAN_FUNC(vkAllocateDescriptorSets)
    HARMONY_MAKE_VULKAN_FUNC(vkUpdateDescriptorSets)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateDescriptorSetLayout)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyDescriptorSetLayout)
    HARMONY_MAKE_VULKAN_FUNC(vkCreatePipelineLayout)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyPipelineLayout)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateShaderModule)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyShaderModule)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateGraphicsPipelines)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateComputePipelines)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyPipeline)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateImage)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyImage)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateImageView)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyImageView)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateSampler)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroySampler)

    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceMemoryProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkGetBufferMemoryRequirements)
    HARMONY_MAKE_VULKAN_FUNC(vkGetImageMemoryRequirements)

    HARMONY_MAKE_VULKAN_FUNC(vkAllocateMemory)
    HARMONY_MAKE_VULKAN_FUNC(vkFreeMemory)
    HARMONY_MAKE_VULKAN_FUNC(vkBindBufferMemory)
    HARMONY_MAKE_VULKAN_FUNC(vkBindImageMemory)
    HARMONY_MAKE_VULKAN_FUNC(vkMapMemory)
    HARMONY_MAKE_VULKAN_FUNC(vkUnmapMemory)
    HARMONY_MAKE_VULKAN_FUNC(vkFlushMappedMemoryRanges)
    HARMONY_MAKE_VULKAN_FUNC(vkInvalidateMappedMemoryRanges)

    HARMONY_MAKE_VULKAN_FUNC(vkBeginCommandBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkEndCommandBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkResetCommandBuffer)

    HARMONY_MAKE_VULKAN_FUNC(vkCmdCopyBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdCopyImage)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdBlitImage)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdCopyBufferToImage)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdCopyImageToBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdPipelineBarrier2)

    HARMONY_MAKE_VULKAN_FUNC(vkCmdBeginRendering)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdEndRendering)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdSetViewport)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdSetScissor)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdBindPipeline)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdBindDescriptorSets)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdPushConstants)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdBindVertexBuffers)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdBindIndexBuffer)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdDraw)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdDrawIndexed)
    HARMONY_MAKE_VULKAN_FUNC(vkCmdDispatch)
} HarmonyVulkanFuncs;

#undef HARMONY_MAKE_VULKAN_FUNC

/**
 * Basic resources for use in Vulkan code
 */
typedef struct HarmonyVulkan {
    VkInstance instance;
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT debug_messenger;
#endif // NDEBUG
    VkPhysicalDevice gpu;
    VkDevice device;
    u32 queue_family;

    void *lib;
    HarmonyVulkanFuncs pfn;
} HarmonyVulkan;

/**
 * Creates basic resources needed for Vulkan
 *
 * Debug messenger is created in debug mode only
 * Device has synchronization 2 and dynamic rendering features enabled
 *
 * Returns
 * - Created Vulkan resources
 */
HarmonyVulkan harmony_vulkan_create(void);

/**
 * Destroys basic resources for Vulkan
 *
 * All resources created with device or instance must be destroyed first
 *
 * Parameters
 * - vk The Harmony Vulkan context to destroy, must not be NULL
 */
void harmony_vulkan_destroy(HarmonyVulkan *vk);

/**
 * Creates a Vulkan instance with sensible defaults
 * 
 * In debug mode, enables debug messaging
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * Returns
 * - The created VkInstance, should never be NULL
 */
VkInstance harmony_vk_create_instance(const HarmonyVulkan *vk);

/**
 * Destroys a Vulkan instance
 * 
 * Parameters
 * - instance The VkInstance to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_instance(const HarmonyVulkan *vk, VkInstance instance);

/**
 * Creates a Vulkan debug messenger
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * Returns
 * - The created debug messenger, should never be NULL
 */
VkDebugUtilsMessengerEXT harmony_vk_create_debug_messenger(const HarmonyVulkan *vk);

/**
 * Destroys a Vulkan debug messenger
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - messenger The debug messenger to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_debug_messenger(const HarmonyVulkan *vk, VkDebugUtilsMessengerEXT messenger);

/**
 * Finds a suitable Vulkan physical device
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * Returns
 * - The found gpu, should never be VK_NULL_HANDLE
 */
VkPhysicalDevice harmony_vk_find_physical_device(const HarmonyVulkan *vk);

/**
 * Creates a Vulkan logical device with sensible defaults
 *
 * Enables sync 2 and dynamic rendering extensions
 * Creates 1 queue that supports graphics and compute
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * Returns
 * - The created Vulkan device, should never be VK_NULL_HANDLE
 */
VkDevice harmony_vk_create_device(const HarmonyVulkan *vk);

/**
 * Destroy a Vulkan logical device
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - device The Vulkan device to destroy
 */
void harmony_vk_destroy_device(const HarmonyVulkan *vk, VkDevice device);

/**
 * Waits for the device to idle
 *
 * Paramaters
 * - vk The Harmony Vulkan context, must not be NULL
 */
void harmony_vk_device_wait(const HarmonyVulkan *vk);

/**
 * Create a Vulkan surface for the window, according to the platform
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - platform The Harmony platform context, must not be NULL
 * - window The window to create a surface for, must not be NULL
 * Returns
 * - The created Vulkan surface
 */
VkSurfaceKHR harmony_vk_create_surface(
    const HarmonyVulkan *vk,
    const HarmonyPlatform *platform,
    const HarmonyWindow *window);

/**
 * Destroys a Vulkan surface
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - surface The surface to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_surface(const HarmonyVulkan *vk, VkSurfaceKHR surface);

/**
 * Creates a Vulkan swapchain
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - width A pointer to store the width of the swapchain, must not be NULL
 * - height A pointer to store the height of the swapchain, must not be NULL
 * - format A pointer to store the format of the swapchain, must not be NULL
 * - old_swapchain The old swapchain, may be VK_NULL_HANDLE
 * - surface The surface to create from
 * - image_usage How the swapchain's images will be used
 * - desired_mode The preferred present mode (fallback to FIFO if unavailable)
 * Returns
 * - The created Vulkan swapchain
 */
VkSwapchainKHR harmony_vk_create_swapchain(
    const HarmonyVulkan *vk,
    u32 *width,
    u32 *height,
    VkFormat *format,
    VkSwapchainKHR old_swapchain,
    VkSurfaceKHR surface,
    VkImageUsageFlags image_usage,
    VkPresentModeKHR desired_mode);

/**
 * Destroys a Vulkan swapchain
 *
 * Parameters
 * - vk The harmony Vulkan context, must not be NULL
 * - swapchain The swapchain to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_swapchain(const HarmonyVulkan *vk, VkSwapchainKHR swapchain);

/**
 * Query the number of images in a swapchain
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - swapchain The swapchain to query, must not be VK_NULL_HANDLE
 * Returns
 * - The number of images in the swapchain
 */
u32 harmony_vk_get_swapchain_image_count(const HarmonyVulkan *vk, VkSwapchainKHR swapchain);

/**
 * Gets the images from a swapchain
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - swapchain The swapchain to get the images from, must not be VK_NULL_HANDLE
 * - images A buffer to store the images in, must not be NULL
 * - count The number of images, must be equal to the number the swapchain has
 */
void harmony_vk_get_swapchain_images(const HarmonyVulkan *vk, VkSwapchainKHR swapchain, VkImage *images, u32 count);

/**
 * Gets the index of the next image in the Swapchain, signaling a semaphore
 * and/or fence when it becomes available
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - swapchain The swapchain to get from, must not be VK_NULL_HANDLE
 * - signal_semaphore The semaphore to signal, may be VK_NULL_HANDLE
 * - signal_fence The fence to signal, may be VK_NULL_HANDLE
 * Returns
 * - The index of the next image
 */
u32 harmony_vk_acquire_next_image(
    const HarmonyVulkan *vk,
    VkSwapchainKHR swapchain,
    VkSemaphore signal_semaphore,
    VkFence signal_fence);

/**
 * Presents the swapchain to the display
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - queue The queue to use to present, must not be VK_NULL_HANDLE
 * - swapchain The swapchain to present, must not be VK_NULL_HANDLE
 * - image_index The image in the swapchain
 * - wait_semaphores The semaphores to wait for, may be NULL
 * - semaphore_count The number of semaphores in wait_semaphores, may be 0
 */
void harmony_vk_present(
    const HarmonyVulkan *vk,
    VkQueue queue,
    VkSwapchainKHR swapchain,
    u32 image_index,
    VkSemaphore *wait_semaphores,
    u32 semaphore_count);

/**
 * Creates a Vulkan semaphore
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - flags The optional flags to use
 * Returns
 * - The created Vulkan semaphore
 */
VkSemaphore harmony_vk_create_semaphore(const HarmonyVulkan *vk, VkSemaphoreCreateFlags flags);

/**
 * Destroys a Vulkan semaphore
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - semaphore The Vulkan semaphore to destroy
 */
void harmony_vk_destroy_semaphore(const HarmonyVulkan *vk, VkSemaphore semaphore);

/**
 * Creates a Vulkan fence
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - flags The optional flags to use
 * Returns
 * - The created Vulkan fence
 */
VkFence harmony_vk_create_fence(const HarmonyVulkan *vk, VkFenceCreateFlags flags);

/**
 * Destroys a Vulkan fence
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - semaphore The Vulkan fence to destroy
 */
void harmony_vk_destroy_fence(const HarmonyVulkan *vk, VkFence fence);

/**
 * Waits for Vulkan fences to be signaled
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - fences The fences to wait for, must not be NULL
 * - count The number of fences, must be greater than 0
 */
void harmony_vk_wait_for_fences(const HarmonyVulkan *vk, VkFence *fences, u32 count);

/**
 * Resets (unsignals) fences
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - fences The fences to reset, must not be NULL
 * - count The number of fences, must be greater than 0
 */
void harmony_vk_reset_fences(const HarmonyVulkan *vk, VkFence *fences, u32 count);

/**
 * Find the first queue family index which supports the the queue flags
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - queue_family A pointer to store the found queue family, must not be NULL
 * - queue_flags The flags required of the queue family
 * Returns
 * - true if a family was found
 * - false if none could be found
 */
bool harmony_vk_find_queue_family(const HarmonyVulkan *vk, u32 *queue_family, VkQueueFlags queue_flags);

/**
 * Gets the created queue from the device
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * Returns
 * - The queue, should never be VK_NULL_HANDLE
 */
VkQueue harmony_vk_get_queue(const HarmonyVulkan *vk);

/**
 * Wait for a queue to finish commands
 *
 * Can be used to ensure resources are free before destruction
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - queue The Vulkan queue to wait for
 */
void harmony_vk_queue_wait(const HarmonyVulkan *vk, VkQueue queue);

/**
 * Creates a Vulkan command pool
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - flags The optional flags to use
 * Returns
 * - The created Vulkan command pool, should never be VK_NULL_HANDLE
 */
VkCommandPool harmony_vk_create_command_pool(const HarmonyVulkan *vk, VkCommandPoolCreateFlags flags);

/**
 * Destroys a Vulkan command pool
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be VK_NULL_HANDLE
 * - pool The Vulkan command pool to destroy
 */
void harmony_vk_destroy_command_pool(const HarmonyVulkan *vk, VkCommandPool pool);

/**
 * Creates Vulkan commands buffers, filling a buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - pool The command pool to allocate from, must not be VK_NULL_HANDLE
 * - cmds A pointer to store the created command buffers, must not be NULL
 * - count The size of cmds and the number of command buffers to create
 */
void harmony_vk_allocate_command_buffers(
    const HarmonyVulkan *vk,
    VkCommandPool pool,
    VkCommandBuffer *cmds,
    u32 count);

/**
 * Frees Vulkan commands buffers
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - pool The command pool to free to, must not be VK_NULL_HANDLE
 * - cmds The command buffers to free, must not be NULL
 * - count The size of cmds and the number of command buffers to free
 */
void harmony_vk_free_command_buffers(
    const HarmonyVulkan *vk,
    VkCommandPool pool,
    VkCommandBuffer *cmds,
    u32 count);

/**
 * Creates a Vulkan descriptor pool
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - max_sets The maximum number of sets which can be allocated
 * - sizes The numbers of each descriptor type to allocate, must not be NULL
 * - count The count of sizes, must be greater than 0
 * Returns
 * - The created Vulkan descriptor pool, should never be VK_NULL_HANDLE
 */
VkDescriptorPool harmony_vk_create_descriptor_pool(
    const HarmonyVulkan *vk,
    u32 max_sets,
    VkDescriptorPoolSize *sizes,
    u32 count);

/**
 * Destroys a Vulkan descriptor pool
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - pool The Vulkan descriptor pool to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_descriptor_pool(const HarmonyVulkan *vk, VkDescriptorPool pool);

/**
 * Resets a Vulkan descriptor pool, freeing all sets
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - pool The descriptor pool to reset, must not be VK_NULL_HANDLE
 */
void harmony_vk_reset_descriptor_pool(const HarmonyVulkan *vk, VkDescriptorPool pool);

/**
 * Allocates Vulkan descriptor sets from a descriptor pool
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - pool The descriptor pool to allocate from, must not be VK_NULL_HANDLE
 * - layouts The layouts of the sets to allocate, must not be NULL
 * - sets A pointer to store the allocated sets, must not be NULL
 * - count The number of sets to allocate, must be greater than 0
 */
bool harmony_vk_allocate_descriptor_sets(
    const HarmonyVulkan *vk,
    VkDescriptorPool pool,
    VkDescriptorSetLayout *layouts,
    VkDescriptorSet *sets,
    u32 count);

/**
 * Creates a Vulkan descriptor set layout
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - bindings Descriptions of each binding in the layout, must not be NULL
 * - count The number of bindings, must be greater than 0
 */
VkDescriptorSetLayout harmony_vk_create_descriptor_set_layout(
    const HarmonyVulkan *vk,
    const VkDescriptorSetLayoutBinding *bindings,
    u32 count);

/**
 * Destroys a Vulkan descriptor set layout
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - layout The layout to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_descriptor_set_layout(const HarmonyVulkan *vk, VkDescriptorSetLayout layout);

/**
 * Creates a Vulkan pipeline layout
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - layouts The descriptor set layouts to include, may be NULL if none
 * - layout_count The number of descriptor set layouts, may be 0
 * - push_constants The push constant ranges to include, may be NULL
 * - push_constant_count The number of push constant ranges, may be 0
 * Returns
 * - The created Vulkan pipeline layout, should never be VK_NULL_HANDLE
 */
VkPipelineLayout harmony_vk_create_pipeline_layout(
    const HarmonyVulkan *vk,
    const VkDescriptorSetLayout* layouts,
    u32 layout_count,
    const VkPushConstantRange* push_constants,
    u32 push_constant_count
);

/**
 * Destroys a Vulkan pipeline layout
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - layout The layout to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_pipeline_layout(const HarmonyVulkan *vk, VkPipelineLayout layout);

/**
 * Creates a Vulkan shader module
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - code The SPIR-V bytecode of the shader, must not be NULL
 * - size The size of the bytecode in bytes, must be greater than 0
 * Returns
 * - The created Vulkan shader module, should never be VK_NULL_HANDLE
 */
VkShaderModule harmony_vk_create_shader_module(const HarmonyVulkan *vk, const u8 *code, usize size);

/**
 * Destroys a Vulkan shader module
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - shader_module The shader module to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_shader_module(const HarmonyVulkan *vk, VkShaderModule shader_module);

/**
 * Configuration for Vulkan pipelines
 */
typedef struct HarmonyVkPipelineConfig {
    /**
     * The pipeline layout
     */
    VkPipelineLayout layout;
    /**
     * The shaders
     */
    VkShaderModule *shaders;
    /**
     * The stage of each shader, in the same order
     */
    VkShaderStageFlagBits *shader_stages;
    /**
     * The number of shaders and their stages
     */
    u32 shader_count;
    /**
     * Descriptions of the vertex bindings, may be NULL
     */
    VkVertexInputBindingDescription* vertex_bindings;
    /**
     * The number of vertex bindings, may be 0
     */
    u32 vertex_binding_count;
    /**
     * Descriptions of the vertex attributes, may be NULL
     */
    VkVertexInputAttributeDescription* vertex_attributes;
    /**
     * The number of vertex attributes, may be 0
     */
    u32 vertex_attribute_count;
    /**
     * How to interpret vertices into topology, defaults to POINT_LIST
     */
    VkPrimitiveTopology topology;
    /**
     * Enables back/front face culling, defaults to NONE
     */
    VkCullModeFlagBits cull_mode;
    /**
     * The format of the color attachments, none can be UNDEFINED
     */
    VkFormat *color_attachment_formats;
    /**
     * The number of color attachments to use
     */
    u32 color_attachment_count;
    /**
     * The format of the depth attachment, if UNDEFINED no depth attachment
     */
    VkFormat depth_attachment_format;
    /**
     * The format of the stencil attachment, if UNDEFINED no stencil attachment
     */
    VkFormat stencil_attachment_format;
    /**
     * Enables color blending using pixel alpha values
     */
    bool enable_color_blend;
} HarmonyVkPipelineConfig;

/**
 * Creates a graphics pipeline
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - config The pipeline configuration, must not be NULL
 * Returns
 * - The created graphics pipeline, should never be VK_NULL_HANDLE
 */
VkPipeline harmony_vk_create_graphics_pipeline(const HarmonyVulkan *vk, const HarmonyVkPipelineConfig *config);

/**
 * Creates a compute pipeline
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - config The pipeline configuration, must not be NULL
 * Returns
 * - The created compute pipeline, should never be VK_NULL_HANDLE
 */
VkPipeline harmony_vk_create_compute_pipeline(const HarmonyVulkan *vk, const HarmonyVkPipelineConfig *config);

/**
 * Destroys a Vulkan pipeline
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - pipeline The pipeline to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_pipeline(const HarmonyVulkan *vk, VkPipeline pipeline);

/**
 * Creates a Vulkan buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - size The size of the buffer in bytes, must be greater than 0
 * - usage The usage flags for the buffer
 * Returns
 * - The created Vulkan buffer, should never be VK_NULL_HANDLE
 */
VkBuffer harmony_vk_create_buffer(const HarmonyVulkan *vk, usize size, VkBufferUsageFlags usage);

/**
 * Destroys a Vulkan buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - buffer The buffer to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_buffer(const HarmonyVulkan *vk, VkBuffer buffer);

/**
 * Configuration for a Vulkan image
 */
typedef struct HarmonyVkImageConfig {
    /**
     * The format of each pixel, must not be VK_FORMAT_UNDEFINED
     */
    VkFormat format;
    /**
     * The width of the image, 0 defaults to 1
     */
    u32 width;
    /**
     * The height of the image, 0 defaults to 1
     */
    u32 height;
    /**
     * The depth of the image, 0 defaults to 1
     */
    u32 depth;
    /**
     * The dimensionality (1d, 2d, 3d), 0 defaults to 2d
     */
    u32 dimensions;
    /**
     * The number mipmaps to create space for, 0 defaults to 1, and UINT32_MAX
     * creates the maximum space for the size of the image
     */
    u32 mip_levels;
    /**
     * The number of array layers, 0 defaults to 1
     */
    u32 array_layers;
    /**
     * The number of samples for MSAA, must be a power of 2 between 1 and 64,
     * 0 defaults to 1
     */
    u32 multisample_count;
    /**
     * How the image will be used, must not be 0
     */
    VkImageUsageFlags usage;
    /**
     * Optional flags for image creation
     */
    VkImageCreateFlags flags;
} HarmonyVkImageConfig;

/**
 * Creates a Vulkan image
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - config Configuration describing the image, must not be NULL
 * Returns
 * - The created Vulkan image, should never be VK_NULL_HANDLE
 */
VkImage harmony_vk_create_image(const HarmonyVulkan *vk, const HarmonyVkImageConfig *config);

/**
 * Destroys a Vulkan image
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - image The Vulkan image to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_image(const HarmonyVulkan *vk, VkImage image);

/**
 * Configuration for a Vulkan image view
 */
typedef struct HarmonyVkImageViewConfig {
    /**
     * The type of the view (1d, 2d, 3d, cubemap, array, etc.)
     */
    VkImageViewType view_type;
    /**
     * The format of each pixel, must not be VK_FORMAT_UNDEFINED
     */
    VkFormat format;
    /**
     * The aspect mask, must not be 0
     */
    VkImageAspectFlags aspect;
    /**
     * The first mip level to use in the view
     */
    uint32_t base_mip;
    /**
     * The number of mips to use starting from base_mip, 0 defaults to 1
     */
    uint32_t mip_count;
    /**
     * The first array layer to use in the view
     */
    uint32_t base_layer;
    /**
     * The number of layers to use starting from base_layer, 0 defaults to 1
     */
    uint32_t layer_count;
} HarmonyVkImageViewConfig;

/**
 * Creates an image view for a Vulkan image
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - image The image to view, must not be VK_NULL_HANDLE
 * - config The view configuration, must not be NULL
 * Returns
 * - The created image view, should never be VK_NULL_HANDLE
 */
VkImageView harmony_vk_create_image_view(
    const HarmonyVulkan *vk,
    VkImage image,
    HarmonyVkImageViewConfig *config);

/**
 * Destroys a Vulkan image view
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - image_view The image view to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_image_view(const HarmonyVulkan *vk, VkImageView image_view);

/**
 * Creates a Vulkan sampler
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - filter The filtering method to use
 * - edge_mode The addressing mode for texture edges
 * Returns
 * - The created sampler, should never be VK_NULL_HANDLE
 */
VkSampler harmony_vk_create_sampler(const HarmonyVulkan *vk, VkFilter filter, VkSamplerAddressMode edge_mode);

/**
 * Destroys a Vulkan sampler
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - sampler The Vulkan sampler to destroy, must not be VK_NULL_HANDLE
 */
void harmony_vk_destroy_sampler(const HarmonyVulkan *vk, VkSampler sampler);

/**
 * Gets memory requirements for a buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - buffer The buffer to query, must not be VK_NULL_HANDLE
 * Returns
 * - The memory requirements for the buffer
 */
VkMemoryRequirements harmony_vk_get_buffer_mem_reqs(const HarmonyVulkan *vk, VkBuffer buffer);

/**
 * Gets memory requirements for an image
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - image The image to query, must not be VK_NULL_HANDLE
 * Returns
 * - The memory requirements for the image
 */
VkMemoryRequirements harmony_vk_get_image_mem_reqs(const HarmonyVulkan *vk, VkImage image);

/**
 * Allocates device memory matching the requirements
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - mem_reqs The required memory properties, must not be NULL
 * - desired_flags The flags the memory should have
 * - undesired_flags Flags that the memory should avoid
 * Returns
 * - The allocated memory, should never be VK_NULL_HANDLE
 */
VkDeviceMemory harmony_vk_allocate_memory(
    const HarmonyVulkan *vk,
    VkMemoryRequirements *mem_reqs,
    VkMemoryPropertyFlags desired_flags,
    VkMemoryPropertyFlags undesired_flags);

/**
 * Frees device memory
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - memory The memory to free, may be VK_NULL_HANDLE
 */
void harmony_vk_free_memory(const HarmonyVulkan *vk, VkDeviceMemory memory);

/**
 * Binds device memory to a buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - buffer The buffer to bind to, must not be VK_NULL_HANDLE
 * - memory The memory to bind, must not be VK_NULL_HANDLE
 * - offset Offset into memory
 */
void harmony_vk_bind_buffer_memory(const HarmonyVulkan *vk, VkBuffer buffer, VkDeviceMemory memory, usize offset);

/**
 * Binds device memory to an image
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - image The image to bind to, must not be VK_NULL_HANDLE
 * - memory The memory to bind, must not be VK_NULL_HANDLE
 * - offset Offset into memory
 */
void harmony_vk_bind_image_memory(const HarmonyVulkan *vk, VkImage image, VkDeviceMemory memory, usize offset);

/**
 * Maps device memory for CPU access
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - memory The memory to map, must not be VK_NULL_HANDLE
 * - offset The offset into memory
 * - size The region to map, must be greater than 0
 * Returns
 * - A pointer to mapped memory, never NULL unless mapping failed
 */
void *harmony_vk_map_memory(const HarmonyVulkan *vk, VkDeviceMemory memory, usize offset, usize size);

/**
 * Unmaps device memory
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - memory The memory to unmap, must not be VK_NULL_HANDLE
 */
void harmony_vk_unmap_memory(const HarmonyVulkan *vk, VkDeviceMemory memory);

/**
 * Flushes a range of device memory to make host writes visible to the GPU
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - memory The memory to flush, must not be VK_NULL_HANDLE
 * - offset Offset into the memory
 * - size Size of the memory region to flush, must be greater than 0
 */
void harmony_vk_flush_memory(const HarmonyVulkan *vk, VkDeviceMemory memory, usize offset, usize size);

/**
 * Invalidates a range of device memory to make GPU writes visible to the CPU
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - memory The memory to invalidate, must not be VK_NULL_HANDLE
 * - offset Offset into the memory
 * - size Size of the memory region to invalidate, must be greater than 0
 */
void harmony_vk_invalidate_memory(const HarmonyVulkan *vk, VkDeviceMemory memory, usize offset, usize size);

/**
 * Begins recording a command buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to begin recording, must not be VK_NULL_HANDLE
 * - flags Optional usage flags
 */
void harmony_vk_begin_cmd(const HarmonyVulkan *vk, VkCommandBuffer cmd, VkCommandBufferUsageFlags flags);

/**
 * Ends recording of a command buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to finish, must not be VK_NULL_HANDLE
 */
void harmony_vk_end_cmd(const HarmonyVulkan *vk, VkCommandBuffer cmd);

/**
 * Copies data from one buffer to another
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write the command to, must not be VK_NULL_HANDLE
 * - dst The destination buffer, must not be VK_NULL_HANDLE
 * - src The source buffer, must not be VK_NULL_HANDLE
 * - regions Copy regions, must not be NULL
 * - region_count Number of regions, must be greater than 0
 */
void harmony_vk_copy_buffer(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkBuffer dst,
    VkBuffer src,
    const VkBufferCopy *regions,
    u32 region_count);

/**
 * Copies data from one image to another
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write to, must not be VK_NULL_HANDLE
 * - dst The destination image, must not be VK_NULL_HANDLE
 * - src The source image, must not be VK_NULL_HANDLE
 * - regions Copy regions, must not be NULL
 * - region_count Number of copy regions, must be greater than 0
 */
void harmony_vk_copy_image(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkImage dst,
    VkImage src,
    const VkImageCopy *regions,
    u32 region_count);

/**
 * Performs a blit between images
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write to, must not be VK_NULL_HANDLE
 * - dst The destination image, must not be VK_NULL_HANDLE
 * - src The source image, must not be VK_NULL_HANDLE
 * - regions The blit regions, must not be NULL
 * - region_count Number of blit regions, must be greater than 0
 * - filter The filtering method to use
 */
void harmony_vk_blit_image(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkImage dst,
    VkImage src,
    const VkImageBlit *regions,
    u32 region_count,
    VkFilter filter);

/**
 * Copies data from a buffer to an image
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - dst The destination image, must not be VK_NULL_HANDLE
 * - src The source buffer, must not be VK_NULL_HANDLE
 * - regions Copy regions, must not be NULL
 * - region_count Count of regions, must be greater than 0
 */
void harmony_vk_copy_buffer_to_image(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkImage dst,
    VkBuffer src,
    VkBufferImageCopy *regions,
    u32 region_count);

/**
 * Copies data from an image to a buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - dst The destination buffer, must not be VK_NULL_HANDLE
 * - src The source image, must not be VK_NULL_HANDLE
 * - regions Copy regions, must not be NULL
 * - region_count Count of regions, must be greater than 0
 */
void harmony_vk_copy_image_to_buffer(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkBuffer dst,
    VkImage src,
    VkBufferImageCopy *regions,
    u32 region_count);

/**
 * Inserts a pipeline barrier using Vulkan synchronization2
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write to, must not be VK_NULL_HANDLE
 * - dependencies The dependency info, must not be NULL
 */
void harmony_vk_pipeline_barrier(const HarmonyVulkan *vk, VkCommandBuffer cmd, const VkDependencyInfo *dependencies);

/**
 * Begins a dynamic rendering pass
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - info The rendering info, must not be NULL
 */
void harmony_vk_begin_rendering(const HarmonyVulkan *vk, VkCommandBuffer cmd, const VkRenderingInfo *info);

/**
 * Ends a dynamic rendering pass
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 */
void harmony_vk_end_rendering(const HarmonyVulkan *vk, VkCommandBuffer cmd);

/**
 * Sets the viewport dynamically
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write the command to, must not be VK_NULL_HANDLE
 * - x Left coordinate of viewport
 * - y Top coordinate of viewport
 * - width Width of viewport
 * - height Height of viewport
 * - near Minimum depth value
 * - far Maximum depth value
 */
void harmony_vk_set_viewport(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    float x,
    float y,
    float width,
    float height,
    float near,
    float far);

/**
 * Sets the scissor rectangle dynamically
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write to, must not be VK_NULL_HANDLE
 * - x Left coordinate of scissor
 * - y Top coordinate of scissor
 * - width Width of scissor
 * - height Height of scissor
 */
void harmony_vk_set_scissor(const HarmonyVulkan *vk, VkCommandBuffer cmd, i32 x, i32 y, u32 width, u32 height);

/**
 * Binds a pipeline to a command buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to bind to, must not be VK_NULL_HANDLE
 * - pipeline The Vulkan pipeline to bind, must not be VK_NULL_HANDLE
 * - bind_point Graphics or compute bind point
 */
void harmony_vk_bind_pipeline(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkPipeline pipeline,
    VkPipelineBindPoint bind_point
);

/**
 * Binds descriptor sets to a pipeline
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - layout The pipeline layout, must not be VK_NULL_HANDLE
 * - bind_point Graphics or compute bind point
 * - begin_index First set index
 * - set_count Number of sets
 * - descriptor_sets The descriptor sets to bind, must not be NULL
 */
void harmony_vk_bind_descriptor_sets(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkPipelineLayout layout,
    VkPipelineBindPoint bind_point,
    u32 begin_index,
    u32 set_count,
    const VkDescriptorSet *descriptor_sets);

/**
 * Pushes constant values into the command buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - layout The pipeline layout, must not be VK_NULL_HANDLE
 * - stages Shader stages to update
 * - offset Byte offset into push constant range
 * - size Size of data, must be greater than 0
 * - data Pointer to constant data, must not be NULL
 */
void harmony_vk_push_constants(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkPipelineLayout layout,
    VkShaderStageFlags stages,
    u32 offset,
    u32 size,
    const void *data);

/**
 * Binds multiple vertex buffers
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - begin_index First binding index
 * - count Number of vertex buffers, must be greater than 0
 * - vertex_buffers Buffers to bind, must not be NULL
 * - offsets Byte offsets for each buffer, must not be NULL
 */
void harmony_vk_bind_vertex_buffers(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    u32 begin_index,
    u32 count,
    VkBuffer *vertex_buffers,
    usize *offsets);

/**
 * Binds a single vertex buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - vertex_buffer The vertex buffer to bind, must not be VK_NULL_HANDLE
 */
void harmony_vk_bind_vertex_buffer(const HarmonyVulkan *vk, VkCommandBuffer cmd, VkBuffer vertex_buffer);

/**
 * Binds an index buffer for indexed drawing
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer, must not be VK_NULL_HANDLE
 * - index_buffer The buffer containing indices, must not be VK_NULL_HANDLE
 * - offset Byte offset into the index buffer
 * - type The index type
 */
void harmony_vk_bind_index_buffer(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkBuffer index_buffer,
    usize offset,
    VkIndexType type);

/**
 * Draws the vertices to the framebuffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write, must not be VK_NULL_HANDLE
 * - first_vertex The beginning vertex in the vertex buffer to use
 * - vertex_count The number of vertices to read, must be greater than 0
 * - first_instance The beginning instance in to use
 * - instance_count The number of instances to draw, must be greater than 0
 */
void harmony_vk_draw(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    u32 first_vertex,
    u32 vertex_count,
    u32 first_instance,
    u32 instance_count);

/**
 * Draws the vertices to the framebuffer, using an index buffer
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write, must not be VK_NULL_HANDLE
 * - vertex_offset The offset into the vertices
 * - first_index The beginning index in the index buffer to use
 * - index_count The number of indices to read, must be greater than 0
 * - first_instance The beginning instance in to use
 * - instance_count The number of instances to draw, must be greater than 0
 */
void harmony_vk_draw_indexed(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    u32 vertex_offset,
    u32 first_index,
    u32 index_count,
    u32 first_instance,
    u32 instance_count);

/**
 * Dispatches the bound compute shader
 *
 * Parameters
 * - vk The Harmony Vulkan context, must not be NULL
 * - cmd The command buffer to write, must not be VK_NULL_HANDLE
 * - x The number of local workgroups in the x direction
 * - y The number of local workgroups in the x direction
 * - z The number of local workgroups in the x direction
 */
void harmony_vk_dispatch(const HarmonyVulkan *vk, VkCommandBuffer cmd, u32 x, u32 y, u32 z);

#if defined(HARMONY_IMPLEMENTATION_GRAPHICS) || defined(HARMONY_IMPLEMENTATION_ALL)

#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vulkan/vulkan_xlib.h>

typedef struct HarmonyX11Funcs {
    Display *(*XOpenDisplay)(_Xconst char*);
    int (*XCloseDisplay)(Display*);
    Window (*XCreateWindow)(Display*, Window, int, int, unsigned int, unsigned int,
        unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*);
    int (*XDestroyWindow)(Display*, Window);
    int (*XStoreName)(Display*, Window, _Xconst char*);
    Atom (*XInternAtom)(Display*, _Xconst char*, Bool);
    Status (*XSetWMProtocols)(Display*, Window, Atom*, int);
    int (*XMapWindow)(Display*, Window);
    Status (*XSendEvent)(Display*, Window, Bool, long, XEvent*);
    int (*XFlush)(Display*);
    int (*XNextEvent)(Display*, XEvent*);
    int (*XPending)(Display*);
    KeySym (*XLookupKeysym)(XKeyEvent*, int);
} HarmonyX11Funcs;

struct HarmonyPlatform {
    Display *display;
    void *lib;
    HarmonyX11Funcs pfn;
};

#define HARMONY_LOAD_X11_FUNC(name) *(void **)&platform->pfn. name \
    = harmony_dynamic_lib_load_symbol(platform->lib, #name); \
    if (platform->pfn. name == NULL) { harmony_error("Could not load Xlib function: " #name) }

HarmonyPlatform *harmony_platform_init(const HarmonyAllocator *allocator) {
    HarmonyPlatform *platform = harmony_alloc(allocator, sizeof(HarmonyPlatform));

    platform->lib = harmony_dynamic_lib_open("libX11.so.6");
    if (platform->lib == NULL)
        harmony_error("Could not open Xlib");

    HARMONY_LOAD_X11_FUNC(XOpenDisplay);
    HARMONY_LOAD_X11_FUNC(XCloseDisplay);
    HARMONY_LOAD_X11_FUNC(XCreateWindow);
    HARMONY_LOAD_X11_FUNC(XDestroyWindow);
    HARMONY_LOAD_X11_FUNC(XStoreName);
    HARMONY_LOAD_X11_FUNC(XInternAtom);
    HARMONY_LOAD_X11_FUNC(XSetWMProtocols);
    HARMONY_LOAD_X11_FUNC(XMapWindow);
    HARMONY_LOAD_X11_FUNC(XSendEvent);
    HARMONY_LOAD_X11_FUNC(XFlush);
    HARMONY_LOAD_X11_FUNC(XPending);
    HARMONY_LOAD_X11_FUNC(XNextEvent);
    HARMONY_LOAD_X11_FUNC(XLookupKeysym);

    platform->display = platform->pfn.XOpenDisplay(NULL);
    if (platform->display == NULL)
        harmony_error("Could not open X display");

    return platform;
}

void harmony_platform_shutdown(const HarmonyAllocator *allocator, HarmonyPlatform *platform) {
    platform->pfn.XCloseDisplay(platform->display);

    harmony_dynamic_lib_close(platform->lib);

    harmony_free(allocator, platform, sizeof(*platform));
}

struct HarmonyWindowInternals {
    Window window;
    Atom delete_atom;
};

static Window harmony_create_x11_window(
    const HarmonyPlatform *platform,
    Display *display,
    u32 width,
    u32 height,
    const char *title
) {
    XSetWindowAttributes window_attributes = {
        .event_mask
            = KeyPressMask
            | KeyReleaseMask
            | ButtonPressMask
            | ButtonReleaseMask
            | PointerMotionMask
            | StructureNotifyMask
    };
    Window window = platform->pfn.XCreateWindow(
        display, RootWindow(display, DefaultScreen(display)),
        0, 0,
        width,
        height,
        1,
        CopyFromParent,
        InputOutput,
        CopyFromParent,
        CWEventMask,
        &window_attributes
    );
    if (window == ~0U)
        harmony_error("X11 could not create window");

    int name_result = platform->pfn.XStoreName(display, window, title);
    if (name_result == 0)
        harmony_error("X11 could not set window title");

    int map_result = platform->pfn.XMapWindow(display, window);
    if (map_result == 0)
        harmony_error("X11 could not map window");

    return window;
}

static Atom harmony_set_delete_behavior(
    const HarmonyPlatform *platform,
    Display* display,
    Window window
) {
    Atom delete_atom = platform->pfn.XInternAtom(display, "WM_DELETE_WINDOW", False);
    if (delete_atom == None)
        harmony_error("X11 could not get WM_DELETE_WINDOW atom");

    int set_protocols_result = platform->pfn.XSetWMProtocols(
        display,
        window,
        &delete_atom,
        1
    );
    if (set_protocols_result == 0)
        harmony_error("X11 could not set WM_DELETE_WINDOW protocol");

    return delete_atom;
}

static void harmony_set_fullscreen(
    const HarmonyPlatform *platform,
    Display* display,
    Window window
) {
    Atom state_atom = platform->pfn.XInternAtom(display, "_NET_WM_STATE", False);
    if (state_atom == None)
        harmony_error("X11 failed to get state atom");

    Atom fullscreen_atom = platform->pfn.XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    if (fullscreen_atom == None)
        harmony_error("X11 failed to get fullscreen atom");

    int fullscreen_result = platform->pfn.XSendEvent(
        display,
        RootWindow(display, DefaultScreen(display)),
        False,
        SubstructureRedirectMask | SubstructureNotifyMask,
        &(XEvent){.xclient = {
            .type = ClientMessage,
            .window = window,
            .message_type = state_atom,
            .format = 32,
            .data.l = {
                [0] = 1,
                [1] = (long)fullscreen_atom,
                [2] = 0,
                [3] = 0,
                [4] = 0,
            },
        }}
    );
    if (fullscreen_result == 0)
        harmony_error("X11 could not send fullscreen message");
}

HarmonyWindow harmony_window_create(
    const HarmonyAllocator *allocator,
    const HarmonyPlatform *platform,
    const HarmonyWindowConfig *config
) {
    harmony_assert(platform != NULL);
    harmony_assert(config != NULL);

    HarmonyWindowInternals *x11_window = harmony_alloc(allocator, sizeof(HarmonyWindowInternals));

    u32 width = config->windowed ? config->width
        : (u32)DisplayWidth(platform->display, DefaultScreen(platform->display));
    u32 height = config->windowed ? config->height
        : (u32)DisplayHeight(platform->display, DefaultScreen(platform->display));

    x11_window->window = harmony_create_x11_window(platform, platform->display, width, height, config->title);

    x11_window->delete_atom = harmony_set_delete_behavior(platform, platform->display, x11_window->window);

    if (!config->windowed)
        harmony_set_fullscreen(platform, platform->display, x11_window->window);

    int flush_result = platform->pfn.XFlush(platform->display);
    if (flush_result == 0)
        harmony_error("X11 could not flush window");

    return (HarmonyWindow){
        .platform_internals = x11_window,
        .width = width,
        .height = height,
    };
}

void harmony_window_destroy(const HarmonyAllocator *allocator, const HarmonyPlatform *platform, HarmonyWindow *window) {
    harmony_assert(allocator != NULL);
    harmony_assert(platform != NULL);
    harmony_assert(window != NULL);

    platform->pfn.XDestroyWindow(platform->display, window->platform_internals->window);
    platform->pfn.XFlush(platform->display);

    harmony_free(allocator, window->platform_internals, sizeof(*window->platform_internals));
}

void harmony_window_process_events(const HarmonyPlatform *platform, HarmonyWindow **windows, u32 window_count) {
    harmony_assert(platform != NULL);
    harmony_assert(windows != NULL);
    harmony_assert(window_count > 0);

    if (window_count > 1)
        harmony_error("Multiple windows unsupported"); // : TODO
    HarmonyWindow* window = windows[0];

    memset(window->keys_pressed, 0, sizeof(window->keys_pressed));
    memset(window->keys_released, 0, sizeof(window->keys_released));
    window->was_resized = false;

    u32 new_window_width = window->width;
    u32 new_window_height = window->height;
    f32 new_mouse_pos_x = window->mouse_pos_x;
    f32 new_mouse_pos_y = window->mouse_pos_y;

    while (platform->pfn.XPending(platform->display)) {
        XEvent event;
        int event_result = platform->pfn.XNextEvent(platform->display, &event);
        if (event_result != 0)
            harmony_error("X11 could not get next event");

        switch (event.type) {
            case ConfigureNotify: {
                new_window_width = (u32)event.xconfigure.width;
                new_window_height = (u32)event.xconfigure.height;
            } break;
            case ClientMessage: {
                if ((Atom)event.xclient.data.l[0] == window->platform_internals->delete_atom)
                    window->was_closed = true;
            } break;
            case KeyPress: {
                switch (platform->pfn.XLookupKeysym(&event.xkey, 0)) {
                    case XK_Escape: {
                        window->keys_pressed[HARMONY_KEY_ESCAPE] = true;
                        window->keys_down[HARMONY_KEY_ESCAPE] = true;
                    } break;
                    case XK_space: {
                        window->keys_pressed[HARMONY_KEY_SPACE] = true;
                        window->keys_down[HARMONY_KEY_SPACE] = true;
                    } break;
                    case XK_Return: {
                        window->keys_pressed[HARMONY_KEY_ENTER] = true;
                        window->keys_down[HARMONY_KEY_ENTER] = true;
                    } break;
                    case XK_Tab: {
                        window->keys_pressed[HARMONY_KEY_TAB] = true;
                        window->keys_down[HARMONY_KEY_TAB] = true;
                    } break;
                    case XK_Delete: {
                        window->keys_pressed[HARMONY_KEY_DELETE] = true;
                        window->keys_down[HARMONY_KEY_DELETE] = true;
                    } break;
                    case XK_BackSpace: {
                        window->keys_pressed[HARMONY_KEY_BACKSPACE] = true;
                        window->keys_down[HARMONY_KEY_BACKSPACE] = true;
                    } break;
                    case XK_Up: {
                        window->keys_pressed[HARMONY_KEY_UP] = true;
                        window->keys_down[HARMONY_KEY_UP] = true;
                    } break;
                    case XK_Down: {
                        window->keys_pressed[HARMONY_KEY_DOWN] = true;
                        window->keys_down[HARMONY_KEY_DOWN] = true;
                    } break;
                    case XK_Left: {
                        window->keys_pressed[HARMONY_KEY_LEFT] = true;
                        window->keys_down[HARMONY_KEY_LEFT] = true;
                    } break;
                    case XK_Right: {
                        window->keys_pressed[HARMONY_KEY_RIGHT] = true;
                        window->keys_down[HARMONY_KEY_RIGHT] = true;
                    } break;
                    case XK_a: {
                        window->keys_pressed[HARMONY_KEY_A] = true;
                        window->keys_down[HARMONY_KEY_A] = true;
                    } break;
                    case XK_b: {
                        window->keys_pressed[HARMONY_KEY_B] = true;
                        window->keys_down[HARMONY_KEY_B] = true;
                    } break;
                    case XK_c: {
                        window->keys_pressed[HARMONY_KEY_C] = true;
                        window->keys_down[HARMONY_KEY_C] = true;
                    } break;
                    case XK_d: {
                        window->keys_pressed[HARMONY_KEY_D] = true;
                        window->keys_down[HARMONY_KEY_D] = true;
                    } break;
                    case XK_e: {
                        window->keys_pressed[HARMONY_KEY_E] = true;
                        window->keys_down[HARMONY_KEY_E] = true;
                    } break;
                    case XK_f: {
                        window->keys_pressed[HARMONY_KEY_F] = true;
                        window->keys_down[HARMONY_KEY_F] = true;
                    } break;
                    case XK_g: {
                        window->keys_pressed[HARMONY_KEY_G] = true;
                        window->keys_down[HARMONY_KEY_G] = true;
                    } break;
                    case XK_h: {
                        window->keys_pressed[HARMONY_KEY_H] = true;
                        window->keys_down[HARMONY_KEY_H] = true;
                    } break;
                    case XK_i: {
                        window->keys_pressed[HARMONY_KEY_I] = true;
                        window->keys_down[HARMONY_KEY_I] = true;
                    } break;
                    case XK_j: {
                        window->keys_pressed[HARMONY_KEY_J] = true;
                        window->keys_down[HARMONY_KEY_J] = true;
                    } break;
                    case XK_k: {
                        window->keys_pressed[HARMONY_KEY_K] = true;
                        window->keys_down[HARMONY_KEY_K] = true;
                    } break;
                    case XK_l: {
                        window->keys_pressed[HARMONY_KEY_L] = true;
                        window->keys_down[HARMONY_KEY_L] = true;
                    } break;
                    case XK_m: {
                        window->keys_pressed[HARMONY_KEY_M] = true;
                        window->keys_down[HARMONY_KEY_M] = true;
                    } break;
                    case XK_n: {
                        window->keys_pressed[HARMONY_KEY_N] = true;
                        window->keys_down[HARMONY_KEY_N] = true;
                    } break;
                    case XK_o: {
                        window->keys_pressed[HARMONY_KEY_O] = true;
                        window->keys_down[HARMONY_KEY_O] = true;
                    } break;
                    case XK_p: {
                        window->keys_pressed[HARMONY_KEY_P] = true;
                        window->keys_down[HARMONY_KEY_P] = true;
                    } break;
                    case XK_q: {
                        window->keys_pressed[HARMONY_KEY_Q] = true;
                        window->keys_down[HARMONY_KEY_Q] = true;
                    } break;
                    case XK_r: {
                        window->keys_pressed[HARMONY_KEY_R] = true;
                        window->keys_down[HARMONY_KEY_R] = true;
                    } break;
                    case XK_s: {
                        window->keys_pressed[HARMONY_KEY_S] = true;
                        window->keys_down[HARMONY_KEY_S] = true;
                    } break;
                    case XK_t: {
                        window->keys_pressed[HARMONY_KEY_T] = true;
                        window->keys_down[HARMONY_KEY_T] = true;
                    } break;
                    case XK_u: {
                        window->keys_pressed[HARMONY_KEY_U] = true;
                        window->keys_down[HARMONY_KEY_U] = true;
                    } break;
                    case XK_v: {
                        window->keys_pressed[HARMONY_KEY_V] = true;
                        window->keys_down[HARMONY_KEY_V] = true;
                    } break;
                    case XK_w: {
                        window->keys_pressed[HARMONY_KEY_W] = true;
                        window->keys_down[HARMONY_KEY_W] = true;
                    } break;
                    case XK_x: {
                        window->keys_pressed[HARMONY_KEY_X] = true;
                        window->keys_down[HARMONY_KEY_X] = true;
                    } break;
                    case XK_y: {
                        window->keys_pressed[HARMONY_KEY_Y] = true;
                        window->keys_down[HARMONY_KEY_Y] = true;
                    } break;
                    case XK_z: {
                        window->keys_pressed[HARMONY_KEY_Z] = true;
                        window->keys_down[HARMONY_KEY_Z] = true;
                    } break;
                    case XK_Shift_L: {
                        window->keys_pressed[HARMONY_KEY_LSHIFT] = true;
                        window->keys_down[HARMONY_KEY_LSHIFT] = true;
                    } break;
                    case XK_Shift_R: {
                        window->keys_pressed[HARMONY_KEY_RSHIFT] = true;
                        window->keys_down[HARMONY_KEY_RSHIFT] = true;
                    } break;
                    case XK_Control_L: {
                        window->keys_pressed[HARMONY_KEY_LCTRL] = true;
                        window->keys_down[HARMONY_KEY_LCTRL] = true;
                    } break;
                    case XK_Control_R: {
                        window->keys_pressed[HARMONY_KEY_RCTRL] = true;
                        window->keys_down[HARMONY_KEY_RCTRL] = true;
                    } break;
                    case XK_Alt_L: {
                        window->keys_pressed[HARMONY_KEY_LALT] = true;
                        window->keys_down[HARMONY_KEY_LALT] = true;
                    } break;
                    case XK_Alt_R: {
                        window->keys_pressed[HARMONY_KEY_RALT] = true;
                        window->keys_down[HARMONY_KEY_RALT] = true;
                    } break;
                }
            } break;
            case KeyRelease: {
                switch (platform->pfn.XLookupKeysym(&event.xkey, 0)) {
                    case XK_Escape: {
                        window->keys_released[HARMONY_KEY_ESCAPE] = true;
                        window->keys_down[HARMONY_KEY_ESCAPE] = false;
                    } break;
                    case XK_space: {
                        window->keys_released[HARMONY_KEY_SPACE] = true;
                        window->keys_down[HARMONY_KEY_SPACE] = false;
                    } break;
                    case XK_Return: {
                        window->keys_released[HARMONY_KEY_ENTER] = true;
                        window->keys_down[HARMONY_KEY_ENTER] = false;
                    } break;
                    case XK_Tab: {
                        window->keys_released[HARMONY_KEY_TAB] = true;
                        window->keys_down[HARMONY_KEY_TAB] = false;
                    } break;
                    case XK_Delete: {
                        window->keys_released[HARMONY_KEY_DELETE] = true;
                        window->keys_down[HARMONY_KEY_DELETE] = false;
                    } break;
                    case XK_BackSpace: {
                        window->keys_released[HARMONY_KEY_BACKSPACE] = true;
                        window->keys_down[HARMONY_KEY_BACKSPACE] = false;
                    } break;
                    case XK_Up: {
                        window->keys_released[HARMONY_KEY_UP] = true;
                        window->keys_down[HARMONY_KEY_UP] = false;
                    } break;
                    case XK_Down: {
                        window->keys_released[HARMONY_KEY_DOWN] = true;
                        window->keys_down[HARMONY_KEY_DOWN] = false;
                    } break;
                    case XK_Left: {
                        window->keys_released[HARMONY_KEY_LEFT] = true;
                        window->keys_down[HARMONY_KEY_LEFT] = false;
                    } break;
                    case XK_Right: {
                        window->keys_released[HARMONY_KEY_RIGHT] = true;
                        window->keys_down[HARMONY_KEY_RIGHT] = false;
                    } break;
                    case XK_a: {
                        window->keys_released[HARMONY_KEY_A] = true;
                        window->keys_down[HARMONY_KEY_A] = false;
                    } break;
                    case XK_b: {
                        window->keys_released[HARMONY_KEY_B] = true;
                        window->keys_down[HARMONY_KEY_B] = false;
                    } break;
                    case XK_c: {
                        window->keys_released[HARMONY_KEY_C] = true;
                        window->keys_down[HARMONY_KEY_C] = false;
                    } break;
                    case XK_d: {
                        window->keys_released[HARMONY_KEY_D] = true;
                        window->keys_down[HARMONY_KEY_D] = false;
                    } break;
                    case XK_e: {
                        window->keys_released[HARMONY_KEY_E] = true;
                        window->keys_down[HARMONY_KEY_E] = false;
                    } break;
                    case XK_f: {
                        window->keys_released[HARMONY_KEY_F] = true;
                        window->keys_down[HARMONY_KEY_F] = false;
                    } break;
                    case XK_g: {
                        window->keys_released[HARMONY_KEY_G] = true;
                        window->keys_down[HARMONY_KEY_G] = false;
                    } break;
                    case XK_h: {
                        window->keys_released[HARMONY_KEY_H] = true;
                        window->keys_down[HARMONY_KEY_H] = false;
                    } break;
                    case XK_i: {
                        window->keys_released[HARMONY_KEY_I] = true;
                        window->keys_down[HARMONY_KEY_I] = false;
                    } break;
                    case XK_j: {
                        window->keys_released[HARMONY_KEY_J] = true;
                        window->keys_down[HARMONY_KEY_J] = false;
                    } break;
                    case XK_k: {
                        window->keys_released[HARMONY_KEY_K] = true;
                        window->keys_down[HARMONY_KEY_K] = false;
                    } break;
                    case XK_l: {
                        window->keys_released[HARMONY_KEY_L] = true;
                        window->keys_down[HARMONY_KEY_L] = false;
                    } break;
                    case XK_m: {
                        window->keys_released[HARMONY_KEY_M] = true;
                        window->keys_down[HARMONY_KEY_M] = false;
                    } break;
                    case XK_n: {
                        window->keys_released[HARMONY_KEY_N] = true;
                        window->keys_down[HARMONY_KEY_N] = false;
                    } break;
                    case XK_o: {
                        window->keys_released[HARMONY_KEY_O] = true;
                        window->keys_down[HARMONY_KEY_O] = false;
                    } break;
                    case XK_p: {
                        window->keys_released[HARMONY_KEY_P] = true;
                        window->keys_down[HARMONY_KEY_P] = false;
                    } break;
                    case XK_q: {
                        window->keys_released[HARMONY_KEY_Q] = true;
                        window->keys_down[HARMONY_KEY_Q] = false;
                    } break;
                    case XK_r: {
                        window->keys_released[HARMONY_KEY_R] = true;
                        window->keys_down[HARMONY_KEY_R] = false;
                    } break;
                    case XK_s: {
                        window->keys_released[HARMONY_KEY_S] = true;
                        window->keys_down[HARMONY_KEY_S] = false;
                    } break;
                    case XK_t: {
                        window->keys_released[HARMONY_KEY_T] = true;
                        window->keys_down[HARMONY_KEY_T] = false;
                    } break;
                    case XK_u: {
                        window->keys_released[HARMONY_KEY_U] = true;
                        window->keys_down[HARMONY_KEY_U] = false;
                    } break;
                    case XK_v: {
                        window->keys_released[HARMONY_KEY_V] = true;
                        window->keys_down[HARMONY_KEY_V] = false;
                    } break;
                    case XK_w: {
                        window->keys_released[HARMONY_KEY_W] = true;
                        window->keys_down[HARMONY_KEY_W] = false;
                    } break;
                    case XK_x: {
                        window->keys_released[HARMONY_KEY_X] = true;
                        window->keys_down[HARMONY_KEY_X] = false;
                    } break;
                    case XK_y: {
                        window->keys_released[HARMONY_KEY_Y] = true;
                        window->keys_down[HARMONY_KEY_Y] = false;
                    } break;
                    case XK_z: {
                        window->keys_released[HARMONY_KEY_Z] = true;
                        window->keys_down[HARMONY_KEY_Z] = false;
                    } break;
                    case XK_Shift_L: {
                        window->keys_released[HARMONY_KEY_LSHIFT] = true;
                        window->keys_down[HARMONY_KEY_LSHIFT] = false;
                    } break;
                    case XK_Shift_R: {
                        window->keys_released[HARMONY_KEY_RSHIFT] = true;
                        window->keys_down[HARMONY_KEY_RSHIFT] = false;
                    } break;
                    case XK_Control_L: {
                        window->keys_released[HARMONY_KEY_LCTRL] = true;
                        window->keys_down[HARMONY_KEY_LCTRL] = false;
                    } break;
                    case XK_Control_R: {
                        window->keys_released[HARMONY_KEY_RCTRL] = true;
                        window->keys_down[HARMONY_KEY_RCTRL] = false;
                    } break;
                    case XK_Alt_L: {
                        window->keys_released[HARMONY_KEY_LALT] = true;
                        window->keys_down[HARMONY_KEY_LALT] = false;
                    } break;
                    case XK_Alt_R: {
                        window->keys_released[HARMONY_KEY_RALT] = true;
                        window->keys_down[HARMONY_KEY_RALT] = false;
                    } break;
                }
            } break;
            case ButtonPress: {
                switch (event.xbutton.button) {
                    case Button1: {
                        window->keys_pressed[HARMONY_KEY_LMOUSE] = true;
                        window->keys_down[HARMONY_KEY_LMOUSE] = true;
                    } break;
                    case Button2: {
                        window->keys_pressed[HARMONY_KEY_RMOUSE] = true;
                        window->keys_down[HARMONY_KEY_RMOUSE] = true;
                    } break;
                    case Button3: {
                        window->keys_pressed[HARMONY_KEY_MMOUSE] = true;
                        window->keys_down[HARMONY_KEY_MMOUSE] = true;
                    } break;
                }
            } break;
            case ButtonRelease: {
                switch (event.xbutton.button) {
                    case Button1: {
                        window->keys_released[HARMONY_KEY_LMOUSE] = true;
                        window->keys_down[HARMONY_KEY_LMOUSE] = false;
                    } break;
                    case Button2: {
                        window->keys_released[HARMONY_KEY_RMOUSE] = true;
                        window->keys_down[HARMONY_KEY_RMOUSE] = false;
                    } break;
                    case Button3: {
                        window->keys_released[HARMONY_KEY_MMOUSE] = true;
                        window->keys_down[HARMONY_KEY_MMOUSE] = false;
                    } break;
                }
            } break;
            case MotionNotify: {
                new_mouse_pos_x = (f32)event.xmotion.x;
                new_mouse_pos_y = (f32)event.xmotion.y;
            } break;
            default: break;
        }
    }

    if (new_window_width != window->width ||
        new_window_height != window->height) {
        window->was_resized = true;
    }

    window->mouse_delta_x = new_mouse_pos_x - window->mouse_pos_x;
    window->mouse_delta_y = new_mouse_pos_y - window->mouse_pos_y;
    window->mouse_pos_x = new_mouse_pos_x;
    window->mouse_pos_y = new_mouse_pos_y;
}

#else // __linux__

#error "unsupported platform, supported: linux"

#endif // __linux__

#define HARMONY_LOAD_VULKAN_FUNC(name) \
    vk->pfn. name = (PFN_##name)vk->pfn.vkGetInstanceProcAddr(NULL, #name); \
    if (vk->pfn. name == NULL) { \
        harmony_error("Could not load " #name); \
    }

static void harmony_load_vulkan(HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);

    vk->lib = harmony_dynamic_lib_open("libvulkan.so.1");
    if (vk->lib == NULL)
        harmony_error("Could not load vulkan");

    *(void **)&vk->pfn.vkGetInstanceProcAddr
        = harmony_dynamic_lib_load_symbol(vk->lib, "vkGetInstanceProcAddr");

    HARMONY_LOAD_VULKAN_FUNC(vkEnumerateInstanceExtensionProperties);
    HARMONY_LOAD_VULKAN_FUNC(vkEnumerateInstanceLayerProperties);
    HARMONY_LOAD_VULKAN_FUNC(vkCreateInstance);
}

#undef HARMONY_LOAD_VULKAN_FUNC

#define HARMONY_LOAD_VULKAN_INSTANCE_FUNC(name) \
    vk->pfn. name = (PFN_##name)vk->pfn.vkGetInstanceProcAddr(vk->instance, #name); \
    if (vk->pfn. name == NULL) { harmony_error("Could not load " #name); }

static void harmony_load_vulkan_instance(HarmonyVulkan *vk) {
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetDeviceProcAddr);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkDestroyInstance);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkCreateDebugUtilsMessengerEXT);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkDestroyDebugUtilsMessengerEXT);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkEnumeratePhysicalDevices);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkEnumerateDeviceExtensionProperties);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceQueueFamilyProperties);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceFeatures);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceProperties);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceMemoryProperties);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceSurfaceFormatsKHR);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceSurfacePresentModesKHR);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    HARMONY_LOAD_VULKAN_INSTANCE_FUNC(vkCreateDevice)
}

#undef HARMONY_LOAD_VULKAN_INSTANCE_FUNC

#define HARMONY_LOAD_VULKAN_DEVICE_FUNC(name) \
    vk->pfn. name = (PFN_##name)vk->pfn.vkGetDeviceProcAddr(vk->device, #name); \
    if (vk->pfn. name == NULL) { harmony_error("Could not load " #name); }

static void harmony_load_vulkan_device(HarmonyVulkan *vk) {
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyDevice)
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDeviceWaitIdle)

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroySurfaceKHR)
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateSwapchainKHR)
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroySwapchainKHR)
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkGetSwapchainImagesKHR)
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkAcquireNextImageKHR)

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateSemaphore);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroySemaphore);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateFence);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyFence);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkResetFences);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkWaitForFences);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkGetDeviceQueue);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkQueueWaitIdle);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkQueueSubmit2);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkQueuePresentKHR);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateCommandPool);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyCommandPool);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkAllocateCommandBuffers);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkFreeCommandBuffers);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateDescriptorPool);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyDescriptorPool);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkResetDescriptorPool);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkAllocateDescriptorSets);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkUpdateDescriptorSets);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateDescriptorSetLayout);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyDescriptorSetLayout);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreatePipelineLayout);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyPipelineLayout);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateShaderModule);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyShaderModule);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateGraphicsPipelines);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateComputePipelines);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyPipeline);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateImage);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyImage);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateImageView);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroyImageView);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCreateSampler);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkDestroySampler);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkGetBufferMemoryRequirements);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkGetImageMemoryRequirements);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkAllocateMemory);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkFreeMemory);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkBindBufferMemory);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkBindImageMemory);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkMapMemory);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkUnmapMemory);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkFlushMappedMemoryRanges);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkInvalidateMappedMemoryRanges);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkBeginCommandBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkEndCommandBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkResetCommandBuffer);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdCopyBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdCopyImage);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdBlitImage);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdCopyBufferToImage);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdCopyImageToBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdPipelineBarrier2);

    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdBeginRendering);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdEndRendering);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdSetViewport);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdSetScissor);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdBindPipeline);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdBindDescriptorSets);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdPushConstants);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdBindVertexBuffers);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdBindIndexBuffer);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdDraw);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdDrawIndexed);
    HARMONY_LOAD_VULKAN_DEVICE_FUNC(vkCmdDispatch);
}

#undef HARMONY_LOAD_VULKAN_DEVICE_FUNC

static const char* const DeviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

HarmonyVulkan harmony_vulkan_create(void) {
    HarmonyVulkan vk;

    harmony_load_vulkan(&vk);

    vk.instance = harmony_vk_create_instance(&vk);

    harmony_load_vulkan_instance(&vk);

    harmony_debug_mode(vk.debug_messenger = harmony_vk_create_debug_messenger(&vk));
    vk.gpu = harmony_vk_find_physical_device(&vk);
    vk.device = harmony_vk_create_device(&vk);

    harmony_load_vulkan_device(&vk);

    if (!harmony_vk_find_queue_family(&vk, &vk.queue_family, VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT))
        harmony_error("Could not find Vulkan queue family");
    
    return vk;
}

void harmony_vulkan_destroy(HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);
    harmony_assert(vk->instance != VK_NULL_HANDLE);
    harmony_assert(vk->debug_messenger != VK_NULL_HANDLE);
    harmony_assert(vk->gpu != VK_NULL_HANDLE);
    harmony_assert(vk->device != VK_NULL_HANDLE);

    vk->pfn.vkDestroyDevice(vk->device, NULL);
    harmony_debug_mode(vk->pfn.vkDestroyDebugUtilsMessengerEXT(vk->instance, vk->debug_messenger, NULL));
    vk->pfn.vkDestroyInstance(vk->instance, NULL);
    harmony_dynamic_lib_close(vk->lib);
}

static void harmony_check_instance_extensions(
    const HarmonyVulkan *vk,
    const char **exts,
    u32 ext_count
) {
    harmony_assert(vk != NULL);
    if (ext_count > 0) {
        harmony_assert(exts != NULL);
    } else {
        return;
    }

    u32 ext_prop_count = 0;
    VkResult ext_count_res = vk->pfn.vkEnumerateInstanceExtensionProperties(
        NULL, &ext_prop_count, NULL);
    switch (ext_count_res) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan incomplete instance extension enumeration");
            break;
        }
        case VK_ERROR_LAYER_NOT_PRESENT: harmony_error("Vulkan layer not present");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }

    VkExtensionProperties* ext_props = malloc(
        ext_prop_count * sizeof(VkExtensionProperties));

    VkResult ext_res = vk->pfn.vkEnumerateInstanceExtensionProperties(
        NULL, &ext_prop_count, ext_props);
    switch (ext_res) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan incomplete instance extension enumeration");
            break;
        }
        case VK_ERROR_LAYER_NOT_PRESENT: harmony_error("Vulkan layer not present");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }

    for (usize i = 0; i < ext_count; ++i) {
        for (usize j = 0; j < ext_prop_count; ++j) {
            if (strcmp(exts[i], ext_props[j].extensionName) == 0)
                goto next_ext;
        }
        harmony_error("Could not find Vulkan instance extension: %s", exts[i]);
next_ext:
        continue;
    }

    free(ext_props);
}

static void harmony_check_instance_layers(
    const HarmonyVulkan *vk,
    const char **layers,
    u32 layer_count
) {
    if (layer_count > 0) {
        harmony_assert(layers != NULL);
    } else {
        return;
    }

    u32 prop_count = 0;
    VkResult count_result = vk->pfn.vkEnumerateInstanceLayerProperties(&prop_count, NULL);
    switch (count_result) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan incomplete instance layer enumeration");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    VkLayerProperties* layer_props = malloc(prop_count * sizeof(VkLayerProperties));
    VkResult props_result = vk->pfn.vkEnumerateInstanceLayerProperties(&prop_count, layer_props);
    switch (props_result) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan incomplete instance layer enumeration");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    for (usize i = 0; i < layer_count; ++i) {
        for (usize j = 0; j < prop_count; ++j) {
            if (strcmp(layers[i], layer_props[j].layerName) == 0)
                goto next_layer;
        }
        harmony_error("Could not find Vulkan instance: %s", layers[i]);
next_layer:
        continue;
    }
}

static VkBool32 harmony_debug_callback(
    const VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    const VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
) {
    (void)type;
    (void)user_data;

    if (severity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                 |  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)) {
        HARMONY_LOG_FUNCTION("Vulkan Info: %s\n", callback_data->pMessage);
    } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        HARMONY_LOG_FUNCTION("Vulkan Warning: %s\n", callback_data->pMessage);
    } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        HARMONY_LOG_FUNCTION("Vulkan Error: %s\n", callback_data->pMessage);
    } else {
        HARMONY_LOG_FUNCTION("Vulkan Unknown: %s\n", callback_data->pMessage);
    }
    return VK_FALSE;
}

static const VkDebugUtilsMessengerCreateInfoEXT harmony_debug_utils_messenger_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = harmony_debug_callback,
};

VkInstance harmony_vk_create_instance(const HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Hurdy Gurdy",
        .applicationVersion = 0,
        .pEngineName = "Hurdy Gurdy",
        .engineVersion = 0,
        .apiVersion = VK_API_VERSION_1_3,
    };

#ifndef NDEBUG
    const char* layers[] = {
        "VK_LAYER_KHRONOS_validation",
    };
    harmony_check_instance_layers(vk, layers, harmony_countof(layers));
#endif // NDEBUG

    const char* exts[] = {
#ifndef NDEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif // NDEBUG
#ifdef __linux__
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#else // __linux__
#error "unsupported platform, supported platforms: linux"
#endif // __linux__
    };
    harmony_check_instance_extensions(vk, exts, harmony_countof(exts));

    VkInstanceCreateInfo instance_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifndef NDEBUG
        .pNext = &harmony_debug_utils_messenger_create_info,
#endif // NDEBUG
        .pApplicationInfo = &app_info,
#ifndef NDEBUG
        .enabledLayerCount = harmony_countof(layers),
        .ppEnabledLayerNames = layers,
#endif // NDEBUG
        .enabledExtensionCount = harmony_countof(exts),
        .ppEnabledExtensionNames = exts,
    };

    VkInstance instance = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateInstance(&instance_info, NULL, &instance);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_LAYER_NOT_PRESENT: harmony_error("Required Vulkan layer not present");
        case VK_ERROR_EXTENSION_NOT_PRESENT: harmony_error("Required Vulkan extension not present");
        case VK_ERROR_INCOMPATIBLE_DRIVER: harmony_error("Incompatible Vulkan driver");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INITIALIZATION_FAILED: harmony_error("Vulkan initialization failed");
        default: harmony_error("Unexpected Vulkan error");
    }

    return instance;
}

void harmony_vk_destroy_instance(const HarmonyVulkan *vk, VkInstance instance) {
    harmony_assert(vk != NULL);
    harmony_assert(instance != VK_NULL_HANDLE);
    vk->pfn.vkDestroyInstance(instance, NULL);
}

VkDebugUtilsMessengerEXT harmony_vk_create_debug_messenger(const HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);
    harmony_assert(vk->instance != VK_NULL_HANDLE);

    VkDebugUtilsMessengerEXT messenger = NULL;
    VkResult result = vk->pfn.vkCreateDebugUtilsMessengerEXT(
        vk->instance,
        &harmony_debug_utils_messenger_create_info,
        NULL,
        &messenger
    );
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        default: harmony_error("Unexpected Vulkan error");
    }

    return messenger;
}

void harmony_vk_destroy_debug_messenger(const HarmonyVulkan *vk, VkDebugUtilsMessengerEXT messenger) {
    harmony_assert(vk != NULL);
    harmony_assert(messenger != NULL);
    vk->pfn.vkDestroyDebugUtilsMessengerEXT(vk->instance, messenger, NULL);
}

VkPhysicalDevice harmony_vk_find_physical_device(const HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);
    harmony_assert(vk->instance != NULL);

    u32 gpu_count;
    VkResult gpu_count_res = vk->pfn.vkEnumeratePhysicalDevices(vk->instance, &gpu_count, NULL);
    switch (gpu_count_res) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan incomplete gpu enumeration");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INITIALIZATION_FAILED: harmony_error("Vulkan initialization failed");
        default: harmony_error("Unexpected Vulkan error");
    }

#define HARMONY_MAX_GPUS 8
    if (gpu_count > HARMONY_MAX_GPUS)
        harmony_error("Too many Vulkan physical devices");
    VkPhysicalDevice gpus[HARMONY_MAX_GPUS];
#undef HARMONY_MAX_GPUS

    VkResult gpu_result = vk->pfn.vkEnumeratePhysicalDevices(vk->instance, &gpu_count, gpus);
    switch (gpu_result) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan incomplete gpu enumeration");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INITIALIZATION_FAILED: harmony_error("Vulkan initialization failed");
        default: harmony_error("Unexpected Vulkan error");
    }

    u32 ext_prop_count = 0;
    VkExtensionProperties* ext_props = NULL;

    VkPhysicalDevice suitable_gpu = VK_NULL_HANDLE;
    for (u32 i = 0; i < gpu_count; ++i) {
        VkPhysicalDevice gpu = gpus[i];

        VkPhysicalDeviceFeatures features = {0};
        vk->pfn.vkGetPhysicalDeviceFeatures(gpu, &features);
        if (features.sampleRateShading != VK_TRUE ||
            features.samplerAnisotropy != VK_TRUE)
            continue;

        u32 new_prop_count = 0;
        VkResult ext_count_res = vk->pfn.vkEnumerateDeviceExtensionProperties(gpu, NULL, &new_prop_count, NULL);
        switch (ext_count_res) {
            case VK_SUCCESS: break;
            case VK_INCOMPLETE: continue;
            case VK_ERROR_LAYER_NOT_PRESENT: continue;
            case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
            default: harmony_error("Unexpected Vulkan error");
        }

        if (new_prop_count > ext_prop_count) {
            ext_prop_count = new_prop_count;
            ext_props = realloc(ext_props, ext_prop_count * sizeof(VkExtensionProperties));
        }

        VkResult ext_res = vk->pfn.vkEnumerateDeviceExtensionProperties(gpu, NULL, &new_prop_count, ext_props);
        switch (ext_res) {
            case VK_SUCCESS: break;
            case VK_INCOMPLETE: {
                harmony_log_warning("Vulkan incomplete gpu extension enumeration");
                continue;
            }
            case VK_ERROR_LAYER_NOT_PRESENT: {
                harmony_log_warning("Vulkan layer not present");
                continue;
            }
            case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
            default: harmony_error("Unexpected Vulkan error");
        }

        bool found_all_extensions = true;
        for (usize j = 0; j < harmony_countof(DeviceExtensions); j++) {
            bool found_extension = false;
            for (usize k = 0; k < new_prop_count; k++) {
                if (strcmp(DeviceExtensions[j], ext_props[k].extensionName) == 0) {
                    found_extension = true;
                    break;
                }
            }
            if (!found_extension) {
                found_all_extensions = false;
                break;
            }
        }
        if (!found_all_extensions)
            continue;

        u32 queue_family_count = 0;
        vk->pfn.vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, NULL);

#define HARMONY_MAX_QUEUE_FAMILIES 32
        if (queue_family_count > HARMONY_MAX_QUEUE_FAMILIES)
            harmony_error("Too many Vulkan queue families");
        VkQueueFamilyProperties queue_families[HARMONY_MAX_QUEUE_FAMILIES];
#undef HARMONY_MAX_QUEUE_FAMILIES

        vk->pfn.vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, queue_families);

        bool found_queue_family = false;
        for (u32 j = 0; j < queue_family_count; ++j) {
            if (queue_families[j].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) {
                found_queue_family = true;
                break;
            }
        }
        if (!found_queue_family)
            continue;

        suitable_gpu = gpu;
        break;
    }
    if (suitable_gpu == VK_NULL_HANDLE)
        harmony_error("Could not find suitable gpu");

    free(ext_props);
    return suitable_gpu;
}

VkDevice harmony_vk_create_device(const HarmonyVulkan *vk) {
    harmony_assert(vk->gpu != NULL);

    VkPhysicalDeviceDynamicRenderingFeatures dynamic_rendering_feature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
        .dynamicRendering = VK_TRUE,
    };
    VkPhysicalDeviceSynchronization2Features synchronization2_feature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
        .pNext = &dynamic_rendering_feature,
        .synchronization2 = VK_TRUE,
    };
    VkPhysicalDeviceFeatures features = {
        .sampleRateShading = VK_TRUE,
        .samplerAnisotropy = VK_TRUE,
    };

    float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = vk->queue_family,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority
    };

    VkDeviceCreateInfo device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &synchronization2_feature,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_info,
        .enabledExtensionCount = harmony_countof(DeviceExtensions),
        .ppEnabledExtensionNames = DeviceExtensions,
        .pEnabledFeatures = &features,
    };

    VkDevice device = NULL;
    VkResult result = vk->pfn.vkCreateDevice(vk->gpu, &device_info, NULL, &device);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_EXTENSION_NOT_PRESENT: harmony_error("Required Vulkan extension not present");
        case VK_ERROR_FEATURE_NOT_PRESENT: harmony_error("Required Vulkan feature not present");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INITIALIZATION_FAILED: harmony_error("Vulkan initialization failed");
        case VK_ERROR_TOO_MANY_OBJECTS: harmony_error("Vulkan too many objects");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        default: harmony_error("Unexpected Vulkan error");
    }
    return device;
}

void harmony_vk_destroy_device(const HarmonyVulkan *vk, VkDevice device) {
    harmony_assert(vk != NULL);
    harmony_assert(device != VK_NULL_HANDLE);
    vk->pfn.vkDestroyDevice(device, NULL);
}

void harmony_vk_device_wait(const HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);
    harmony_assert(vk->device != NULL);

    VkResult result = vk->pfn.vkDeviceWaitIdle(vk->device);
    switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }
}

VkSurfaceKHR harmony_vk_create_surface(
    const HarmonyVulkan *vk,
    const HarmonyPlatform *platform,
    const HarmonyWindow *window
) {
    harmony_assert(vk != NULL);
    harmony_assert(platform != NULL);
    harmony_assert(window != NULL);

    VkSurfaceKHR surface = VK_NULL_HANDLE;

#ifdef __linux__

    PFN_vkCreateXlibSurfaceKHR pfn_vkCreateXlibSurfaceKHR
        = (PFN_vkCreateXlibSurfaceKHR)vk->pfn.vkGetInstanceProcAddr(
            vk->instance, "vkCreateXlibSurfaceKHR");
    if (pfn_vkCreateXlibSurfaceKHR == NULL)
        harmony_error("Could not load vkCreateXlibSurfaceKHR");

    VkResult surface_result = pfn_vkCreateXlibSurfaceKHR(
        vk->instance,
        &(VkXlibSurfaceCreateInfoKHR){
            .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .dpy = platform->display,
            .window = window->platform_internals->window,
        },
        NULL,
        &surface
    );
    switch (surface_result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

#else

#error "unsupported platform, supported: linux"

#endif

    harmony_assert(surface != VK_NULL_HANDLE);
    return surface;
}

void harmony_vk_destroy_surface(const HarmonyVulkan *vk, VkSurfaceKHR surface) {
    harmony_assert(vk != NULL);
    harmony_assert(surface != NULL);
    vk->pfn.vkDestroySurfaceKHR(vk->instance, surface, NULL);
}

static VkPresentModeKHR harmony_vk_find_swapchain_present_mode(
    const HarmonyVulkan *vk,
    VkSurfaceKHR surface,
    VkPresentModeKHR desired_mode
) {
    harmony_assert(vk != NULL);
    harmony_assert(surface != VK_NULL_HANDLE);

    if (desired_mode == VK_PRESENT_MODE_FIFO_KHR)
        return desired_mode;

    u32 mode_count = 0;
    VkResult count_res = vk->pfn.vkGetPhysicalDeviceSurfacePresentModesKHR(
        vk->gpu, surface, &mode_count, NULL);
    switch (count_res) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan get present modes incomplete");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        default: harmony_error("Unexpected Vulkan error");
    }

#define HARMONY_MAX_PRESENT_MODES 8
    if (mode_count > HARMONY_MAX_PRESENT_MODES)
        harmony_error("too many present modes");
    VkPresentModeKHR present_modes[HARMONY_MAX_PRESENT_MODES];
#undef HARMONY_MAX_PRESENT_MODES

    VkResult present_res = vk->pfn.vkGetPhysicalDeviceSurfacePresentModesKHR(
        vk->gpu, surface, &mode_count, present_modes);
    switch (present_res) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan get present modes incomplete");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        default: harmony_error("Unexpected Vulkan error");
    }

    for (usize i = 0; i < mode_count; ++i) {
        if (present_modes[i] == desired_mode)
            return desired_mode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkFormat harmony_vk_find_swapchain_format(const HarmonyVulkan *vk, VkSurfaceKHR surface) {
    harmony_assert(vk != NULL);
    harmony_assert(surface != VK_NULL_HANDLE);

    u32 format_count = 0;
    VkResult formats_count_res = vk->pfn.vkGetPhysicalDeviceSurfaceFormatsKHR(
        vk->gpu,
        surface,
        &format_count,
        NULL
    );
    switch (formats_count_res) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan get swapchain formats incomplete");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        default: harmony_error("Unexpected Vulkan error");
    }
    if (format_count == 0)
        harmony_error("No swapchain formats available");

#define HARMONY_MAX_FORMATS 64
    if (format_count > 64)
        harmony_error("Too many Vulkan swapchain formats");
    VkSurfaceFormatKHR formats[HARMONY_MAX_FORMATS];
#undef HARMONY_MAX_FORMATS

    VkResult format_result = vk->pfn.vkGetPhysicalDeviceSurfaceFormatsKHR(
        vk->gpu,
        surface,
        &format_count,
        formats
    );
    switch (format_result) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan get swapchain formats incomplete");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        default: harmony_error("Unexpected Vulkan error");
    }

    for (usize i = 0; i < format_count; ++i) {
        if (formats[i].format == VK_FORMAT_R8G8B8A8_SRGB)
            return VK_FORMAT_R8G8B8A8_SRGB;
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB)
            return VK_FORMAT_B8G8R8A8_SRGB;
    }
    harmony_error("No supported swapchain formats");
}

VkSwapchainKHR harmony_vk_create_swapchain(
    const HarmonyVulkan *vk,
    u32 *width,
    u32 *height,
    VkFormat *format,
    VkSwapchainKHR old_swapchain,
    VkSurfaceKHR surface,
    VkImageUsageFlags image_usage,
    VkPresentModeKHR desired_mode
) {
    harmony_assert(vk != NULL);
    harmony_assert(width != NULL);
    harmony_assert(height != NULL);
    harmony_assert(format != NULL);
    harmony_assert(surface != VK_NULL_HANDLE);
    harmony_assert(image_usage != 0);

    VkPresentModeKHR present_mode = harmony_vk_find_swapchain_present_mode(vk, surface, desired_mode);

    *format = harmony_vk_find_swapchain_format(vk, surface);

    VkSurfaceCapabilitiesKHR surface_capabilities = {0};
    VkResult surface_result = vk->pfn.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        vk->gpu,
        surface,
        &surface_capabilities
    );
    switch (surface_result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        default: harmony_error("Unexpected Vulkan error");
    }
    if (surface_capabilities.currentExtent.width == 0 || surface_capabilities.currentExtent.height == 0)
        return VK_NULL_HANDLE;
    if (surface_capabilities.currentExtent.width < surface_capabilities.minImageExtent.width)
        return VK_NULL_HANDLE;
    if (surface_capabilities.currentExtent.height < surface_capabilities.minImageExtent.height)
        return VK_NULL_HANDLE;
    if (surface_capabilities.currentExtent.width > surface_capabilities.maxImageExtent.width)
        return VK_NULL_HANDLE;
    if (surface_capabilities.currentExtent.height > surface_capabilities.maxImageExtent.height)
        return VK_NULL_HANDLE;

    *width = surface_capabilities.currentExtent.width;
    *height = surface_capabilities.currentExtent.height;

    VkSwapchainCreateInfoKHR new_swapchain_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = surface_capabilities.minImageCount,
        .imageFormat = *format,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = surface_capabilities.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .preTransform = surface_capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = old_swapchain,
    };

    VkSwapchainKHR new_swapchain = NULL;
    VkResult swapchain_result = vk->pfn.vkCreateSwapchainKHR(
        vk->device,
        &new_swapchain_info,
        NULL,
        &new_swapchain
    );
    switch (swapchain_result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: harmony_error("Vulkan native window in use");
        case VK_ERROR_INITIALIZATION_FAILED: harmony_error("Vulkan initialization failed");
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: harmony_error("Vulkan compression exhausted");
        default: harmony_error("Unexpected Vulkan error");
    }

    return new_swapchain;
}

void harmony_vk_destroy_swapchain(const HarmonyVulkan *vk, VkSwapchainKHR swapchain) {
    harmony_assert(vk != NULL);
    harmony_assert(swapchain != NULL);
    vk->pfn.vkDestroySwapchainKHR(vk->device, swapchain, NULL);
}

u32 harmony_vk_get_swapchain_image_count(const HarmonyVulkan *vk, VkSwapchainKHR swapchain) {
    harmony_assert(vk != NULL);
    harmony_assert(swapchain != NULL);

    u32 image_count;
    VkResult result = vk->pfn.vkGetSwapchainImagesKHR(
        vk->device,
        swapchain,
        &image_count,
        NULL
    );
    switch (result) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan get swapchain images incomplete");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }

    return image_count;
}

void harmony_vk_get_swapchain_images(const HarmonyVulkan *vk, VkSwapchainKHR swapchain, VkImage *images, u32 count) {
    harmony_assert(vk != NULL);
    harmony_assert(swapchain != NULL);
    harmony_assert(images != NULL);
    harmony_assert(count == harmony_vk_get_swapchain_image_count(vk, swapchain));

    VkResult image_result = vk->pfn.vkGetSwapchainImagesKHR(
        vk->device,
        swapchain,
        &count,
        images
    );
    switch (image_result) {
        case VK_SUCCESS: break;
        case VK_INCOMPLETE: {
            harmony_log_warning("Vulkan get swapchain images incomplete");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }
}

u32 harmony_vk_acquire_next_image(
    const HarmonyVulkan *vk,
    VkSwapchainKHR swapchain,
    VkSemaphore signal_semaphore,
    VkFence signal_fence
) {
    harmony_assert(vk != NULL);
    harmony_assert(swapchain != VK_NULL_HANDLE);

    u32 next_image;
    const VkResult acquire_result = vk->pfn.vkAcquireNextImageKHR(
        vk->device,
        swapchain,
        UINT64_MAX,
        signal_semaphore,
        signal_fence,
        &next_image
    );
    switch (acquire_result) {
        case VK_SUCCESS: break;
        case VK_SUBOPTIMAL_KHR:
            harmony_log_warning("Suboptimal KHR");
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            harmony_log_warning("Out of date KHR");
            break;
        case VK_TIMEOUT: harmony_error("Vulkan timed out waiting for image");
        case VK_NOT_READY: harmony_error("Vulkan not ready waiting for image");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        default: harmony_error("Unexpected Vulkan error");
    }

    return next_image;
}

void harmony_vk_present(
    const HarmonyVulkan *vk,
    VkQueue queue,
    VkSwapchainKHR swapchain,
    u32 image_index,
    VkSemaphore *wait_semaphores,
    u32 semaphore_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(queue != VK_NULL_HANDLE);
    harmony_assert(swapchain != VK_NULL_HANDLE);
    if (semaphore_count > 0)
        harmony_assert(wait_semaphores != NULL);

    VkPresentInfoKHR present_info = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = semaphore_count,
        .pWaitSemaphores = wait_semaphores,
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &image_index,
    };
    const VkResult present_result = vk->pfn.vkQueuePresentKHR(queue, &present_info);
    switch (present_result) {
        case VK_SUCCESS: break;
        case VK_SUBOPTIMAL_KHR: {
            harmony_log_warning("Suboptimal KHR");
            break;
        }
        case VK_ERROR_OUT_OF_DATE_KHR: {
            harmony_log_warning("Out of date KHR");
            break;
        }
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        default: harmony_error("Unexpected Vulkan error");
    }
}

VkSemaphore harmony_vk_create_semaphore(const HarmonyVulkan *vk, VkSemaphoreCreateFlags flags) {
    harmony_assert(vk != NULL);

    VkSemaphoreCreateInfo semaphore_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .flags = flags,
    };
    VkSemaphore semaphore = VK_NULL_HANDLE;
    const VkResult result = vk->pfn.vkCreateSemaphore(
        vk->device,
        &semaphore_info,
        NULL,
        &semaphore
    );
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }

    return semaphore;
}

void harmony_vk_destroy_semaphore(const HarmonyVulkan *vk, VkSemaphore semaphore) {
    harmony_assert(vk != NULL);
    harmony_assert(semaphore != NULL);
    vk->pfn.vkDestroySemaphore(vk->device, semaphore, NULL);
}

VkFence harmony_vk_create_fence(const HarmonyVulkan *vk, VkFenceCreateFlags flags) {
    harmony_assert(vk != NULL);

    VkFenceCreateInfo fence_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = flags,
    };
    VkFence fence = VK_NULL_HANDLE;
    const VkResult result = vk->pfn.vkCreateFence(
        vk->device,
        &fence_info,
        NULL,
        &fence
    );
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }

    return fence;
}

void harmony_vk_destroy_fence(const HarmonyVulkan *vk, VkFence fence) {
    harmony_assert(vk != NULL);
    harmony_assert(fence != NULL);
    vk->pfn.vkDestroyFence(vk->device, fence, NULL);
}

void harmony_vk_wait_for_fences(const HarmonyVulkan *vk, VkFence *fences, u32 count) {
    harmony_assert(vk != NULL);
    harmony_assert(fences != NULL);
    harmony_assert(count > 0);

    VkResult result = vk->pfn.vkWaitForFences(vk->device, count, fences, VK_TRUE, UINT64_MAX);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_TIMEOUT: harmony_error("Vulkan timed out waiting for fence");
        case VK_NOT_READY: harmony_error("Vulkan not ready");
        case VK_SUBOPTIMAL_KHR: harmony_error("Vulkan suboptimal");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        case VK_ERROR_SURFACE_LOST_KHR: harmony_error("Vulkan surface lost");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_reset_fences(const HarmonyVulkan *vk, VkFence *fences, u32 count) {
    harmony_assert(vk != NULL);
    harmony_assert(fences != NULL);
    harmony_assert(count > 0);

    VkResult result = vk->pfn.vkResetFences(vk->device, count, fences);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan error unknown");
        default: harmony_error("Unexpected Vulkan error");
    }
}

bool harmony_vk_find_queue_family(const HarmonyVulkan *vk, u32 *queue_family, VkQueueFlags queue_flags) {
    *queue_family = UINT32_MAX;

    u32 queue_family_count = 0;
    vk->pfn.vkGetPhysicalDeviceQueueFamilyProperties(vk->gpu, &queue_family_count, NULL);

#define HARMONY_MAX_QUEUE_FAMILIES 32
    if (queue_family_count > HARMONY_MAX_QUEUE_FAMILIES)
        harmony_error("Too many queue families");
    VkQueueFamilyProperties queue_families[HARMONY_MAX_QUEUE_FAMILIES];
#undef HARMONY_MAX_QUEUE_FAMILIES

    vk->pfn.vkGetPhysicalDeviceQueueFamilyProperties(vk->gpu, &queue_family_count, queue_families);

    for (u32 i = 0; i < queue_family_count; ++i) {
        if (queue_families[i].queueFlags & (queue_flags)) {
            *queue_family = i;
            return true;
        }
    }
    return false;
}

VkQueue harmony_vk_get_queue(const HarmonyVulkan *vk) {
    harmony_assert(vk != NULL);
    harmony_assert(vk->queue_family != UINT32_MAX);

    VkQueue queue = VK_NULL_HANDLE;
    vk->pfn.vkGetDeviceQueue(vk->device, vk->queue_family, 0, &queue);
    if (queue == VK_NULL_HANDLE)
        harmony_error("Vulkan Device queue does not exist");

    return queue;
}

void harmony_vk_queue_wait(const HarmonyVulkan *vk, VkQueue queue) {
    harmony_assert(vk != NULL);
    harmony_assert(queue != NULL);

    VkResult wait_result = vk->pfn.vkQueueWaitIdle(queue);
    switch (wait_result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_DEVICE_LOST: harmony_error("Vulkan device lost");
        default: harmony_error("Unexpected Vulkan error");
    }
}

VkCommandPool harmony_vk_create_command_pool(const HarmonyVulkan *vk, VkCommandPoolCreateFlags flags) {
    harmony_assert(vk != NULL);

    VkCommandPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = flags,
        .queueFamilyIndex = vk->queue_family,
    };
    VkCommandPool pool = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateCommandPool(vk->device, &info, NULL, &pool);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INITIALIZATION_FAILED: harmony_error("Vulkan failed to initialize");
        default: harmony_error("Vulkan failed to create command pool");
    }

    return pool;
}

void harmony_vk_destroy_command_pool(const HarmonyVulkan *vk, VkCommandPool pool) {
    harmony_assert(vk != NULL);
    harmony_assert(pool != VK_NULL_HANDLE);
    vk->pfn.vkDestroyCommandPool(vk->device, pool, NULL);
}

void harmony_vk_allocate_command_buffers(
    const HarmonyVulkan *vk,
    VkCommandPool pool,
    VkCommandBuffer *cmds,
    u32 count
) {
    harmony_assert(vk != NULL);
    harmony_assert(pool != VK_NULL_HANDLE);
    harmony_assert(cmds != NULL);
    harmony_assert(count != 0);

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = pool,
        .commandBufferCount = count,
    };
    const VkResult result = vk->pfn.vkAllocateCommandBuffers(
        vk->device,
        &alloc_info,
        cmds
    );
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_free_command_buffers(
    const HarmonyVulkan *vk,
    VkCommandPool pool,
    VkCommandBuffer *cmds,
    u32 count
) {
    harmony_assert(vk != NULL);
    harmony_assert(pool != VK_NULL_HANDLE);
    harmony_assert(cmds != NULL);
    harmony_assert(count != 0);
    vk->pfn.vkFreeCommandBuffers(vk->device, pool, count, cmds);
}

VkDescriptorPool harmony_vk_create_descriptor_pool(
    const HarmonyVulkan *vk,
    u32 max_sets,
    VkDescriptorPoolSize *sizes,
    u32 count
) {
    harmony_assert(vk != NULL);
    harmony_assert(max_sets > 0);
    harmony_assert(sizes != NULL);
    harmony_assert(count > 0);

    VkDescriptorPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .maxSets = max_sets,
        .poolSizeCount = count,
        .pPoolSizes = sizes
    };
    VkDescriptorPool pool;
    VkResult result = vk->pfn.vkCreateDescriptorPool(vk->device, &pool_info, NULL, &pool);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_FRAGMENTATION_EXT: harmony_error("Vulkan fragmentation error");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Vulkan failed to create descriptor pool");
    }
    return pool;
}

void harmony_vk_destroy_descriptor_pool(const HarmonyVulkan *vk, VkDescriptorPool pool) {
    harmony_assert(vk != NULL);
    harmony_assert(pool != VK_NULL_HANDLE);
    vk->pfn.vkDestroyDescriptorPool(vk->device, pool, NULL);
}

void harmony_vk_reset_descriptor_pool(const HarmonyVulkan *vk, VkDescriptorPool pool) {
    harmony_assert(vk != NULL);
    harmony_assert(pool != VK_NULL_HANDLE);
    vk->pfn.vkResetDescriptorPool(vk->device, pool, 0);
}

bool harmony_vk_allocate_descriptor_sets(
    const HarmonyVulkan *vk,
    VkDescriptorPool pool,
    VkDescriptorSetLayout *layouts,
    VkDescriptorSet *sets,
    u32 count
) {
    harmony_assert(vk != NULL);
    harmony_assert(pool != VK_NULL_HANDLE);
    harmony_assert(layouts != NULL);
    harmony_assert(sets != NULL);
    harmony_assert(count > 0);

    VkDescriptorSetAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = pool,
        .descriptorSetCount = count,
        .pSetLayouts = layouts,
    };
    VkResult result = vk->pfn.vkAllocateDescriptorSets(vk->device, &alloc_info, sets);
    switch (result) {
        case VK_SUCCESS: return true;
        case VK_ERROR_FRAGMENTED_POOL:
            harmony_log_warning("Vulkan descriptor pool was fragmented");
            return false;
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            harmony_log_warning("Vulkan ran out of descriptor pool memory");
            return false;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }
}

VkDescriptorSetLayout harmony_vk_create_descriptor_set_layout(
    const HarmonyVulkan *vk,
    const VkDescriptorSetLayoutBinding *bindings,
    u32 count
) {
    harmony_assert(vk != NULL);
    harmony_assert(bindings != NULL);
    harmony_assert(count > 0);

    VkDescriptorSetLayoutCreateInfo layout_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = count,
        .pBindings = bindings,
    };
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    const VkResult result = vk->pfn.vkCreateDescriptorSetLayout(vk->device, &layout_info, NULL, &layout);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return layout;
}

void harmony_vk_destroy_descriptor_set_layout(const HarmonyVulkan *vk, VkDescriptorSetLayout layout) {
    harmony_assert(vk != NULL);
    harmony_assert(layout != VK_NULL_HANDLE);
    vk->pfn.vkDestroyDescriptorSetLayout(vk->device, layout, NULL);
}

VkPipelineLayout harmony_vk_create_pipeline_layout(
    const HarmonyVulkan *vk,
    const VkDescriptorSetLayout* layouts,
    u32 layout_count,
    const VkPushConstantRange* push_constants,
    u32 push_constant_count
) {
    harmony_assert(vk != NULL);
    if (layout_count > 0)
        harmony_assert(layouts != NULL);
    if (push_constant_count > 0)
        harmony_assert(push_constants != NULL);

    VkPipelineLayoutCreateInfo pipeline_layout_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = layout_count,
        .pSetLayouts = layouts,
        .pushConstantRangeCount = push_constant_count,
        .pPushConstantRanges = push_constants,
    };
    VkPipelineLayout layout = VK_NULL_HANDLE;
    const VkResult result = vk->pfn.vkCreatePipelineLayout(vk->device, &pipeline_layout_info, NULL, &layout);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return layout;
}

void harmony_vk_destroy_pipeline_layout(const HarmonyVulkan *vk, VkPipelineLayout layout) {
    harmony_assert(vk != NULL);
    harmony_assert(layout != VK_NULL_HANDLE);
    vk->pfn.vkDestroyPipelineLayout(vk->device, layout, NULL);
}

VkShaderModule harmony_vk_create_shader_module(const HarmonyVulkan *vk, const u8 *code, usize size) {
    harmony_assert(vk != NULL);
    harmony_assert(code != NULL);
    harmony_assert(size > 0);

    VkShaderModuleCreateInfo shader_module_info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = size,
        .pCode = (const u32*)code,
    };
    VkShaderModule shader_module = VK_NULL_HANDLE;
    const VkResult result = vk->pfn.vkCreateShaderModule(vk->device, &shader_module_info, NULL, &shader_module);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_INVALID_SHADER_NV: harmony_error("Vulkan invalid shader");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return shader_module;
}

void harmony_vk_destroy_shader_module(const HarmonyVulkan *vk, VkShaderModule shader_module) {
    harmony_assert(vk != NULL);
    harmony_assert(shader_module != VK_NULL_HANDLE);
    vk->pfn.vkDestroyShaderModule(vk->device, shader_module, NULL);
}

VkPipeline harmony_vk_create_graphics_pipeline(const HarmonyVulkan *vk, const HarmonyVkPipelineConfig *config) {
    harmony_assert(vk != NULL);
    harmony_assert(config != NULL);
    harmony_assert(config->layout != NULL);
    harmony_assert(config->shaders != NULL);
    harmony_assert(config->shader_stages != NULL);
    harmony_assert(config->shader_count > 0);
    if (config->vertex_binding_count > 0)
        harmony_assert(config->vertex_bindings != NULL);
    if (config->color_attachment_count > 0)
        harmony_assert(config->color_attachment_formats != NULL);

#define HARMONY_MAX_SHADER_STAGES 8
    if (config->shader_count > HARMONY_MAX_SHADER_STAGES)
        harmony_error("Too many shader stages for graphics pipeline");
    VkPipelineShaderStageCreateInfo shader_stages[HARMONY_MAX_SHADER_STAGES];
#undef HARMONY_MAX_SHADER_STAGES

    for (usize i = 0; i < config->shader_count; ++i) {
        shader_stages[i] = (VkPipelineShaderStageCreateInfo){
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = config->shader_stages[i],
            .module = config->shaders[i],
            .pName = "main",
        };
    }

    VkPipelineVertexInputStateCreateInfo vertex_input_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = config->vertex_binding_count,
        .pVertexBindingDescriptions = config->vertex_bindings,
        .vertexAttributeDescriptionCount = config->vertex_attribute_count,
        .pVertexAttributeDescriptions = config->vertex_attributes,
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = config->topology,
        .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineTessellationStateCreateInfo tessellation_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    };

    VkPipelineViewportStateCreateInfo viewport_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1,
    };

    VkPipelineRasterizationStateCreateInfo rasterization_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = config->cull_mode,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    VkPipelineMultisampleStateCreateInfo multisample_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .minSampleShading = 1.0f,
        .pSampleMask = NULL,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };

    VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = config->depth_attachment_format != VK_FORMAT_UNDEFINED
            ? VK_TRUE
            : VK_FALSE,
        .depthWriteEnable = config->depth_attachment_format != VK_FORMAT_UNDEFINED
            ? VK_TRUE
            : VK_FALSE,
        .depthCompareOp = config->enable_color_blend
            ? VK_COMPARE_OP_LESS_OR_EQUAL
            : VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        .minDepthBounds = 0.0f,
        .maxDepthBounds = 1.0f,
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment = {
        .blendEnable = config->enable_color_blend ? VK_TRUE : VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                        | VK_COLOR_COMPONENT_G_BIT
                        | VK_COLOR_COMPONENT_B_BIT
                        | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo color_blend_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &color_blend_attachment,
        .blendConstants = {1.0f, 1.0f, 1.0f, 1.0f},
    };

    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };
    VkPipelineDynamicStateCreateInfo dynamic_state = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pDynamicStates = dynamic_states,
        .dynamicStateCount = harmony_countof(dynamic_states),
    };

    VkPipelineRenderingCreateInfo rendering_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = config->color_attachment_count,
        .pColorAttachmentFormats = config->color_attachment_formats,
        .depthAttachmentFormat = config->depth_attachment_format,
        .stencilAttachmentFormat = config->stencil_attachment_format,
    };

    VkGraphicsPipelineCreateInfo pipeline_info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &rendering_info,
        .stageCount = harmony_countof(shader_stages),
        .pStages = shader_stages,
        .pVertexInputState = &vertex_input_state,
        .pInputAssemblyState = &input_assembly_state,
        .pTessellationState = &tessellation_state,
        .pViewportState = &viewport_state,
        .pRasterizationState = &rasterization_state,
        .pMultisampleState = &multisample_state,
        .pDepthStencilState = &depth_stencil_state,
        .pColorBlendState = &color_blend_state,
        .pDynamicState = &dynamic_state,
        .layout = config->layout,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1,
    };

    VkPipeline pipeline = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateGraphicsPipelines(vk->device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &pipeline);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_PIPELINE_COMPILE_REQUIRED_EXT: {
            harmony_log_warning("Pipeline requires recompilation");
        } break;
        case VK_ERROR_INVALID_SHADER_NV: harmony_error("Vulkan invalid shader");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return pipeline;
}

VkPipeline harmony_vk_create_compute_pipeline(const HarmonyVulkan *vk, const HarmonyVkPipelineConfig *config) {
    harmony_assert(vk != NULL);
    harmony_assert(config != NULL);
    harmony_assert(config->layout != NULL);
    harmony_assert(config->shaders != NULL);
    harmony_assert(config->shader_stages != NULL);
    harmony_assert(config->shader_stages[0] == VK_SHADER_STAGE_COMPUTE_BIT);
    harmony_assert(config->shader_count == 1);
    if (config->vertex_binding_count > 0)
        harmony_assert(config->vertex_bindings != NULL);
    harmony_assert(config->color_attachment_count == 0)
    harmony_assert(config->color_attachment_formats == NULL);
    harmony_assert(config->depth_attachment_format == VK_FORMAT_UNDEFINED);
    harmony_assert(config->stencil_attachment_format == VK_FORMAT_UNDEFINED);

    VkPipelineShaderStageCreateInfo shader_stage = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_COMPUTE_BIT,
        .module = config->shaders[0],
        .pName = "main",
    };

    VkComputePipelineCreateInfo pipeline_info = {
        .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .stage = shader_stage,
        .layout = config->layout,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1,
    };

    VkPipeline pipeline = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateComputePipelines(vk->device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &pipeline);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_PIPELINE_COMPILE_REQUIRED_EXT: {
            harmony_log_warning("Pipeline requires recompilation");
        } break;
        case VK_ERROR_INVALID_SHADER_NV: harmony_error("Vulkan invalid shader");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return pipeline;
}

void harmony_vk_destroy_pipeline(const HarmonyVulkan *vk, VkPipeline pipeline) {
    harmony_assert(vk != NULL);
    harmony_assert(pipeline != VK_NULL_HANDLE);
    vk->pfn.vkDestroyPipeline(vk->device, pipeline, NULL);
}

VkBuffer harmony_vk_create_buffer(const HarmonyVulkan *vk, usize size, VkBufferUsageFlags usage) {
    harmony_assert(vk != NULL);
    harmony_assert(size > 0);
    harmony_assert(usage != 0);

    VkBufferCreateInfo buffer_info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
    };
    VkBuffer buffer = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateBuffer(vk->device, &buffer_info, NULL, &buffer);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: harmony_error("Vulkan invalid external handle");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: harmony_error("Vulkan invalid opaque capture address");
        default: harmony_error("Unexpected Vulkan error");
    }

    return buffer;
}

void harmony_vk_destroy_buffer(const HarmonyVulkan *vk, VkBuffer buffer) {
    harmony_assert(vk != NULL);
    harmony_assert(buffer != VK_NULL_HANDLE);
    vk->pfn.vkDestroyBuffer(vk->device, buffer, NULL);
}

VkImage harmony_vk_create_image(const HarmonyVulkan *vk, const HarmonyVkImageConfig *config) {
    harmony_assert(vk != NULL);
    harmony_assert(config->format != VK_FORMAT_UNDEFINED);
    harmony_assert(config->usage != 0);
    u32 width = harmony_max(config->width, 1);
    u32 height = harmony_max(config->height, 1);
    u32 depth = harmony_max(config->depth, 1);
    u32 mip_levels = harmony_max(config->mip_levels, 1);
    if (mip_levels == UINT32_MAX) {
        u32 max_length = harmony_max(harmony_max(
            width,
            height),
            depth
        );
        mip_levels = (u32)log2f((f32)max_length) + 1;
    }
    u32 array_layers = harmony_max(config->array_layers, 1);

    VkImageType type;
    switch (config->dimensions) {
        case 1:
            type = VK_IMAGE_TYPE_1D;
            break;
        case 3:
            type = VK_IMAGE_TYPE_3D;
            break;
        default:
            type = VK_IMAGE_TYPE_2D;
            break;
    }

    VkSampleCountFlagBits multisamples;
    switch (config->multisample_count) {
        case 2:
            multisamples = VK_SAMPLE_COUNT_2_BIT;
            break;
        case 4:
            multisamples = VK_SAMPLE_COUNT_4_BIT;
            break;
        case 8:
            multisamples = VK_SAMPLE_COUNT_8_BIT;
            break;
        case 16:
            multisamples = VK_SAMPLE_COUNT_16_BIT;
            break;
        case 32:
            multisamples = VK_SAMPLE_COUNT_32_BIT;
            break;
        case 64:
            multisamples = VK_SAMPLE_COUNT_64_BIT;
            break;
        default:
            multisamples = VK_SAMPLE_COUNT_1_BIT;
            break;
    }

    if (config->flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) {
        harmony_assert(width == height);
        harmony_assert(depth == 1);
        harmony_assert(type == VK_IMAGE_TYPE_2D);
        harmony_assert(mip_levels == 1);
        if (array_layers == 1)
            array_layers = 6;
        harmony_assert(array_layers == 6);
    }

    VkImageCreateInfo image_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .flags = config->flags,
        .imageType = type,
        .format = config->format,
        .extent = (VkExtent3D){width, height, depth},
        .mipLevels = mip_levels,
        .arrayLayers = array_layers,
        .samples = multisamples,
        .usage = config->usage,
    };

    VkImage image = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateImage(vk->device, &image_info, NULL, &image);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: harmony_error("Vulkan compression exhausted");
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: harmony_error("Vulkan invalid external handle");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: harmony_error("Vulkan invalid opaque capture address");
        default: harmony_error("Unexpected Vulkan error");
    }

    return image;
}

void harmony_vk_destroy_image(const HarmonyVulkan *vk, VkImage image) {
    harmony_assert(vk != NULL);
    harmony_assert(image != NULL);
    vk->pfn.vkDestroyImage(vk->device, image, NULL);
}

VkImageView harmony_vk_create_image_view(
    const HarmonyVulkan *vk,
    VkImage image,
    HarmonyVkImageViewConfig *config
) {
    harmony_assert(vk != NULL);
    harmony_assert(image != VK_NULL_HANDLE);
    harmony_assert(config != NULL);
    harmony_assert(config->format != VK_FORMAT_UNDEFINED);
    harmony_assert(config->aspect != 0);

    VkImageViewCreateInfo view_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = config->view_type,
        .format = config->format,
        .subresourceRange = (VkImageSubresourceRange){
            .aspectMask = config->aspect,
            .baseMipLevel = config->base_mip,
            .levelCount = harmony_max(config->mip_count, 1),
            .baseArrayLayer = config->base_layer,
            .layerCount = harmony_max(config->layer_count, 1),
        },
    };

    VkImageView view = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkCreateImageView(vk->device, &view_info, NULL, &view);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: harmony_error("Vulkan invalid opaque capture address");
        default: harmony_error("Unexpected Vulkan error");
    }

    return view;
}

void harmony_vk_destroy_image_view(const HarmonyVulkan *vk, VkImageView view) {
    harmony_assert(vk != NULL);
    harmony_assert(view != VK_NULL_HANDLE);
    vk->pfn.vkDestroyImageView(vk->device, view, NULL);
}

VkSampler harmony_vk_create_sampler(const HarmonyVulkan *vk, VkFilter filter, VkSamplerAddressMode edge_mode) {
    VkPhysicalDeviceProperties gpu_properties = {0};
    vk->pfn.vkGetPhysicalDeviceProperties(vk->gpu, &gpu_properties);

    VkSamplerCreateInfo sampler_info = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = filter,
        .minFilter = filter,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = edge_mode,
        .addressModeV = edge_mode,
        .addressModeW = edge_mode,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = gpu_properties.limits.maxSamplerAnisotropy,
        .maxLod = 1000.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    };

    VkSampler sampler = VK_NULL_HANDLE;
    VkResult sampler_result = vk->pfn.vkCreateSampler(vk->device, &sampler_info, NULL, &sampler);
    switch (sampler_result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: harmony_error("Vulkan invalid device address");
        default: harmony_error("Unexpected Vulkan error");
    }

    return sampler;
}

void harmony_vk_destroy_sampler(const HarmonyVulkan *vk, VkSampler sampler) {
    harmony_assert(vk != NULL);
    harmony_assert(sampler != VK_NULL_HANDLE);
    vk->pfn.vkDestroySampler(vk->device, sampler, NULL);
}

VkMemoryRequirements harmony_vk_get_buffer_mem_reqs(const HarmonyVulkan *vk, VkBuffer buffer) {
    harmony_assert(vk != NULL);
    harmony_assert(buffer != VK_NULL_HANDLE);
    VkMemoryRequirements reqs;
    vk->pfn.vkGetBufferMemoryRequirements(vk->device, buffer, &reqs);
    return reqs;
}

VkMemoryRequirements harmony_vk_get_image_mem_reqs(const HarmonyVulkan *vk, VkImage image) {
    harmony_assert(vk != NULL);
    harmony_assert(image != VK_NULL_HANDLE);
    VkMemoryRequirements reqs;
    vk->pfn.vkGetImageMemoryRequirements(vk->device, image, &reqs);
    return reqs;
}

static u32 harmony_vk_find_memory_type_index(
    const HarmonyVulkan *vk,
    u32 bitmask,
    VkMemoryPropertyFlags desired_flags,
    VkMemoryPropertyFlags undesired_flags
) {
    harmony_assert(vk != NULL);
    harmony_assert(bitmask != 0);

    VkPhysicalDeviceMemoryProperties mem_props;
    vk->pfn.vkGetPhysicalDeviceMemoryProperties(vk->gpu, &mem_props);

    for (uint32_t i = 0; i < mem_props.memoryTypeCount; ++i) {
        if ((bitmask & (1 << i))) {
            if ((mem_props.memoryTypes[i].propertyFlags & undesired_flags) != 0)
                continue;
            if ((mem_props.memoryTypes[i].propertyFlags & desired_flags) == 0)
                continue;
            return i;
        }
    }
    for (uint32_t i = 0; i < mem_props.memoryTypeCount; ++i) {
        if ((bitmask & (1 << i))) {
            if ((mem_props.memoryTypes[i].propertyFlags & desired_flags) == 0)
                continue;
            harmony_log_warning("Could not find Vulkan memory type without undesired flags");
            return i;
        }
    }
    for (uint32_t i = 0; i < mem_props.memoryTypeCount; ++i) {
        if ((bitmask & (1 << i))) {
            harmony_log_warning("Could not find Vulkan memory type with desired flags");
            return i;
        }
    }
    harmony_error("Could not find Vulkan memory type");
}

VkDeviceMemory harmony_vk_allocate_memory(
    const HarmonyVulkan *vk,
    VkMemoryRequirements *mem_reqs,
    VkMemoryPropertyFlags desired_flags,
    VkMemoryPropertyFlags undesired_flags
) {
    harmony_assert(vk != NULL);
    harmony_assert(mem_reqs != NULL);

    VkMemoryAllocateInfo alloc_info = {
        .allocationSize = mem_reqs->size,
        .memoryTypeIndex = harmony_vk_find_memory_type_index(
            vk, mem_reqs->memoryTypeBits, desired_flags, undesired_flags),
    };
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkResult result = vk->pfn.vkAllocateMemory(vk->device, &alloc_info, NULL, &memory);
    switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: harmony_error("Vulkan invalid external handle");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: harmony_error("Vulkan invalid opaque capture address");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return memory;
}

void harmony_vk_free_memory(const HarmonyVulkan *vk, VkDeviceMemory memory) {
    harmony_assert(vk != NULL);
    harmony_assert(memory != VK_NULL_HANDLE);
    vk->pfn.vkFreeMemory(vk->device, memory, NULL);
}

void harmony_vk_bind_buffer_memory(const HarmonyVulkan *vk, VkBuffer buffer, VkDeviceMemory memory, usize offset) {
    harmony_assert(vk != NULL);
    harmony_assert(buffer != VK_NULL_HANDLE);
    harmony_assert(memory != VK_NULL_HANDLE);

    VkResult result = vk->pfn.vkBindBufferMemory(vk->device, buffer, memory, offset);
    switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: harmony_error("Vulkan invalid opaque capture address");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_bind_image_memory(const HarmonyVulkan *vk, VkImage image, VkDeviceMemory memory, usize offset) {
    harmony_assert(vk != NULL);
    harmony_assert(image != VK_NULL_HANDLE);
    harmony_assert(memory != VK_NULL_HANDLE);

    VkResult result = vk->pfn.vkBindImageMemory(vk->device, image, memory, offset);
    switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: harmony_error("Vulkan invalid opaque capture address");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void *harmony_vk_map_memory(const HarmonyVulkan *vk, VkDeviceMemory memory, usize offset, usize size) {
    harmony_assert(vk != NULL);
    harmony_assert(memory != VK_NULL_HANDLE);
    harmony_assert(size > 0);

    void *data;
    VkResult result = vk->pfn.vkMapMemory(vk->device, memory, offset, size, 0, &data);
    switch (result) {
        case VK_ERROR_MEMORY_MAP_FAILED: harmony_error("Vulkan memory map failed");
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }

    return data;
}

void harmony_vk_unmap_memory(const HarmonyVulkan *vk, VkDeviceMemory memory) {
    harmony_assert(vk != NULL);
    harmony_assert(memory != VK_NULL_HANDLE);
    vk->pfn.vkUnmapMemory(vk->device, memory);
}

void harmony_vk_flush_memory(const HarmonyVulkan *vk, VkDeviceMemory memory, usize offset, usize size) {
    harmony_assert(vk != NULL);
    harmony_assert(memory != VK_NULL_HANDLE);
    harmony_assert(size > 0);

    VkMappedMemoryRange range = {
        .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        .memory = memory,
        .offset = offset,
        .size = size,
    };

    VkResult result = vk->pfn.vkFlushMappedMemoryRanges(vk->device, 1, &range);
    switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_invalidate_memory(const HarmonyVulkan *vk, VkDeviceMemory memory, usize offset, usize size) {
    harmony_assert(vk != NULL);
    harmony_assert(memory != VK_NULL_HANDLE);
    harmony_assert(size > 0);

    VkMappedMemoryRange range = {
        .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        .memory = memory,
        .offset = offset,
        .size = size,
    };

    VkResult result = vk->pfn.vkInvalidateMappedMemoryRanges(vk->device, 1, &range);
    switch (result) {
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_VALIDATION_FAILED: harmony_error("Vulkan validation failed");
        case VK_ERROR_UNKNOWN: harmony_error("Vulkan unknown error");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_begin_cmd(const HarmonyVulkan *vk, VkCommandBuffer cmd, VkCommandBufferUsageFlags flags) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);

    const VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = flags,
    };
    VkResult result = vk->pfn.vkBeginCommandBuffer(cmd, &begin_info);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_end_cmd(const HarmonyVulkan *vk, VkCommandBuffer cmd) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    VkResult result = vk->pfn.vkEndCommandBuffer(cmd);
    switch (result) {
        case VK_SUCCESS: break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: harmony_error("Vulkan ran out of host memory");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: harmony_error("Vulkan ran out of device memory");
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: harmony_error("Vulkan invalid video parameters");
        default: harmony_error("Unexpected Vulkan error");
    }
}

void harmony_vk_copy_buffer(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkBuffer dst,
    VkBuffer src,
    const VkBufferCopy *regions,
    u32 region_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(dst != NULL);
    harmony_assert(src != NULL);
    harmony_assert(regions != NULL);
    harmony_assert(region_count > 0);
    vk->pfn.vkCmdCopyBuffer(cmd, src, dst, region_count, regions);
}

void harmony_vk_copy_image(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkImage dst,
    VkImage src,
    const VkImageCopy *regions,
    u32 region_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(dst != NULL);
    harmony_assert(src != NULL);
    harmony_assert(regions != NULL);
    harmony_assert(region_count > 0);
    vk->pfn.vkCmdCopyImage(
        cmd,
        src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        region_count,
        regions);
}

void harmony_vk_blit_image(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkImage dst,
    VkImage src,
    const VkImageBlit *regions,
    u32 region_count,
    VkFilter filter
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(dst != NULL);
    harmony_assert(src != NULL);
    harmony_assert(regions != NULL);
    harmony_assert(region_count > 0);
    vk->pfn.vkCmdBlitImage(
        cmd,
        src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        region_count,
        regions,
        filter);
}

void harmony_vk_copy_buffer_to_image(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkImage dst,
    VkBuffer src,
    VkBufferImageCopy *regions,
    u32 region_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(dst != NULL);
    harmony_assert(src != NULL);
    harmony_assert(regions != NULL);
    harmony_assert(region_count > 0);
    vk->pfn.vkCmdCopyBufferToImage(
        cmd,
        src,
        dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        region_count,
        regions);
}

void harmony_vk_copy_image_to_buffer(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkBuffer dst,
    VkImage src,
    VkBufferImageCopy *regions,
    u32 region_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(dst != NULL);
    harmony_assert(src != NULL);
    harmony_assert(regions != NULL);
    harmony_assert(region_count > 0);
    vk->pfn.vkCmdCopyImageToBuffer(
        cmd,
        src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dst,
        region_count,
        regions);
}

void harmony_vk_pipeline_barrier(const HarmonyVulkan *vk, VkCommandBuffer cmd, const VkDependencyInfo *dependencies) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(dependencies != NULL);
    vk->pfn.vkCmdPipelineBarrier2(cmd, dependencies);
}

void harmony_vk_begin_rendering(const HarmonyVulkan *vk, VkCommandBuffer cmd, const VkRenderingInfo *info) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(info != NULL);
    vk->pfn.vkCmdBeginRendering(cmd, info);
}

void harmony_vk_end_rendering(const HarmonyVulkan *vk, VkCommandBuffer cmd) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    vk->pfn.vkCmdEndRendering(cmd);
}

void harmony_vk_set_viewport(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    float x,
    float y,
    float width,
    float height,
    float near,
    float far
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    VkViewport viewport = {x, y, width, height, near, far};
    vk->pfn.vkCmdSetViewport(cmd, 0, 1, &viewport);
}

void harmony_vk_set_scissor(const HarmonyVulkan *vk, VkCommandBuffer cmd, i32 x, i32 y, u32 width, u32 height) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    VkRect2D scissor = {{x, y}, {width, height}};
    vk->pfn.vkCmdSetScissor(cmd, 0, 1, &scissor);
}

void harmony_vk_bind_pipeline(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkPipeline pipeline,
    VkPipelineBindPoint bind_point
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(pipeline != VK_NULL_HANDLE);
    vk->pfn.vkCmdBindPipeline(cmd, bind_point, pipeline);
}

void harmony_vk_bind_descriptor_sets(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkPipelineLayout layout,
    VkPipelineBindPoint bind_point,
    u32 begin_index,
    u32 set_count,
    const VkDescriptorSet *descriptor_sets
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(layout != VK_NULL_HANDLE);
    vk->pfn.vkCmdBindDescriptorSets(cmd, bind_point, layout, begin_index, set_count, descriptor_sets, 0, NULL);
}

void harmony_vk_push_constants(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkPipelineLayout layout,
    VkShaderStageFlags stages,
    u32 offset,
    u32 size,
    const void *data
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(layout != VK_NULL_HANDLE);
    harmony_assert(stages != 0);
    harmony_assert(data != NULL);
    harmony_assert(size > 0);
    vk->pfn.vkCmdPushConstants(cmd, layout, stages, offset, size, data);
}

void harmony_vk_bind_vertex_buffers(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    u32 begin_index,
    u32 count,
    VkBuffer *vertex_buffers,
    usize *offsets

) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(count > 0);
    harmony_assert(vertex_buffers != NULL);
    harmony_assert(offsets != NULL);
    vk->pfn.vkCmdBindVertexBuffers(cmd, begin_index, count, vertex_buffers, offsets);
}

void harmony_vk_bind_vertex_buffer(const HarmonyVulkan *vk, VkCommandBuffer cmd, VkBuffer vertex_buffer) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(vertex_buffer != VK_NULL_HANDLE);
    vk->pfn.vkCmdBindVertexBuffers(cmd, 0, 1, &vertex_buffer, &(usize){0});
}

void harmony_vk_bind_index_buffer(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    VkBuffer index_buffer,
    usize offset,
    VkIndexType type
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(index_buffer != VK_NULL_HANDLE);
    vk->pfn.vkCmdBindIndexBuffer(cmd, index_buffer, offset, type);
}

void harmony_vk_draw(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    u32 first_vertex,
    u32 vertex_count,
    u32 first_instance,
    u32 instance_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(vertex_count > 0);
    harmony_assert(instance_count > 0);
    vk->pfn.vkCmdDraw(cmd, vertex_count, instance_count, first_vertex, first_instance);
}

void harmony_vk_draw_indexed(
    const HarmonyVulkan *vk,
    VkCommandBuffer cmd,
    u32 vertex_offset,
    u32 first_index,
    u32 index_count,
    u32 first_instance,
    u32 instance_count
) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(index_count > 0);
    harmony_assert(instance_count > 0);
    vk->pfn.vkCmdDrawIndexed(cmd, index_count, instance_count, first_index, (i32)vertex_offset, first_instance);
}

void harmony_vk_dispatch(const HarmonyVulkan *vk, VkCommandBuffer cmd, u32 x, u32 y, u32 z) {
    harmony_assert(vk != NULL);
    harmony_assert(cmd != VK_NULL_HANDLE);
    harmony_assert(x > 0);
    harmony_assert(y > 0);
    harmony_assert(z > 0);
    vk->pfn.vkCmdDispatch(cmd, x, y, z);
}

#endif // defined(HARMONY_IMPLEMENTATION_GRAPHICS) || defined(HARMONY_IMPLEMENTATION_ALL)

#endif // HARMONY_GRAPHICS_H
