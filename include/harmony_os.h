#ifndef HARMONY_OS_H
#define HARMONY_OS_H

#include "harmony_core.h"

/**
 * Creates platform specific internal resources for audio, window, etc.
 *
 * Parameters
 * - harmony The harmony context for logging and allocation, must not be NULL
 * Returns
 * - An opaque pointer to the platform's resources
 */
void *harmony_platform_init(const Harmony *harmony);

/**
 * A high precision clock for timers and game deltas
 */
typedef struct HarmonyClock {
    f64 seconds;
    f64 nanoseconds;
} HarmonyClock;

/**
 * Gets the delta since last tick, without resetting
 *
 * Parameters
 * - clock The clock to tick, must not be NULL
 * Returns
 * - Seconds since last tick
 */
f64 harmony_clock_delta(HarmonyClock *clock);

/**
 * Resets the clock and gets the delta since the last tick in seconds
 *
 * Parameters
 * - clock The clock to tick, must not be NULL
 * Returns
 * - Seconds since last tick
 */
f64 harmony_clock_tick(HarmonyClock *clock);

/**
 * Formats and logs verbose messages to stdout
 *
 * Parameters
 * - dummy Ignored value to match HarmonyLogger interface
 * - format The string to be formatted then logged
 */
void harmony_system_log_verbose(void *dummy, HarmonyString format, ...);

/**
 * Formats and logs info messages to stdout
 *
 * Parameters
 * - dummy Ignored value to match HarmonyLogger interface
 * - format The string to be formatted then logged
 */
void harmony_system_log_info(void *dummy, HarmonyString format, ...);

/**
 * Formats and logs warning messages to stdout
 *
 * Parameters
 * - dummy Ignored value to match HarmonyLogger interface
 * - format The string to be formatted then logged
 */
void harmony_system_log_warning(void *dummy, HarmonyString format, ...);

/**
 * Formats and logs error messages to stdout
 *
 * Parameters
 * - dummy Ignored value to match HarmonyLogger interface
 * - format The string to be formatted then logged
 */
void harmony_system_log_error(void *dummy, HarmonyString format, ...);

/**
 * Formats and logs debug messages to stdout
 *
 * Parameters
 * - dummy Ignored value to match HarmonyLogger interface
 * - format The string to be formatted then logged
 */
void harmony_system_log_debug(void *dummy, HarmonyString format, ...);

/**
 * Sets up an interface to use the stdout and stderr loggers as a Harmony logger
 *
 * Returns
 * - The created interface
 */
inline HarmonyLogger harmony_system_logger(void) {
    return (HarmonyLogger){
        .data = NULL,
        .verbose = &harmony_system_log_verbose,
        .info = &harmony_system_log_info,
        .warning = &harmony_system_log_warning,
        .error = &harmony_system_log_error,
        .debug = &harmony_system_log_debug,
    };
}

/**
 * Allocates pages from the OS
 *
 * Returns
 * - The allocated pages
 * - NULL if failed to allocate
 */
void *harmony_page_alloc(void);

/**
 * Frees allocated pages from the OS
 *
 * Parameters
 * - pages The allocated pages to free, must not be NULL
 */
void harmony_page_free(void *pages);

/**
 * A general purpose allocator using a free list and page slab allocator
 *
 * Is thread safe
 */
typedef struct HarmonySystemAllocator {
    /**
     * Currently just a wrapper for malloc, so no data is needed
     */
    u8 dummy;
} HarmonySystemAllocator;

/**
 * Creates a system allocator
 *
 * Parameters
 * - harmony The Harmony context for logging
 * Returns
 * - The created allocator
 */
HarmonySystemAllocator harmony_system_allocator_create(const Harmony *harmony);

/**
 * Destroys a system allocator
 *
 * Parameters
 * - harmony The Harmony context for logging
 * - The allocator to destroy, must not be NULL
 */
void harmony_system_allocator_destroy(const Harmony *harmony, HarmonySystemAllocator *allocator);

/**
 * Allocates memory
 *
 * Parameters
 * - allocator The allocator to allocate using
 * - size The size in bytes of the allocation
 * Returns
 * - The allocation if successful
 * - NULL if the allocation failed, or size is 0
 */
