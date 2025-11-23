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
    HarmonyVulkan vk = harmony_vulkan_create();
    harmony_vulkan_destroy(&vk);

    harmony_log_info("Tests : TODO\n");
}

