#include "harmony_os.h"

#include <time.h>
#include <unistd.h>
#include <dlfcn.h>

#include <stdlib.h>
#include <stdio.h>

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

#include "stb_image.h"
#include "stb_image_write.h"

void *harmony_platform_init(const Harmony *h) {
    // platform internal : TODO
    (void)h;
    return NULL;
}

f64 harmony_clock_delta(HarmonyClock *hclock) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    HarmonyClock new_clock = {
        .seconds = (f64)time.tv_sec,
        .nanoseconds = (f64)time.tv_nsec,
    };
    f64 new_time = new_clock.seconds + new_clock.nanoseconds / 1.0e9;
    f64 old_time = hclock->seconds + hclock->nanoseconds / 1.0e9;

    return new_time - old_time;
}

f64 harmony_clock_tick(HarmonyClock *hclock) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    HarmonyClock new_clock = {
        .seconds = (f64)time.tv_sec,
        .nanoseconds = (f64)time.tv_nsec,
    };
    f64 new_time = new_clock.seconds + new_clock.nanoseconds / 1.0e9;
    f64 old_time = hclock->seconds + hclock->nanoseconds / 1.0e9;

    *hclock = new_clock;
    return new_time - old_time;
}

void harmony_system_log_verbose(void *dummy, HarmonyString format, ...) {
    (void)dummy;
    char prefix[] = "Harmony Verbose: ";
    char postfix[] = "\n";
    ssize_t write_length;

    write_length = write(FD_STDOUT, prefix, countof(prefix));
    if (write_length != countof(prefix))
        return;

    write_length = write(FD_STDOUT, format.data, format.length);
    if ((usize)write_length != format.length)
        return;

    write_length = write(FD_STDOUT, postfix, countof(postfix));
    if (write_length != countof(postfix))
        return;
}

void harmony_system_log_info(void *dummy, HarmonyString format, ...) {
    (void)dummy;
    char prefix[] = "Harmony Info: ";
    char postfix[] = "\n";
    ssize_t write_length;

    write_length = write(FD_STDOUT, prefix, countof(prefix));
    if (write_length != countof(prefix))
        return;

    write_length = write(FD_STDOUT, format.data, format.length);
    if ((usize)write_length != format.length)
        return;

    write_length = write(FD_STDOUT, postfix, countof(postfix));
    if (write_length != countof(postfix))
        return;

}

void harmony_system_log_warning(void *dummy, HarmonyString format, ...) {
    (void)dummy;
    char prefix[] = "Harmony Warning: ";
    char postfix[] = "\n";
    ssize_t write_length;


    write_length = write(FD_STDERR, prefix, countof(prefix));
    if (write_length != countof(prefix))
        return;

    write_length = write(FD_STDERR, format.data, format.length);
    if ((usize)write_length != format.length)
        return;

    write_length = write(FD_STDERR, postfix, countof(postfix));
    if (write_length != countof(postfix))
        return;

}

void harmony_system_log_error(void *dummy, HarmonyString format, ...) {
    (void)dummy;
    char prefix[] = "Harmony Error: ";
    char postfix[] = "\n";
    ssize_t write_length;


    write_length = write(FD_STDERR, prefix, countof(prefix));
    if (write_length != countof(prefix))
        return;

    write_length = write(FD_STDERR, format.data, format.length);
    if ((usize)write_length != format.length)
        return;

    write_length = write(FD_STDERR, postfix, countof(postfix));
    if (write_length != countof(postfix))
        return;

}

void harmony_system_log_debug(void *dummy, HarmonyString format, ...) {
    (void)dummy;
    char prefix[] = "Harmony Debug: ";
    char postfix[] = "\n";
    ssize_t write_length;


    write_length = write(FD_STDERR, prefix, countof(prefix));
    if (write_length != countof(prefix))
        return;

    write_length = write(FD_STDERR, format.data, format.length);
    if ((usize)write_length != format.length)
        return;

    write_length = write(FD_STDERR, postfix, countof(postfix));
    if (write_length != countof(postfix))
        return;

}