void *harmony_system_allocator_alloc(HarmonySystemAllocator *allocator, usize size);

/**
 * Reallocates memory
 *
 * Parameters
 * - allocator The allocator to use
 * - allocation The allocation to grow
 * - old_size The original size in bytes of the allocation
 * - new_size The new size in bytes of the allocation
 * Returns
 * - The allocation if successful
 * - NULL if the allocation failed, or new_size is 0
 */
void *harmony_system_allocator_realloc(HarmonySystemAllocator *allocator, void *allocation, usize old_size, usize new_size);

/**
 * Frees an allocation from a arena allocator
 *
 * Parameters
 * - allocator The allocator to use
 * - allocation The allocation to free
 * - size The size of the allocation
 */
void harmony_system_allocator_free(HarmonySystemAllocator *allocator, void *allocation, usize size);

/**
 * Sets up an interface to use the system allocator as a Harmony allocator
 *
 * Parameters
 * - allocator The allocator to use
 * Returns
 * - The created interface
 */
inline HarmonyAllocator harmony_system_allocator(HarmonySystemAllocator *allocator) {
    return (HarmonyAllocator){
        .data = allocator,
        .alloc = (void *(*)(void *, usize))&harmony_system_allocator_alloc,
        .realloc = (void *(*)(void *, void *, usize, usize))&harmony_system_allocator_realloc,
        .free = (void (*)(void *, void *, usize))&harmony_system_allocator_free,
    };
}

// threading library : TODO

/**
 * Opens a dynamic library at the path
 *
 * Parameters
 * - harmony The harmony context, must not be NULL
 * - lib The pointer to store the library, must not be NULL
 * - path The path to the library, must not be empty
 * Returns
 * - HARMONY_SUCCESS on success
 * - HARMONY_ERROR_DYNAMIC_LIB_NOT_FOUND on failure
 */
HarmonyError harmony_dynamic_lib_open(const Harmony *harmony, void **lib, HarmonyString path);

/**
 * Closes a dynamic library
 *
 * Parameters
 * - harmony The harmony context, must not be NULL
 * - lib The dynamic library to close, must not be NULL
 */
void harmony_dynamic_lib_close(const Harmony *harmony, void *lib);

/**
 * Loads a symbol from a dynamic library
 *
 * Parameters
 * - harmony The harmony context, must not be NULL
 * - symbol The pointer to store the symbol, must not be NULL
 * - lib The library to load from, must not be NULL
 * - name The name of the symbol, must not be empty
 * Returns
 * - A pointer to the loaded symbol
 * - NULL on failure
 */
HarmonyError harmony_dynamic_lib_load_symbol(const Harmony *harmony, void **symbol, void *lib, HarmonyString name);

/**
 * Loads a binary file
 *
 * Parameters
 * - context The Harmony context, must not be NULL
 * - path The path to the file to load, must not be empty
 * - data A pointer to store the loaded data, must not be NULL
 * - size A pointer to store the size of the loaded data, must not be NULL
 * Returns
 * - HARMONY_SUCCESS if the file was loaded successfully
 * - HARMONY_ERROR_FILE_NOT_FOUND if the file was not found
 * - HARMONY_ERROR_FILE_READ_FAILURE if the file could not be read
 */
HarmonyError harmony_file_load_binary(const Harmony *harmony, HarmonyString path, u8** data, usize* size);

/**
 * Unloads a binary file
 *
 * Parameters
 * - context The Harmony context, must not be NULL
 * - data The data to unload, or NULL to do nothing
 * - size The size of the data to unload, must be 0 if data is NULL
 */
void harmony_file_unload_binary(const Harmony *harmony, u8* data, usize size);

/**
 * Saves a binary file
 *
 * Parameters
 * - context The Harmony context, must not be NULL
 * - path The path to the file to save, must not be empty
 * - data The data to save, must not be NULL
 * - size The size of the data to save, must be greater than 0
 * Returns
 * - HARMONY_SUCCESS if the file was saved successfully
 * - HARMONY_ERROR_FILE_WRITE_FAILURE if the file could not be written
 */
