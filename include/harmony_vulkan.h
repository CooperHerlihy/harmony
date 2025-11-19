#ifndef HARMONY_VULKAN_H
#define HARMONY_VULKAN_H

#include "harmony_core.h"

#define HARMONY_MAKE_VULKAN_FUNC(name) PFN_##name name;

typedef struct HarmonyVulkanFuncs {
    HARMONY_MAKE_VULKAN_FUNC(vkGetInstanceProcAddr)
    HARMONY_MAKE_VULKAN_FUNC(vkGetDeviceProcAddr)

    HARMONY_MAKE_VULKAN_FUNC(vkEnumerateInstanceExtensionProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkEnumerateInstanceLayerProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkCreateInstance)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyInstance)

#ifndef NDEBUG
    HARMONY_MAKE_VULKAN_FUNC(vkCreateDebugUtilsMessengerEXT)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyDebugUtilsMessengerEXT)
#endif

    HARMONY_MAKE_VULKAN_FUNC(vkEnumeratePhysicalDevices)
    HARMONY_MAKE_VULKAN_FUNC(vkEnumerateDeviceExtensionProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceQueueFamilyProperties)
    HARMONY_MAKE_VULKAN_FUNC(vkGetPhysicalDeviceFeatures)

    HARMONY_MAKE_VULKAN_FUNC(vkCreateDevice)
    HARMONY_MAKE_VULKAN_FUNC(vkDestroyDevice)

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

#undef HARMONY_CREATE_VULKAN_FUNC

/**
 * Basic resources for use in Vulkan code
 */
struct HarmonyVulkan {
    VkInstance instance;
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
    VkPhysicalDevice gpu;
    VkDevice device;
    HarmonyVulkanFuncs pfn;
};

/**
 * Creates basic resources needed for Vulkan
 *
 * Debug messenger is created in debug mode only
 * Device has synchronization 2 and dynamic rendering features enabled
 *
 * Returns
 * - Created Vulkan resources
 */
HarmonyVulkan *harmony_vulkan_create(const Harmony *harmony);

/**
 * Destroys basic resources for Vulkan
 *
 * All resources created with device or instance must be destroyed
 */
void harmony_vulkan_destroy(const Harmony *harmony, HarmonyVulkan *vulkan);

/**
 * Fills a buffer with pointers to names of the platform's required instance
 * extensions for windowing
 *
 * Parameters
 * - extension_buffer The buffer to fill, must not be NULL
 * - extension_buffer_capacity The capacity of the buffer in char pointers
 * Returns
 * - The number of extensions filled
 */
usize harmony_platform_get_required_vulkan_instance_extensions(
    const Harmony *harmony,
    const char** extension_buffer,
    usize extension_buffer_capacity);

/**
 * Creates a Vulkan instance with sensible defaults
 * 
 * In debug mode, enables debug messaging
 *
 * Returns
 * - The created VkInstance
 */
VkInstance harmony_vk_create_instance(void);

/**
 * Destroys a Vulkan instance
 * 
 * Parameters
 * - instance The VkInstance to destroy
 */
void harmony_vk_destroy_instance(VkInstance instance);

#ifndef NDEBUG

VkDebugUtilsMessengerEXT harmony_vk_create_debug_messenger(const Harmony *harmony);

void harmony_vk_destroy_debug_messenger(const Harmony *harmony, VkDebugUtilsMessengerEXT messenger);

#endif

VkPhysicalDevice harmony_vk_find_physical_device(const Harmony *harmony);

VkDevice harmony_vk_create_device(const Harmony *harmony);

void harmony_vk_destroy_device(const Harmony *harmony, VkDevice device);

VkSurfaceKHR harmony_platform_create_vulkan_surface(const Harmony *harmony);

void harmony_vk_destroy_surface(const Harmony *harmony, VkSurfaceKHR surface);