void *harmony_page_alloc(void) {
    // page alloc : TODO
    return NULL;
}

void harmony_page_free(void *pages) {
    // page free : TODO
    (void)pages;
}

HarmonySystemAllocator harmony_system_allocator_create(const Harmony *h) {
    (void)h;
    return (HarmonySystemAllocator){0};
}

void harmony_system_allocator_destroy(const Harmony *h, HarmonySystemAllocator *a) {
    (void)h;
    (void)a;
}

void *harmony_system_allocator_alloc(HarmonySystemAllocator *a, usize size) {
    (void)a;
    return malloc(size);
}

void *harmony_system_allocator_realloc(HarmonySystemAllocator *a, void *allocation, usize old_size, usize new_size) {
    (void)a;
    (void)old_size;
    return realloc(allocation, new_size);
}

void harmony_system_allocator_free(HarmonySystemAllocator *a, void *allocation, usize size) {
    (void)a;
    (void)size;
    free(allocation);
}

HarmonyError harmony_dynamic_lib_open(const Harmony *h, void **lib, HarmonyString path) {
    (void)h;
    harmony_assert(h, lib != NULL);
    harmony_assert(h, path.data != NULL);
    harmony_assert(h, path.length > 0);
    *lib = NULL;

    char cpath[256];
    harmony_mem_copy(cpath, path.data, path.length);
    cpath[path.length] = '\0';

    void *library = dlopen(cpath, RTLD_LAZY);
    if (library == NULL) {
        harmony_log_warning(h, harmony_string("Could not open dynamic library: {s}"), path);
        return HARMONY_ERROR_DYNAMIC_LIB_NOT_FOUND;
    }

    *lib = library;
    return HARMONY_SUCCESS;
}

void harmony_dynamic_lib_close(const Harmony *h, void *lib) {
    (void)h;
    harmony_assert(h, lib != NULL);
    dlclose(lib);
}

HarmonyError harmony_dynamic_lib_load_symbol(const Harmony *h, void **symbol, void *lib, HarmonyString name) {
    (void)h;
    harmony_assert(h, symbol != NULL);
    harmony_assert(h, name.data != NULL);
    harmony_assert(h, name.length > 0);
    *symbol = NULL;

    char cname[256];
    harmony_mem_copy(cname, name.data, name.length);
    cname[name.length] = '\0';

    void *sym = dlsym(lib, cname);
    if (sym == NULL) {
        harmony_log_warning(h, harmony_string("Could not load symbol from dynamic lib: {s}"), name);
        return HARMONY_ERROR_DYNAMIC_LIB_SYMBOL_NOT_FOUND;
    }

    *symbol = sym;
    return HARMONY_SUCCESS;
}

