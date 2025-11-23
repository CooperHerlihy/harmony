#ifndef HARMONY_IMPLEMENTATION_ALL
#define HARMONY_IMPLEMENTATION_ALL
#endif
#include "harmony.h"
#include "harmony_audio.h"
#include "harmony_containers.h"
#include "harmony_files.h"
#include "harmony_graphics.h"
#include "harmony_math.h"

int main(void) {
    VkInstance instance = harmony_vk_create_instance();
    VkDebugUtilsMessengerEXT debug_messenger = harmony_vk_create_debug_messenger(instance);
    VkPhysicalDevice gpu = harmony_vk_find_physical_device(instance);
    u32 queue_family;
    (void)harmony_vk_find_queue_family(gpu, &queue_family, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    VkDevice device = harmony_vk_create_single_queue_device(gpu, queue_family);

    HarmonyPlatform *platform = harmony_platform_create();
    HarmonyWindow window = harmony_window_create(platform, &(HarmonyWindowConfig){
        .title = "Harmony Test",
    });

    while(true) {
        harmony_window_process_events(platform, &(HarmonyWindow *){&window}, 1);
        if (window.was_closed || window.keys_down[HARMONY_KEY_ESCAPE])
            goto shutdown;
    }
shutdown:

    harmony_window_destroy(platform, &window);
    harmony_platform_destroy(platform);

    harmony_vk_destroy_device(device);
    harmony_vk_destroy_debug_messenger(instance, debug_messenger);
    harmony_vk_destroy_instance(instance);

    harmony_log_info("Tests complete\n");
}