HarmonyError harmony_file_save_binary(const Harmony *harmony, HarmonyString path, const u8* data, usize size);

/**
 * Loads an image file
 *
 * Parameters
 * - context The Harmony context, must not be NULL
 * - path The null-terminated path to the file to load
 * - data A pointer to store the loaded data, must not be NULL
 * - width A pointer to store the width of the loaded data, must not be NULL
 * - height A pointer to store the height of the loaded data, must not be NULL
 * Returns
 * - HARMONY_SUCCESS if the file was loaded successfully
 * - HARMONY_ERROR_FILE_NOT_FOUND if the file was not found
 * - HARMONY_ERROR_FILE_READ_FAILURE if the file could not be read
 */
HarmonyError harmony_file_load_image(const Harmony *harmony, HarmonyString path, u32** data, u32* width, u32* height);

/**
 * Unloads an image file
 *
 * Just a wrapper around free()
 *
 * Parameters
 * - context The Harmony context, must not be NULL
 * - data The data to unload, or NULL to do nothing
 * - width The width of the data to unload, must be 0 if data is NULL
 * - height The height of the data to unload, must be 0 if data is NULL
 */
void harmony_file_unload_image(const Harmony *harmony, u32* data, u32 width, u32 height);

/**
 * Saves an image file
 *
 * Parameters
 * - context The Harmony context, must not be NULL
 * - path The path to the file to save, must not be empty
 * - width The width of the data to save, must be greater than 0
 * - height The height of the data to save, must be greater than 0
 * Returns
 * - HARMONY_SUCCESS if the file was saved successfully
 * - HARMONY_ERROR_FILE_WRITE_FAILURE if the file could not be written
 */
HarmonyError harmony_file_save_image(const Harmony *harmony, HarmonyString path, const u32* data, u32 width, u32 height);

// networking library : TODO

// audio library : TODO

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

/**
 * A window
 *
 * Note, the values stored in the struct are not to be changed manually, they
 * are updated by processing events, and can be read by the application
 */
typedef struct HarmonyWindow {
    /**
     * Platform dependent resources internal to the window
     */
    void *platform_internals;
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
 *
 * title is the title of the window, must not be NULL
 * windowed is whether it is windowed or fullscreen
 * width and height are the size if windowed, otherwise ignored
 */
typedef struct HarmonyWindowConfig {
    const char* title;
    bool windowed;
    u32 width;
    u32 height;
} HarmonyWindowConfig;

/**
 * Creates a window
 *
 * Parameters
 * - harmony The Harmony context, must not be NULL
 * - config The window configuration, must not be NULL
 * Returns
 * - A pointer to the window
 */
HarmonyWindow* harmony_window_create(const Harmony* harmony, const HarmonyWindowConfig* config);

/**
 * Destroys a window
 *
 * Parameters
 * - harmony The Harmony context, must not be NULL
 * - window The window, must not be NULL
 */
void harmony_window_destroy(const Harmony* harmony, HarmonyWindow* window);

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
void harmony_window_process_events(const Harmony* harmony, HarmonyWindow** windows, u32 window_count);

/**
 * Gets the size of the window
 *
 * Parameters
 * - window The window, must not be NULL
 * - width A pointer to store the width, must not be NULL
 * - height A pointer to store the height, must not be NULL
 */
inline void harmony_window_get_size(const HarmonyWindow* window, u32* width, u32* height) {
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
inline bool harmony_window_was_closed(const HarmonyWindow* window) {
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
inline bool harmony_window_was_resized(const HarmonyWindow* window) {
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
inline void harmony_window_get_mouse_pos(const HarmonyWindow* window, f32* x, f32* y) {
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
inline void harmony_window_get_mouse_delta(const HarmonyWindow* window, f32* x, f32* y) {
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
inline bool harmony_window_is_key_down(const HarmonyWindow* window, HarmonyKey key) {
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
inline bool harmony_window_was_key_pressed(const HarmonyWindow* window, HarmonyKey key) {
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
inline bool harmony_window_was_key_released(const HarmonyWindow* window, HarmonyKey key) {
    return window->keys_released[key];
}

#endif // HARMONY_OS_H