VkSwapchainKHR harmony_vk_create_swapchain(
    const Harmony *harmony,
    u32 *width,
    u32 *height,
    VkFormat *format,
    VkSwapchainKHR old_swapchain,
    VkSurfaceKHR surface,
    VkImageUsageFlags image_usage,
    VkPresentModeKHR desired_mode);

u32 harmony_vk_get_swapchain_image_count(const Harmony *harmony, VkSwapchainKHR swapchain);

void harmony_vk_get_swapchain_images(const Harmony *harmony, VkImage *images, u32 count);

u32 harmony_vk_acquire_next_image(
    const Harmony *harmony,
    VkSwapchainKHR swapchain,
    VkSemaphore signal_semaphore,
    VkFence signal_fence);

void harmony_vk_present(
    VkQueue queue,
    VkSwapchainKHR swapchain,
    u32 image_index,
    VkSemaphore *wait_semaphores,
    u32 semaphore_count);

VkSemaphore harmony_vk_create_semaphore(const Harmony *harmony);

void harmony_vk_destroy_semaphore(const Harmony *harmony, VkSemaphore semaphore);

VkFence harmony_vk_create_fence(const Harmony *harmony, VkFenceCreateFlags flags);

void harmony_vk_destroy_fence(const Harmony *harmony, VkFence fence);

void harmony_vk_wait_for_fences(const Harmony *harmony, VkFence *fences, u32 count);

void harmony_vk_reset_fences(const Harmony *harmony, VkFence *fences, u32 count);

bool harmony_vk_find_queue_family(const Harmony *harmony, u32 *queue_family, VkQueueFlags queue_flags);

VkQueue harmony_vk_get_queue(const Harmony *harmony, u32 index);

VkCommandPool harmony_vk_create_command_pool(const Harmony *harmony, VkCommandPoolCreateFlags flags);

void harmony_vk_destroy_command_pool(const Harmony *harmony, VkCommandPool pool);

void harmony_vk_allocate_command_buffer(
    const Harmony *harmony,
    VkCommandPool pool,
    VkCommandBuffer *cmds,
    u32 count);

void harmony_vk_free_command_buffers(
    const Harmony *harmony,
    VkCommandPool pool,
    VkCommandBuffer *cmds,
    u32 count);

VkDescriptorPool harmony_vk_create_descriptor_pool(const Harmony *harmony);

void harmony_vk_destroy_descriptor_pool(const Harmony *harmony, VkDescriptorPool pool);

void harmony_vk_allocate_descriptor_sets(
    const Harmony *harmony,
    VkDescriptorPool pool,
    VkDescriptorSetLayout *layouts,
    VkDescriptorSet *sets,
    u32 count);

void harmony_vk_free_descriptor_sets(
    const Harmony *harmony,
    VkDescriptorPool pool,
    VkDescriptorSet *sets,
    u32 count);

VkDescriptorSetLayout harmony_vk_create_descriptor_set_layout(
    const Harmony *harmony,
    const VkDescriptorSetLayoutBinding *bindings,
    u32 count);

void harmony_vk_destroy_descriptor_set_layout(const Harmony *harmony, VkDescriptorSetLayout layout);

VkPipelineLayout harmony_vk_create_pipeline_layout(
    const Harmony *harmony,
    const VkDescriptorSetLayout* layouts,
    u32 layout_count,
    const VkPushConstantRange* push_constants,
    u32 push_constant_count
);

void harmony_vk_destroy_pipeline_layout(const Harmony *harmony, VkPipelineLayout layout);

VkShaderModule harmony_vk_create_shader_module(const Harmony *harmony, const u8 *code, usize size);

void harmony_vk_destroy_shader_module(const Harmony *harmony, VkShaderModule shader_module);

