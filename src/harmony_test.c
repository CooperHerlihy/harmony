#ifndef HARMONY_IMPLEMENTATION_ALL
#define HARMONY_IMPLEMENTATION_ALL
#endif
#include "harmony.h"
#include "harmony_audio.h"
#include "harmony_containers.h"
#include "harmony_files.h"
#include "harmony_graphics.h"
#include "harmony_math.h"

#include <alloca.h>

int main(void) {
    HarmonyPlatform *platform = harmony_platform_create();
    HarmonyWindow window = harmony_window_create(platform, &(HarmonyWindowConfig){
        .title = "Harmony Test",
        .windowed = true,
        .width = 800,
        .height = 600,
    });

    VkInstance instance = harmony_vk_create_instance();
    harmony_debug_mode(VkDebugUtilsMessengerEXT debug_messenger = harmony_vk_create_debug_messenger(instance));
    VkSurfaceKHR surface = harmony_vk_create_surface(instance, platform, &window);

    VkPhysicalDevice gpu = harmony_vk_find_physical_device(instance);
    u32 queue_family;
    (void)harmony_vk_find_queue_family(gpu, &queue_family, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    VkDevice device = harmony_vk_create_single_queue_device(gpu, queue_family);
    VkQueue queue = harmony_vk_get_queue(device, queue_family, 0);

    u32 swap_width;
    u32 swap_height;
    VkFormat swap_format;
    VkSwapchainKHR swapchain = harmony_vk_create_swapchain(
        device, gpu, &swap_width, &swap_height, &swap_format, NULL, surface,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_PRESENT_MODE_FIFO_KHR);

    u32 swap_image_count = harmony_vk_get_swapchain_image_count(device, swapchain);
    VkImage *swap_images = malloc(swap_image_count * sizeof(*swap_images));
    harmony_vk_get_swapchain_images(device, swapchain, swap_images, swap_image_count);

    VkCommandPool command_pool = harmony_vk_create_command_pool(
        device, queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkCommandBuffer cmd;
    harmony_vk_allocate_command_buffers(device, command_pool, &cmd, 1);

    VkSemaphore acquire_semaphore = harmony_vk_create_semaphore(device, 0);
    VkSemaphore render_semaphore = harmony_vk_create_semaphore(device, 0);
    VkFence render_fence = harmony_vk_create_fence(device, 0);

    u32 frame_count = 0;
    f64 frame_time = 0.0f;
    HarmonyClock hclock;
    harmony_clock_tick(&hclock);

    while(true) {
        f64 delta = harmony_clock_tick(&hclock);
        ++frame_count;
        frame_time += delta;
        if (frame_time > 1.0f) {
            harmony_log_info("fps: %d, avg: %fms\n", frame_count, 1.0e3 / (f64)frame_count);
            frame_count = 0;
            frame_time -= 1.0f;
        }

        harmony_window_process_events(platform, &(HarmonyWindow *){&window}, 1);
        if (window.was_closed || window.keys_down[HARMONY_KEY_ESCAPE])
            break;

        if (window.was_resized) {
            VkSwapchainKHR old_swapchain = swapchain;
            swapchain = harmony_vk_create_swapchain(
                device, gpu, &swap_width, &swap_height, &swap_format, old_swapchain, surface,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_PRESENT_MODE_FIFO_KHR);

            u32 old_count = swap_image_count;
            swap_image_count = harmony_vk_get_swapchain_image_count(device, swapchain);
            if (swap_image_count != old_count)
                swap_images = realloc(swap_images, swap_image_count * sizeof(*swap_images));
            harmony_vk_get_swapchain_images(device, swapchain, swap_images, swap_image_count);

            harmony_vk_destroy_swapchain(device, old_swapchain);
            harmony_log_info("window resized\n");
        }

        u32 image_index;
        if (!harmony_vk_acquire_next_image(device, swapchain, &image_index, acquire_semaphore, VK_NULL_HANDLE))
            continue;
        VkImage current_image = swap_images[image_index];

        harmony_vk_begin_cmd(device, cmd, 0);

        VkImageMemoryBarrier2 color_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .image = current_image,
            .subresourceRange = (VkImageSubresourceRange){VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
        };
        harmony_vk_pipeline_barrier(cmd, &(VkDependencyInfo){
            .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &color_barrier,
        });

        // render pass

        VkImageMemoryBarrier2 present_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .image = current_image,
            .subresourceRange = (VkImageSubresourceRange){VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
        };
        harmony_vk_pipeline_barrier(cmd, &(VkDependencyInfo){
            .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &present_barrier,
        });

        harmony_vk_end_cmd(cmd);

        harmony_vk_submit_commands(queue, &(VkSubmitInfo){
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &acquire_semaphore,
            .pWaitDstStageMask = &(VkPipelineStageFlags){VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT},
            .commandBufferCount = 1,
            .pCommandBuffers = &cmd,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &render_semaphore,
        }, 1, render_fence);

        harmony_vk_present(queue, swapchain, image_index, &render_semaphore, 1);

        harmony_vk_wait_for_fences(device, &render_fence, 1);
        harmony_vk_reset_fences(device, &render_fence, 1);
    }

    harmony_vk_wait_for_device(device);

    harmony_vk_destroy_fence(device, render_fence);
    harmony_vk_destroy_semaphore(device, render_semaphore);
    harmony_vk_destroy_semaphore(device, acquire_semaphore);

    harmony_vk_free_command_buffers(device, command_pool, &cmd, 1);
    harmony_vk_destroy_command_pool(device, command_pool);

    free(swap_images);
    harmony_vk_destroy_swapchain(device, swapchain);

    harmony_vk_destroy_device(device);
    harmony_vk_destroy_surface(instance, surface);
    harmony_debug_mode(harmony_vk_destroy_debug_messenger(instance, debug_messenger));
    harmony_vk_destroy_instance(instance);

    harmony_window_destroy(platform, &window);
    harmony_platform_destroy(platform);

    harmony_log_info("Tests complete\n");
}