HarmonyError harmony_file_load_binary(const Harmony *h, HarmonyString path, u8** data, usize* size) {
    harmony_assert(h, path.data != NULL);
    harmony_assert(h, path.length > 0);
    harmony_assert(h, data != NULL);
    harmony_assert(h, size != NULL);
    *data = NULL;
    *size = 0;

    char cpath[256];
    harmony_mem_copy(cpath, path.data, path.length);
    cpath[path.length] = '\0';

    FILE* file = fopen(cpath, "rb");
    if (file == NULL) {
        harmony_log_warning(h, harmony_string("Could not find file: {s}"), path);
        return HARMONY_ERROR_FILE_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    usize file_size = (usize)ftell(file);
    rewind(file);

    u8* file_data = harmony_alloc(h, file_size);
    if (fread(file_data, 1, file_size, file) != file_size) {
        fclose(file);
        harmony_log_warning(h, harmony_string("Failed to read file: {s}"), path);
        return HARMONY_ERROR_FILE_READ_FAILURE;
    }

    *data = file_data;
    *size = file_size;
    fclose(file);
    return HARMONY_SUCCESS;
}

void harmony_file_unload_binary(const Harmony *h, u8* file_data, usize file_size) {
    if (file_data == NULL)
        harmony_assert(h, file_size == 0);
    harmony_free(h, file_data, file_size);
}

HarmonyError harmony_file_save_binary(const Harmony *h, HarmonyString path, const u8* data, usize size) {
    (void)h;
    harmony_assert(h, path.data != NULL);
    harmony_assert(h, path.length > 0);
    harmony_assert(h, data != NULL);
    harmony_assert(h, size > 0);

    char cpath[256];
    harmony_mem_copy(cpath, path.data, path.length);
    cpath[path.length] = '\0';

    FILE* file = fopen(cpath, "wb");
    if (file == NULL)
        return HARMONY_ERROR_FILE_NOT_FOUND;

    if (fwrite(data, 1, size, file) != size) {
        fclose(file);
        return HARMONY_ERROR_FILE_WRITE_FAILURE;
    }

    fclose(file);
    return HARMONY_SUCCESS;
}

HarmonyError harmony_file_load_image(const Harmony *h, HarmonyString path, u32** data, u32* width, u32* height) {
    (void)h;
    harmony_assert(h, path.data != NULL);
    harmony_assert(h, path.length > 0);
    harmony_assert(h, data != NULL);
    harmony_assert(h, width != NULL);
    harmony_assert(h, height != NULL);
    *data = NULL;
    *width = 0;
    *height = 0;

    char cpath[256];
    harmony_mem_copy(cpath, path.data, path.length);
    cpath[path.length] = '\0';

    int file_width, file_height, file_channels;
    stbi_uc* file_data = stbi_load(cpath, &file_width, &file_height, &file_channels, STBI_rgb_alpha);
    if (file_data == NULL)
        return HARMONY_ERROR_FILE_NOT_FOUND;
    if (file_width <= 0 || file_height <= 0 || file_channels <= 0) {
        stbi_image_free(file_data);
        return HARMONY_ERROR_FILE_READ_FAILURE;
    }

    *data = (u32*)file_data;
    *width = (u32)file_width;
    *height = (u32)file_height;
    return HARMONY_SUCCESS;
}

void harmony_file_unload_image(const Harmony *h, u32* data, u32 width, u32 height) {
    (void)h;
    (void)width;
    (void)height;
    if (data == NULL)
        harmony_assert(h, width == 0 && height == 0);
    stbi_image_free(data);
}

HarmonyError harmony_file_save_image(const Harmony *h, HarmonyString path, const u32* data, u32 width, u32 height) {
    harmony_assert(h, path.data != NULL);
    harmony_assert(h, path.length > 0);
    harmony_assert(h, data != NULL);
    harmony_assert(h, width > 0);
    harmony_assert(h, height > 0);

    char cpath[256];
    harmony_mem_copy(cpath, path.data, path.length);
    cpath[path.length] = '\0';

    int result = stbi_write_png(cpath, (int)width, (int)height, 4, (void*)data, (int)(width * sizeof(u32)));
    if (result == 0) {
        harmony_log_warning(h, harmony_string("Failed to write file: {s}"), path);
        return HARMONY_ERROR_FILE_WRITE_FAILURE;
    }

    return HARMONY_SUCCESS;
}

HarmonyWindow* harmony_window_create(const Harmony* h, const HarmonyWindowConfig* config) {
    (void)h;
    (void)config;
    // window create : TODO
    return NULL;
}

void harmony_window_destroy(const Harmony* h, HarmonyWindow* window) {
    (void)h;
    (void)window;
    // window destroy : TODO
}

void harmony_window_process_events(const Harmony* h, HarmonyWindow** windows, u32 window_count) {
    (void)h;
    (void)windows;
    (void)window_count;
    // process events : TODO
}