typedef struct HarmonyVkPipelineConfig {

    VkPipelineLayout layout;

    VkShaderModule *shaders;

    VkShaderStageFlagBits *shader_stages;

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

VkPipeline harmony_vk_create_graphics_pipeline(const Harmony *harmony, const HarmonyVkPipelineConfig *config);

VkPipeline harmony_vk_create_compute_pipeline(const Harmony *harmony, const HarmonyVkPipelineConfig *config);

void harmony_vk_destroy_pipeline(const Harmony *harmony, VkPipeline pipeline);

VkBuffer harmony_vk_create_buffer(const Harmony *harmony, usize size, VkBufferUsageFlags usage);

VkImage harmony_vk_create_image(
    const Harmony *harmony,
    VkFormat format,
    u32 width,
    u32 height,
    u32 depth,
    u32 dimensions,
    u32 mip_levels,
    u32 array_layers,
    u32 multisample_count,
    VkImageUsageFlags usage,
    VkImageCreateFlags flags);

VkImageView harmony_vk_create_image_view(
    const Harmony *harmony,
    VkImage image,
    VkImageViewType view_type,
    VkFormat format,
    VkImageAspectFlags aspect,
    uint32_t base_mip,
    uint32_t mip_count,
    uint32_t base_layer,
    uint32_t layer_count);

VkSampler harmony_vk_create_sampler(
    const Harmony *harmony,
    VkSamplerAddressMode edge_mode,
    VkFilter mag_filter,
    VkFilter min_filter,
    VkFilter mip_filter,
    u32 max_mips);

typedef struct HarmonyVkImageConfig {
    /**
     * The format of each pixel, must not be UNDEFINED
     */
    VkFormat format;
    /**
     * The width of the texture in pixels, 0 defaults to 1
     */
    u32 width;
    /**
     * The height of the texture in pixels, 0 defaults to 1
     */
    u32 height;
    /**
     * The depth of the texture in pixels, 0 defaults to 1
     */
    u32 depth;
    /**
     * The dimensionality, 1, 2, or 3, for 1D, 2D, or 3D, and 0 defaults to 2D
     */
    u32 dimensions;
    /**
     * Create space for mipmaps, 0 defaults to 1 (no mips), and UINT32_MAX
     * creates the maximum mips for the size of the texture
     */
    u32 mip_levels;
    /**
     * The number of array layers, 0 default to 1
     */
    u32 array_layers;
    /**
     * The number of samples for MSAA, 0 defaults to 1
     */
    u32 multisample_count;
    /**
     * How the texture will be used, must not be UNDEFINED
     */
    VkImageUsageFlags usage;
    /**
     * How sampling off the edge of the texture behaves, defaults to REPEAT
     */
    VkSamplerAddressMode edge_mode;
    /**
     * The filter the sampler will use, defaults to NEAREST
     */
    VkFilter filter;
    /**
     * make_cubemap creates a cubemap, width must equal height, depth must be 1,
     * dimensions must be 2D, mip_levels must be 1, and array_layers must be 6
     */
    bool make_cubemap;
} HarmonyVkImageConfig;

void harmony_vk_create_image_bundle(
    const Harmony *harmony,
    VkImage *image,
    VkImageView *image_view,
    VkSampler *sampler,
    HarmonyVkImageConfig *config);

VkMemoryRequirements harmony_vk_get_buffer_mem_reqs(const Harmony *harmony, VkBuffer buffer);

VkMemoryRequirements harmony_vk_get_image_mem_reqs(const Harmony *harmony, VkImage buffer);

VkPhysicalDeviceMemoryProperties harmony_vk_get_gpu_mem_properties(const Harmony *harmony);

u32 harmony_vk_find_memory_type_index(
    const Harmony *harmony,
    VkPhysicalDeviceMemoryProperties *mem_props,
    u32 bitmask,
    VkMemoryPropertyFlags desired_flags,
    VkMemoryPropertyFlags undesired_flags);

VkDeviceMemory harmony_vk_allocate_memory(const Harmony *harmony, usize size, u32 memory_type_index);

void harmony_vk_free_memory(const Harmony *harmony, VkDeviceMemory memory);

void harmony_vk_bind_buffer_memory(const Harmony *harmony, VkBuffer buffer, VkDeviceMemory memory, usize offset);

void harmony_vk_bind_image_memory(const Harmony *harmony, VkImage image, VkDeviceMemory memory, usize offset);

void *harmony_vk_map_memory(const Harmony *harmony, VkDeviceMemory memory, usize offset, usize size);

void harmony_vk_unmap_memory(const Harmony *harmony, VkDeviceMemory memory);

void harmony_vk_flush_memory(const Harmony *harmony, VkDeviceMemory memory, usize offset, usize size);

void harmony_vk_invalidate_memory(const Harmony *harmony, VkDeviceMemory memory, usize offset, usize size);

void harmony_vk_begin_cmd(const Harmony *harmony, VkCommandBuffer cmd, VkCommandBufferUsageFlags flags);

void harmony_vk_end_cmd(const Harmony *harmony, VkCommandBuffer cmd);

void harmony_vk_copy_buffer(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkBuffer dst,
    VkBuffer src,
    const VkBufferCopy *regions,
    u32 region_count);

void harmony_vk_copy_image(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkImage dst,
    VkImage src,
    const VkImageCopy *regions,
    u32 region_count);

void harmony_vk_blit_image(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkImage dst,
    VkImage src,
    const VkImageBlit *regions,
    u32 region_count,
    VkFilter filter);

void harmony_vk_copy_buffer_to_image(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkImage dst,
    VkBuffer src,
    VkBufferImageCopy *regions,
    u32 region_count);

void harmony_vk_copy_image_to_buffer(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkBuffer dst,
    VkImage src,
    VkBufferImageCopy *regions,
    u32 region_count);

void harmony_vk_pipeline_barrier(const Harmony *harmony, VkCommandBuffer cmd, const VkDependencyInfo *dependencies);

void harmony_vk_begin_rendering(const Harmony *harmony, VkCommandBuffer cmd, const VkRenderingInfo *info);

void harmony_vk_end_rendering(const Harmony *harmony, VkCommandBuffer cmd);

void harmony_vk_set_viewport(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    float x,
    float y,
    float width,
    float height,
    float near,
    float far);

void harmony_vk_set_scissor(const Harmony *harmony, VkCommandBuffer cmd, i32 x, i32 y, u32 width, u32 height);

void harmony_vk_bind_pipeline(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkPipeline pipeline,
    VkPipelineBindPoint bind_point
);

void harmony_vk_bind_descriptor_sets(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkPipelineLayout layout,
    VkPipelineBindPoint bind_point,
    u32 begin_index,
    u32 set_count,
    const VkDescriptorSet *descriptor_sets);

void harmony_vk_push_constants(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkPipelineLayout layout,
    VkPipelineBindPoint bind_point,
    u32 offset,
    u32 size,
    const void *data);

void harmony_vk_bind_vertex_buffer(const Harmony *harmony, VkCommandBuffer cmd, VkBuffer vertex_buffer);

void harmony_vk_bind_vertex_buffers(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    u32 begin_index,
    u32 count,
    VkBuffer *vertex_buffers,
    usize *offsets);

void harmony_vk_bind_index_buffer(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    VkBuffer index_buffer,
    usize offset,
    VkIndexType type);

void harmony_vk_draw(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    u32 vertex_begin_index,
    u32 vertex_count,
    u32 instance_begin_index,
    u32 instance_count);

void harmony_vk_draw_indexed(
    const Harmony *harmony,
    VkCommandBuffer cmd,
    u32 vertex_offset,
    u32 index_begin_index,
    u32 index_count,
    u32 instance_begin_index,
    u32 instance_count);

void harmony_vk_dispatch(const Harmony *harmony, VkCommandBuffer cmd, u32 x, u32 y, u32 z);

#endif // HARMONY_VULKAN_H
