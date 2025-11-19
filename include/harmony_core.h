#ifndef HARMONY_CORE_H
#define HARMONY_CORE_H

#ifdef NDEBUG

/**
 * Runs code only in debug mode
 */
#define harmony_debug_mode(code)

#else

/**
 * Runs code only in debug mode
 */
#define harmony_debug_mode(code) code

#endif

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

#define HARMONY_MAX_ALIGNMENT 16

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef intptr_t isize;

/**
 * An immutable string
 */
typedef struct HarmonyString {
    const char* data;
    usize length;
} HarmonyString;

/**
 * Creates a string from a null terminated c style string
 *
 * Note, the max length of the C string is assumed 4096 chars, because it will
 * be assumed there is no null terminator
 *
 * Parameters
 * - str The null terminated string, may be NULL
 * Returns
 * - The created string
 */
HarmonyString harmony_string(const char* str);

/**
 * Gets the number of elements in a stack allocated array
 *
 * Parameters
 * - array The array to take the count of
 * Returns
 * - The nuymber of elements
 */
#define countof(array) (sizeof(array) / sizeof((array)[0]))

/**
 * Takes the minimum of two values
 *
 * Note, if the values are expressions, they will be evaluated twice
 *
 * Parameters
 * - a The first value to compare
 * - b The second value to compare
 * Returns
 * - The lower value
 */
#define min(a, b) ((a) < (b) ? (a) : (b))

/**
 * Takes the maximum of two values
 *
 * Note, if the values are expressions, they will be evaluated twice
 *
 * Parameters
 * - a The first value to compare
 * - b The second value to compare
 * Returns
 * - The higher value
 */
#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * Clamps a value between two bounds
 *
 * Note, if the values are expressions, they will be multiple times
 *
 * Parameters
 * - x The value to clamp
 * - a The lower bound
 * - b The upper bound
 * Returns
 * - The clamped value
 */
#define clamp(x, a, b) max((a), min((b), (x)))

/**
 * The Harmony interface for loggers
 */
typedef struct HarmonyLogger {
    /**
     * Opaque data passed to all functions
     */
    void *data;
    /**
     * Logs which are very verbose
     *
     * Parameters
     * - data The opaque data needed by the logger
     * - format The string to format before logging
     * - ... The items to format
     */
    void (*verbose)(void *data, HarmonyString format, ...);
    /**
     * Logs giving general info
     *
     * Parameters
     * - data The opaque data needed by the logger
     * - format The string to format before logging
     * - ... The items to format
     */
    void (*info)(void *data, HarmonyString format, ...);
    /**
     * Logs warning about potential dangers or oddities
     *
     * Parameters
     * - data The opaque data needed by the logger
     * - format The string to format before logging
     * - ... The items to format
     */
    void (*warning)(void *data, HarmonyString format, ...);
    /**
     * Logs indicating failure
     *
     * Parameters
     * - data The opaque data needed by the logger
     * - format The string to format before logging
     * - ... The items to format
     */
    void (*error)(void *data, HarmonyString format, ...);
    /**
     * Logs for debugging purposes
     *
     * Parameters
     * - data The opaque data needed by the logger
     * - format The string to format before logging
     * - ... The items to format
     */
    void (*debug)(void *data, HarmonyString format, ...);
} HarmonyLogger;

/**
 * The Harmony interface for generic allocators
 */
typedef struct HarmonyAllocator {
    /**
     * Opaque data passed to all functions
     */
    void *data;
    /**
     * Allocates memory
     */
    void *(*alloc)(void *data, usize size);
    /**
     * Changes the size of an allocation, potentially returning a different
     * allocation, with the data copied over
     */
    void *(*realloc)(void *data, void *allocation, usize old_size, usize new_size);
    /**
     * Frees allocated memory
     */
    void (*free)(void *data, void *allocation, usize size);
} HarmonyAllocator;

typedef struct HarmonyVulkan HarmonyVulkan;

/**
 * The context struct for all harmony functions
 *
 * Should always be thread safe
 */
typedef struct Harmony {
    /**
     * Interface to log messages
     */
    const HarmonyLogger *logger;
    /**
     * Interface to allocate memory
     */
    const HarmonyAllocator *allocator;
    /**
     * Internal platform resources for audio, windowing, etc.
     *
     * Create with harmony_platform_init()
     */
    const void *platform;
    /**
     * Common Vulkan resources and function pointers
     *
     * Create with harmony_vulkan_create();
     */
    const HarmonyVulkan *vk;
} Harmony;

/**
 * A convenience to call the verbose log from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - ... The format and data to fill it
 */
#define harmony_log_verbose(context, ...) \
    (context)->logger->verbose((context)->logger->data, __VA_ARGS__)

/**
 * A convenience to call the verbose log from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - ... The format and data to fill it
 */
#define harmony_log_info(context, ...) \
    (context)->logger->info((context)->logger->data, __VA_ARGS__)

/**
 * A convenience to call the warning log from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - ... The format and data to fill it
 */
#define harmony_log_warning(context, ...) \
    (context)->logger->warning((context)->logger->data, __VA_ARGS__)

/**
 * A convenience to call the error log from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - ... The format and data to fill it
 */
#define harmony_log_error(context, ...) \
    (context)->logger->error((context)->logger->data, __VA_ARGS__)

/**
 * A convenience to call the debug log from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - ... The format and data to fill it
 */
#define harmony_log_debug(context, ...) \
    harmony_debug_mode((context)->logger->debug((context)->logger->data, __VA_ARGS__));

/**
 * A convenience to call alloc from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - size The size in bytes to allocate
 * Returns
 * - The allocation
 * - NULL if the allocation failed
 */
inline void *harmony_alloc(const Harmony *harmony, usize size) {
    return harmony->allocator->alloc(harmony->allocator->data, size);
}

/**
 * A convenience to call realloc from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - allocation The allocation to resize
 * - old_size The original size in bytes of the allocation
 * - new_size The new size in bytes for the allocation
 * Returns
 * - The allocation
 * - NULL if the allocation failed
 */
inline void *harmony_realloc(const Harmony *harmony, void *allocation, usize old_size, usize new_size) {
    return harmony->allocator->realloc(harmony->allocator->data, allocation, old_size, new_size);
}

/**
 * A convenience to call alloc from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - allocation The allocation to resize
 * - size The size in bytes of the allocation
 */
inline void harmony_free(const Harmony *harmony, void *allocation, usize size) {
    harmony->allocator->free(harmony->allocator->data, allocation, size);
}

/**
 * Basic error codes for the Harmony library
 */
typedef enum HarmonyError {
    /**
     * Success, no error
     */
    HARMONY_SUCCESS = 0,
    /**
     * Failure, no error
     */
    HARMONY_FAILURE = 1,
    /**
     * Unknown error, should not occur, but reserved in case
     */
    HARMONY_ERROR_UNKNOWN,
    /**
     * The dynamic library could not be found
     */
    HARMONY_ERROR_DYNAMIC_LIB_NOT_FOUND,
    /**
     * The dynamic library symbol could not be found
     */
    HARMONY_ERROR_DYNAMIC_LIB_SYMBOL_NOT_FOUND,
    /**
     * The requested file could not be found
     */
    HARMONY_ERROR_FILE_NOT_FOUND,
    /**
     * The file could not be read from
     */
    HARMONY_ERROR_FILE_READ_FAILURE,
    /**
     * The file could not be written to
     */
    HARMONY_ERROR_FILE_WRITE_FAILURE,
} HarmonyError;

/**
 * Logs an message error message and crashes the program
 *
 * Parameters
 * - context The Harmony logger context
 * - message The error message to log
 */
void harmony_error(const Harmony *harmony, HarmonyString message);

/**
 * Triggers an error if a condition is not met
 *
 * Parameters
 * - context The Harmony logger context
 * - message The error message to log
 */
#define harmony_require(context, condition) \
    if (!(condition)) { \
        harmony_error(context, harmony_string("Requirement failed: " #condition)); \
    }

/**
 * Triggers an error if a condition is not met, only in debug mode
 *
 * Parameters
 * - context The Harmony logger context
 * - message The error message to log
 */
#define harmony_assert(context, condition) \
    if (!(condition)) { \
        harmony_debug_mode(harmony_error(context, harmony_string("Assertion failed: " #condition))); \
    }

/**
 * Copies memory from one place to another
 *
 * Parameters
 * - dst The destination to copy to, must not be NULL
 * - src The source to copy from, must not be NULL
 * - size The size of the memory to copy
 */
void harmony_mem_copy(void *dst, const void *src, usize size);

/**
 * Sets bytes in memory to a value
 *
 * Parameters
 * - memory The memory to set, must not be NULL
 * - size The size of the memory
 */
void harmony_mem_set(void *memory, u8 value, usize size);

/**
 * Compares equality of two blocks of memory
 *
 * Parameters
 * - a First block of memory, must not be NULL
 * - b Second block of memory, must not be NULL
 * - size The size of the memory
 * Returns
 * - true if the memory is the same
 * - false if the memory is different
 */
bool harmony_mem_equal(void *a, void *b, usize size);

/**
 * Aligns a pointer to an alignment
 *
 * Parameters
 * - mem The value to align
 * - alignment The alignment, must be a multiple of 2 greater than 0
 * Returns
 * - The aligned size
 */
inline usize harmony_mem_align(usize mem, usize alignment) {
    return (mem + alignment - 1) & ~(alignment - 1);
}

/**
 * An arena allocator
 *
 * Allocations are made very quickly, and are not freed individually, instead
 * the whole block is freed at once
 *
 * Note, is not thread safe
 */
typedef struct HarmonyArena{
    /*
     * A pointer to the memory being allocated
     */
    void *data;
    /*
     * The total capacity of the data in bytes
     */
    usize capacity;
    /*
     * The next allocation to be given out
     */
    usize head;
} HarmonyArena;

/**
 * Allocates an arena with capacity
 *
 * Parameters
 * - harmony The harmony context to allocate from
 * - capacity The size of the block to allocate and use
 * Returns
 * - The allocated arena
 */
HarmonyArena harmony_arena_create(const Harmony *harmony, usize capacity);

/**
 * Frees an arena
 *
 * Parameters
 * - harmony The harmony context to free to
 * - arena The arena to destroy
 */
void harmony_arena_destroy(const Harmony *harmony, HarmonyArena *arena);

/**
 * Frees all allocations from an arena
 *
 * Parameters
 * - arena The arena to reset, must not be NULL
 */
void harmony_arena_reset(HarmonyArena *arena);

/**
 * Allocates memory from an arena
 *
 * Allocations are not individually freed, harmony_arena() is
 * called instead to free all allocations at once
 *
 * Parameters
 * - arena The arena to allocate from, must not be NULL
 * - size The size in bytes of the allocation
 * Returns
 * - The allocation if successful
 * - NULL if the allocation exceeds capacity, or size is 0
 */
void *harmony_arena_alloc(HarmonyArena *arena, usize size);

/**
 * Reallocates memory from a arena
 *
 * Simply increases the size if allocation is the most recent allocation
 *
 * Parameters
 * - arena The to allocate from, must not be NULL
 * - allocation The allocation to grow, must be the last allocation made
 * - old_size The original size in bytes of the allocation
 * - new_size The new size in bytes of the allocation
 * Returns
 * - The allocation if successful
 * - NULL if the allocation exceeds capacity
 */
void *harmony_arena_realloc(HarmonyArena *arena, void *allocation, usize old_size, usize new_size);

/**
 * Frees an allocation from a arena
 *
 * Can only deallocate the most recent allocation, otherwise does nothing
 *
 * Parameters
 * - arena The to free from, must not be NULL
 * - allocation The allocation to free, must be the last allocation made
 * - size The size of the allocation
 */
void harmony_arena_free(HarmonyArena *arena, void *allocation, usize size);

/**
 * Sets up an interface to use an arena as a Harmony allocator
 *
 * Parameters
 * - arena The arena to use, must not be NULL
 * Returns
 * - The created interface
 */
inline HarmonyAllocator harmony_arena_allocator(HarmonyArena *arena) {
    return (HarmonyAllocator){
        .data = arena,
        .alloc = (void *(*)(void *, usize))&harmony_arena_alloc,
        .realloc = (void *(*)(void *, void *, usize, usize))&harmony_arena_realloc,
        .free = (void (*)(void *, void *, usize))&harmony_arena_free,
    };
}

/**
 * An object pool
 *
 * Before using the pool, harmony_pool_reset() must be called
 *
 * Note, is not thread safe
 */
typedef struct HarmonyPool {
    /**
     * The memory to use
     */
    void *data;
    /**
     * The size of the memory block in bytes
     */
    usize capacity;
    /**
     * The width of each element in the pool
     *
     * Must be at least 8 to fit the free list indices
     */
    usize item_width;
    /**
     * The offset of the next element in bytes
     */
    usize next_offset;
} HarmonyPool;

/**
 * Creates an object pool with item_count number of items sized item_width
 *
 * Parameters
 * - context The Harmony context to allocate from
 * - item_width The size of each item in bytes
 * - item_count The number of items to allocate
 * Returns
 * - The allocated and initialized pool
 */
HarmonyPool harmony_pool_create(const Harmony *harmony, usize item_width, usize item_count);

/**
 * Frees the object pool's memory 
 *
 * Parameters
 * - harmony The harmony context to free to
 * - pool The pool to destroy, must not be NULL
 */
void harmony_pool_destroy(const Harmony *harmony, HarmonyPool *pool);

/**
 * Resets a pool, freeing all items, and (re)initializing slots
 *
 * Parameters
 * - pool The pool to reset, must not be NULL
 */
void harmony_pool_reset(HarmonyPool *pool);

/**
 * Allocates from a pool
 *
 * The allocation size is the pool's fixed item_size
 *
 * Parameters
 * - pool The allocator to allocate from, must not be NULL
 * Returns
 * - The allocation if successful
 * - NULL if out of slots
 */
void *harmony_pool_alloc(HarmonyPool *pool);

/**
 * Frees an allocation from a pool
 *
 * Parameters
 * - pool The pool to free to, must not be NULL
 * - allocation The allocation to free, must not be NULL
 */
void harmony_pool_free(HarmonyPool *pool, void *allocation);

/**
 * Checks a pool allocator for leaks, double frees, corruptions
 *
 * Parameters
 * - pool The pool to check, must not be NULL
 * Returns
 * - true if all objects have been freed successfully
 * - false if the pool was invalidated, or a leak occurred
 */
bool harmony_pool_is_valid(HarmonyPool *pool);

/**
 * A hash map
 */

// hash map api : TODO

/**
 * A hash set
 */

// hash set api : TODO

/**
 * A mutable string
 */
typedef struct HarmonyStringBuilder {
    const char* data;
    usize length;
    usize capacity;
} HarmonyStringBuilder;

// string builder utilities : TODO

#define PI      3.1415926535897932
#define TAU     6.2831853071795864
#define EULER   2.7182818284590452
#define ROOT2   1.4142135623730951
#define ROOT3   1.7320508075688772

/**
 * A 2D vector
 */
typedef struct Vec2 {
    f32 x, y;
} Vec2;

/**
 * A 3D vector
 */
typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/**
 * A 4D vector
 */
typedef struct Vec4 {
    f32 x, y, z, w;
} Vec4;

/**
 * A 2x2 matrix
 */
typedef struct Mat2 {
    Vec2 x, y;
} Mat2;

/**
 * A 3x3 matrix
 */
typedef struct Mat3 {
    Vec3 x, y, z;
} Mat3;

/**
 * A 4x4 matrix
 */
typedef struct Mat4 {
    Vec4 x, y, z, w;
} Mat4;

/**
 * A complex number
 */
typedef struct Complex {
    f32 r, i;
} Complex;

/**
 * A quaternion
 *
 * r is the real part
 */
typedef struct Quat {
    f32 r, i, j, k;
} Quat;

/**
 * Creates a 2D vector with the given scalar
 *
 * Parameters
 * - scalar The scalar to create the vector with
 * Returns
 * - The created vector
 */
inline Vec2 svec2(f32 scalar) {
    return (Vec2){scalar, scalar};
}

/**
 * Creates a 3D vector with the given scalar
 *
 * Parameters
 * - scalar The scalar to create the vector with
 * Returns
 * - The created vector
 */
inline Vec3 svec3(f32 scalar) {
    return (Vec3){scalar, scalar, scalar};
}

/**
 * Creates a 4D vector with the given scalar
 *
 * Parameters
 * - scalar The scalar to create the vector with
 * Returns
 * - The created vector
 */
inline Vec4 svec4(f32 scalar) {
    return (Vec4){scalar, scalar, scalar, scalar};
}

/**
 * Creates a 2x2 matrix with the given scalar
 *
 * Stores the scalar in the diagonal only
 *
 * Parameters
 * - scalar The scalar to create the matrix with
 * Returns
 * - The created matrix
 */
inline Mat2 smat2(f32 scalar) {
    return (Mat2){
        {scalar, 0.0f},
        {0.0f, scalar},
    };
}

/**
 * Creates a 3x3 matrix with the given scalar
 *
 * Stores the scalar in the diagonal only
 *
 * Parameters
 * - scalar The scalar to create the matrix with
 * Returns
 * - The created matrix
 */
inline Mat3 smat3(f32 scalar) {
    return (Mat3){
        {scalar, 0.0f, 0.0f},
        {0.0f, scalar, 0.0f},
        {0.0f, 0.0f, scalar},
    };
}

/**
 * Creates a 4x4 matrix with the given scalar
 *
 * Stores the scalar in the diagonal only
 *
 * Parameters
 * - scalar The scalar to create the matrix with
 * Returns
 * - The created matrix
 */
inline Mat4 smat4(f32 scalar) {
    return (Mat4){
        {scalar, 0.0f, 0.0f, 0.0f},
        {0.0f, scalar, 0.0f, 0.0f},
        {0.0f, 0.0f, scalar, 0.0f},
        {0.0f, 0.0f, 0.0f, scalar},
    };
}

/**
 * Creates a 3D vector from a 2D vector and 0 for the z component
 *
 * Parameters
 * - lhs The vector to convert
 * Returns
 * - The converted vector
 */
inline Vec3 vec2to3(Vec2 lhs) {
    return (Vec3){lhs.x, lhs.y, 0.0f};
}

/**
 * Creates a 4D vector from a 2D vector and 0 for the z and w components
 *
 * Parameters
 * - lhs The vector to convert
 * Returns
 * - The converted vector
 */
inline Vec4 vec2to4(Vec2 lhs) {
    return (Vec4){lhs.x, lhs.y, 0.0f, 0.0f};
}

/**
 * Creates a 3D vector from a 3D vector and 0 for the w component
 *
 * Parameters
 * - lhs The vector to convert
 * Returns
 * - The converted vector
 */
inline Vec4 vec3to4(Vec3 lhs) {
    return (Vec4){lhs.x, lhs.y, lhs.z, 0.0f};
}

/**
 * Scales a 2x2 matrix to a 3x3 matrix with 1 on the diagonal
 *
 * Parameters
 * - lhs The matrix to convert
 * Returns
 * - The converted matrix
 */
inline Mat3 mat2to3(Mat2 lhs) {
    return (Mat3){
        {lhs.x.x, lhs.x.y, 0.0f},
        {lhs.y.x, lhs.y.y, 0.0f},
        {0.0f,    0.0f,    1.0f},
    };
}

/**
 * Scales a 2x2 matrix to a 4x4 matrix with 1 on the diagonal
 *
 * Parameters
 * - lhs The matrix to convert
 * Returns
 * - The converted matrix
 */
inline Mat4 mat2to4(Mat2 lhs) {
    return (Mat4){
        {lhs.x.x, lhs.x.y, 0.0f, 0.0f},
        {lhs.y.x, lhs.y.y, 0.0f, 0.0f},
        {0.0f,    0.0f,    1.0f, 0.0f},
        {0.0f,    0.0f,    0.0f, 1.0f},
    };
}

/**
 * Scales a 3x3 matrix to a 4x4 matrix with 1 on the diagonal
 *
 * Parameters
 * - lhs The matrix to convert
 * Returns
 * - The converted matrix
 */
inline Mat4 mat3to4(Mat3 lhs) {
    return (Mat4){
        {lhs.x.x, lhs.x.y, lhs.x.z, 0.0f},
        {lhs.y.x, lhs.y.y, lhs.y.z, 0.0f},
        {lhs.z.x, lhs.z.y, lhs.z.z, 0.0f},
        {0.0f,    0.0f,    0.0f,    1.0f},
    };
}

/**
 * Adds two arbitrary size vectors
 *
 * Parameters
 * - size The size of the vectors
 * - dst The destination vector
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 */
inline void vadd(u32 size, f32* dst, f32* lhs, f32* rhs){
    for (u32 i = 0; i < size; ++i) {
        dst[i] = lhs[i] + rhs[i];
    }
}

/**
 * Adds two 2D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The added vector
 */
inline Vec2 vadd2(Vec2 lhs, Vec2 rhs){
    return (Vec2){lhs.x + rhs.x, lhs.y + rhs.y};
}

/**
 * Adds two 3D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The added vector
 */
inline Vec3 vadd3(Vec3 lhs, Vec3 rhs){
    return (Vec3){lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

/**
 * Adds two 4D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The added vector
 */
inline Vec4 vadd4(Vec4 lhs, Vec4 rhs){
    return (Vec4){lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

/**
 * Subtracts two arbitrary size vectors
 *
 * Parameters
 * - size The size of the vectors
 * - dst The destination vector
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 */
inline void vsub(u32 size, f32* dst, f32* lhs, f32* rhs){
    for (u32 i = 0; i < size; ++i) {
        dst[i] = lhs[i] - rhs[i];
    }
}

/**
 * Subtracts two 2D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The subtracted vector
 */
inline Vec2 vsub2(Vec2 lhs, Vec2 rhs){
    return (Vec2){lhs.x - rhs.x, lhs.y - rhs.y};
}

/**
 * Subtracts two 3D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The subtracted vector
 */
inline Vec3 vsub3(Vec3 lhs, Vec3 rhs){
    return (Vec3){lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

/**
 * Subtracts two 4D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The subtracted vector
 */
inline Vec4 vsub4(Vec4 lhs, Vec4 rhs){
    return (Vec4){lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

/**
 * Multiplies pairwise two arbitrary size vectors
 *
 * Parameters
 * - size The size of the vectors
 * - dst The destination vector
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 */
inline void vmul(u32 size, f32* dst, f32* lhs, f32* rhs){
    for (u32 i = 0; i < size; ++i) {
        dst[i] = lhs[i] * rhs[i];
    }
}

/**
 * Multiplies pairwise two 2D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The multiplied vector
 */
inline Vec2 vmul2(Vec2 lhs, Vec2 rhs){
    return (Vec2){lhs.x * rhs.x, lhs.y * rhs.y};
}

/**
 * Multiplies pairwise two 3D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The multiplied vector
 */
inline Vec3 vmul3(Vec3 lhs, Vec3 rhs){
    return (Vec3){lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}

/**
 * Multiplies pairwise two 4D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The multiplied vector
 */
inline Vec4 vmul4(Vec4 lhs, Vec4 rhs){
    return (Vec4){lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
}

/**
 * Multiplies a scalar and a vector
 *
 * Parameters
 * - size The size of the vector
 * - dst The destination vector
 * - scalar The scalar to multiply with
 * - vec The vector to multiply with
 */
inline void svmul(u32 size, f32* dst, f32 scalar, f32* vec) {
    for (u32 i = 0; i < size; ++i) {
        dst[i] = scalar * vec[i];
    }
}

/**
 * Multiplies a scalar and a 2D vector
 *
 * Parameters
 * - scalar The scalar to multiply with
 * - vec The vector to multiply with
 * Returns
 * - The multiplied vector
 */
inline Vec2 svmul2(f32 scalar, Vec2 vec) {
    return (Vec2){scalar * vec.x, scalar * vec.y};
}

/**
 * Multiplies a scalar and a 3D vector
 *
 * Parameters
 * - scalar The scalar to multiply with
 * - vec The vector to multiply with
 * Returns
 * - The multiplied vector
 */
inline Vec3 svmul3(f32 scalar, Vec3 vec) {
    return (Vec3){scalar * vec.x, scalar * vec.y, scalar * vec.z};
}

/**
 * Multiplies a scalar and a 4D vector
 *
 * Parameters
 * - scalar The scalar to multiply with
 * - vec The vector to multiply with
 * Returns
 * - The multiplied vector
 */
inline Vec4 svmul4(f32 scalar, Vec4 vec) {
    return (Vec4){scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w};
}

/**
 * Divides pairwise two arbitrary size vectors
 *
 * Parameters
 * - size The size of the vectors
 * - dst The destination vector
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 */
inline void vdiv(u32 size, f32* dst, f32* lhs, f32* rhs){
    for (u32 i = 0; i < size; ++i) {
        dst[i] = lhs[i] / rhs[i];
    }
}

/**
 * Divides pairwise two 2D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The divided vector
 */
inline Vec2 vdiv2(Vec2 lhs, Vec2 rhs){
    return (Vec2){lhs.x / rhs.x, lhs.y / rhs.y};
}

/**
 * Divides pairwise two 3D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The divided vector
 */
inline Vec3 vdiv3(Vec3 lhs, Vec3 rhs){
    return (Vec3){lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
}

/**
 * Divides pairwise two 4D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The divided vector
 */
inline Vec4 vdiv4(Vec4 lhs, Vec4 rhs){
    return (Vec4){lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w};
}

/**
 * Divides a vector by a scalar
 *
 * Parameters
 * - size The size of the vector
 * - dst The destination vector
 * - scalar The scalar to divide by
 * - vec The vector to divide
 */
inline void svdiv(u32 size, f32* dst, f32 scalar, f32* vec) {
    for (u32 i = 0; i < size; ++i) {
        dst[i] = scalar / vec[i];
    }
}

/**
 * Divides a 2D vector by a scalar
 *
 * Parameters
 * - scalar The scalar to divide by
 * - vec The vector to divide
 * Returns
 * - The divided vector
 */
inline Vec2 svdiv2(f32 scalar, Vec2 vec) {
    return (Vec2){scalar / vec.x, scalar / vec.y};
}

/**
 * Divides a 3D vector by a scalar
 *
 * Parameters
 * - scalar The scalar to divide by
 * - vec The vector to divide
 * Returns
 * - The divided vector
 */
inline Vec3 svdiv3(f32 scalar, Vec3 vec) {
    return (Vec3){scalar / vec.x, scalar / vec.y, scalar / vec.z};
}

/**
 * Divides a 4D vector by a scalar
 *
 * Parameters
 * - scalar The scalar to divide by
 * - vec The vector to divide
 * Returns
 * - The divided vector
 */
inline Vec4 svdiv4(f32 scalar, Vec4 vec) {
    return (Vec4){scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w};
}

/**
 * Computes the dot product of two arbitrary size vectors
 *
 * Parameters
 * - size The size of the vectors
 * - dst The destination vector
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 */
inline void vdot(u32 size, f32* dst, f32* lhs, f32* rhs){
    *dst = 0.0f;
    for (u32 i = 0; i < size; ++i) {
        *dst += lhs[i] * rhs[i];
    }
}

/**
 * Computes the dot product of two 2D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The dot product
 */
inline float vdot2(Vec2 lhs, Vec2 rhs){
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/**
 * Computes the dot product of two 3D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The dot product
 */
inline float vdot3(Vec3 lhs, Vec3 rhs){
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

/**
 * Computes the dot product of two 4D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The dot product
 */
inline float vdot4(Vec4 lhs, Vec4 rhs){
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/**
 * Computes the length of a vector
 *
 * Parameters
 * - size The size of the vector
 * - dst The destination vector
 * - vec The vector to compute the length of
 */
inline void vlen(u32 size, f32* dst, f32* lhs){
    vdot(size, dst, lhs, lhs);
    *dst = sqrtf(*dst);
}

/**
 * Computes the length of a 2D vector
 *
 * Parameters
 * - lhs The vector to compute the length of
 * Returns
 * - The length of the vector
 */
inline float vlen2(Vec2 vec){
    return sqrtf(vdot2(vec, vec));
}

/**
 * Computes the length of a 3D vector
 *
 * Parameters
 * - lhs The vector to compute the length of
 * Returns
 * - The length of the vector
 */
inline float vlen3(Vec3 vec){
    return sqrtf(vdot3(vec, vec));
}

/**
 * Computes the length of a 4D vector
 *
 * Parameters
 * - lhs The vector to compute the length of
 * Returns
 * - The length of the vector
 */
inline float vlen4(Vec4 vec){
    return sqrtf(vdot4(vec, vec));
}

/**
 * Normalizes a vector
 *
 * Parameters
 * - size The size of the vector
 * - dst The destination vector
 * - vec The vector to normalize
 */
inline void vnorm(u32 size, f32* dst, f32* vec){
    f32 len;
    vlen(size, &len, vec);
    for (u32 i = 0; i < size; ++i) {
        dst[i] = vec[i] / len;
    }
}


/**
 * Normalizes a 2D vector
 *
 * Parameters
 * - vec The vector to normalize
 * Returns
 * - The normalized vector
 */
inline Vec2 vnorm2(Vec2 vec){
    f32 len = vlen2(vec);
    return (Vec2){vec.x / len, vec.y / len};
}

/**
 * Normalizes a 3D vector
 *
 * Parameters
 * - vec The vector to normalize
 * Returns
 * - The normalized vector
 */
inline Vec3 vnorm3(Vec3 vec){
    f32 len = vlen3(vec);
    return (Vec3){vec.x / len, vec.y / len, vec.z / len};
}

/**
 * Normalizes a 4D vector
 *
 * Parameters
 * - vec The vector to normalize
 * Returns
 * - The normalized vector
 */
inline Vec4 vnorm4(Vec4 vec){
    f32 len = vlen4(vec);
    return (Vec4){vec.x / len, vec.y / len, vec.z / len, vec.w / len};
}

/**
 * Computes the cross product of two 3D vectors
 *
 * Parameters
 * - dst The destination vector
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 */
inline void vcross(f32* dst, f32* lhs, f32* rhs) {
    dst[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    dst[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    dst[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

/**
 * Computes the cross product of two 3D vectors
 *
 * Parameters
 * - lhs The left-hand side vector
 * - rhs The right-hand side vector
 * Returns
 * - The cross product
 */
inline Vec3 vcross3(Vec3 lhs, Vec3 rhs){
    return (Vec3){lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x};
}

/**
 * Adds two arbitrary size matrices
 *
 * Parameters
 * - width The width of the matrices
 * - height The height of the matrices
 * - dst The destination matrix
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 */
inline void madd(u32 width, u32 height, f32* dst, f32* lhs, f32* rhs){
    for (u32 i = 0; i < width; ++i) {
        for (u32 j = 0; j < height; ++j) {
            dst[i * width + j] = lhs[i * width + j] + rhs[i * width + j];
        }
    }
}

/**
 * Adds two 2x2 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The added matrix
 */
inline Mat2 madd2(Mat2 lhs, Mat2 rhs){
    Mat2 result;
    madd(2, 2, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Adds two 3x3 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The added matrix
 */
inline Mat3 madd3(Mat3 lhs, Mat3 rhs){
    Mat3 result;
    madd(3, 3, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
    return result;
}
/**
 * Adds two 4x4 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The added matrix
 */
inline Mat4 madd4(Mat4 lhs, Mat4 rhs){
    Mat4 result;
    madd(4, 4, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Subtracts two arbitrary size matrices
 *
 * Parameters
 * - width The width of the matrices
 * - height The height of the matrices
 * - dst The destination matrix
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 */
inline void msub(u32 width, u32 height, f32* dst, f32* lhs, f32* rhs){
    for (u32 i = 0; i < width; ++i) {
        for (u32 j = 0; j < height; ++j) {
            dst[i * width + j] = lhs[i * width + j] - rhs[i * width + j];
        }
    }
}

/**
 * Subtracts two 2x2 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The subtracted matrix
 */
inline Mat2 msub2(Mat2 lhs, Mat2 rhs){
    Mat2 result;
    msub(2, 2, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Subtracts two 3x3 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The subtracted matrix
 */
inline Mat3 msub3(Mat3 lhs, Mat3 rhs){
    Mat3 result;
    msub(3, 3, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
    return result;
}


/**
 * Subtracts two 4x4 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The subtracted matrix
 */
inline Mat4 msub4(Mat4 lhs, Mat4 rhs){
    Mat4 result;
    msub(4, 4, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Multiplies two arbitrary size matrices
 *
 * Parameters
 * - dst The destination matrix
 * - wl The width of the left-hand side matrix
 * - hl The height of the left-hand side matrix
 * - lhs The left-hand side matrix
 * - wr The width of the right-hand side matrix
 * - hr The height of the right-hand side matrix
 * - rhs The right-hand side matrix
 */
inline void mmul(f32* dst, u32 wl, u32 hl, f32* lhs, u32 wr, u32 hr, f32* rhs) {
    (void)hr;
    for (u32 i = 0; i < wl; ++i) {
        for (u32 j = 0; j < wr; ++j) {
            dst[i * wl + j] = 0.0f;
            for (u32 k = 0; k < hl; ++k) {
                dst[i * wl + j] += lhs[k * wl + j] * rhs[i * wr + k];
            }
        }
    }
}

/**
 * Multiplies two 2x2 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The multiplied matrix
 */
inline Mat2 mmul2(Mat2 lhs, Mat2 rhs){
    Mat2 result;
    mmul((f32*)&result, 2, 2, (f32*)&lhs, 2, 2, (f32*)&rhs);
    return result;
}

/**
 * Multiplies two 3x3 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The multiplied matrix
 */
inline Mat3 mmul3(Mat3 lhs, Mat3 rhs){
    Mat3 result;
    mmul((f32*)&result, 3, 3, (f32*)&lhs, 3, 3, (f32*)&rhs);
    return result;
}

/**
 * Multiplies two 4x4 matrices
 *
 * Parameters
 * - lhs The left-hand side matrix
 * - rhs The right-hand side matrix
 * Returns
 * - The multiplied matrix
 */
inline Mat4 mmul4(Mat4 lhs, Mat4 rhs){
    Mat4 result;
    mmul((f32*)&result, 4, 4, (f32*)&lhs, 4, 4, (f32*)&rhs);
    return result;
}

/**
 * Multiplies a matrix and a vector
 *
 * Parameters
 * - dst The destination vector
 * - width The width of the matrix
 * - height The height of the matrix
 * - mat The matrix to multiply with
 * - vec The vector to multiply with
 */
inline void mvmul(f32* dst, u32 width, u32 height, f32* mat, f32* vec) {
    for (u32 i = 0; i < height; ++i) {
        dst[i] = 0.0f;
        for (u32 j = 0; j < width; ++j) {
            dst[i] += mat[j * width + i] * vec[j];
        }
    }
}

/**
 * Multiplies a 2x2 matrix and a 2D vector
 *
 * Parameters
 * - lhs The matrix to multiply with
 * - rhs The vector to multiply with
 * Returns
 * - The multiplied vector
 */
inline Vec2 mvmul2(Mat2 lhs, Vec2 rhs) {
    Vec2 result;
    mvmul((f32*)&result, 2, 2, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Multiplies a 3x3 matrix and a 3D vector
 *
 * Parameters
 * - lhs The matrix to multiply with
 * - rhs The vector to multiply with
 * Returns
 * - The multiplied vector
 */
inline Vec3 mvmul3(Mat3 lhs, Vec3 rhs) {
    Vec3 result;
    mvmul((f32*)&result, 3, 3, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Multiplies a 4x4 matrix and a 4D vector
 *
 * Parameters
 * - lhs The matrix to multiply with
 * - rhs The vector to multiply with
 * Returns
 * - The multiplied vector
 */
inline Vec4 mvmul4(Mat4 lhs, Vec4 rhs) {
    Vec4 result;
    mvmul((f32*)&result, 4, 4, (f32*)&lhs, (f32*)&rhs);
    return result;
}

/**
 * Adds two complex numbers
 *
 * Parameters
 * - lhs The left-hand side complex number
 * - rhs The right-hand side complex number
 * Returns
 * - The added complex number
 */
inline Complex cadd(Complex lhs, Complex rhs){
    return (Complex){lhs.r + rhs.r, lhs.i + rhs.i};
}

/**
 * Subtracts two complex numbers
 *
 * Parameters
 * - lhs The left-hand side complex number
 * - rhs The right-hand side complex number
 * Returns
 * - The subtracted complex number
 */
inline Complex csub(Complex lhs, Complex rhs){
    return (Complex){lhs.r - rhs.r, lhs.i - rhs.i};
}

/**
 * Multiplies two complex numbers
 *
 * Parameters
 * - lhs The left-hand side complex number
 * - rhs The right-hand side complex number
 * Returns
 * - The multiplied complex number
 */
inline Complex cmul(Complex lhs, Complex rhs){
    return (Complex){lhs.r * rhs.r - lhs.i * rhs.i, lhs.r * rhs.i + lhs.i * rhs.r};
}

/**
 * Adds two quaternions
 *
 * Parameters
 * - lhs The left-hand side quaternion
 * - rhs The right-hand side quaternion
 * Returns
 * - The added quaternion
 */
inline Quat qadd(Quat lhs, Quat rhs){
    return (Quat){lhs.r + rhs.r, lhs.i + rhs.i, lhs.j + rhs.j, lhs.k + rhs.k};
}

/**
 * Subtracts two quaternions
 *
 * Parameters
 * - lhs The left-hand side quaternion
 * - rhs The right-hand side quaternion
 * Returns
 * - The subtracted quaternion
 */
inline Quat qsub(Quat lhs, Quat rhs){
    return (Quat){lhs.r - rhs.r, lhs.i - rhs.i, lhs.j - rhs.j, lhs.k - rhs.k};
}

/**
 * Multiplies two quaternions
 *
 * Parameters
 * - lhs The left-hand side quaternion
 * - rhs The right-hand side quaternion
 * Returns
 * - The multiplied quaternion
 */
inline Quat qmul(Quat lhs, Quat rhs){
    return (Quat){
        lhs.r * rhs.r - lhs.i * rhs.i - lhs.j * rhs.j - lhs.k * rhs.k,
        lhs.r * rhs.i + lhs.i * rhs.r + lhs.j * rhs.k - lhs.k * rhs.j,
        lhs.r * rhs.j - lhs.i * rhs.k + lhs.j * rhs.r + lhs.k * rhs.i,
        lhs.r * rhs.k + lhs.i * rhs.j - lhs.j * rhs.i + lhs.k * rhs.r,
    };
}

/**
 * Computes the conjugate of a quaternion
 *
 * Parameters
 * - quat The quaternion to compute the conjugate of
 * Returns
 * - The conjugate of the quaternion
 */
inline Quat qconj(Quat quat) {
    return (Quat){quat.r, -quat.i, -quat.j, -quat.k};
}

/**
 * Creates a rotation quaternion from an axis and angle
 *
 * Parameters
 * - axis The axis of the rotation
 * - angle The angle of the rotation
 * Returns
 * - The created quaternion
 */
inline Quat axis_angle(Vec3 axis, f32 angle) {
    f32 half_angle = angle / 2.0f;
    f32 sin_half_angle = sinf(half_angle);
    return (Quat){
        cosf(half_angle),
        axis.x * sin_half_angle,
        axis.y * sin_half_angle,
        axis.z * sin_half_angle,
    };
}

/**
 * Rotates a 3D vector using a quaternion
 *
 * Parameters
 * - lhs The quaternion to rotate with
 * - rhs The vector to rotate
 * Returns
 * - The rotated vector
 */
inline Vec3 rotate_vec3(Quat lhs, Vec3 rhs) {
    Quat q = qmul(lhs, qmul((Quat){0.0f, rhs.x, rhs.y, rhs.z}, qconj(lhs)));
    return (Vec3){q.i, q.j, q.k};
}

/**
 * Rotates a 3x3 matrix using a quaternion
 *
 * Parameters
 * - lhs The quaternion to rotate with
 * - rhs The matrix to rotate
 * Returns
 * - The rotated matrix
 */
inline Mat3 rotate_mat3(Quat lhs, Mat3 rhs) {
    return (Mat3){
        rotate_vec3(lhs, rhs.x),
        rotate_vec3(lhs, rhs.y),
        rotate_vec3(lhs, rhs.z),
    };
}

/**
 * Creates a model matrix for 2D graphics
 *
 * Parameters
 * - position The position of the model
 * - scale The scale of the model
 * - rotation The rotation of the model
 * Returns
 * - The created matrix
 */
Mat4 harmony_model_matrix_2d(Vec3 position, Vec2 scale, f32 rotation);

/**
 * Creates a model matrix for 3D graphics
 *
 * Parameters
 * - position The position of the model
 * - scale The scale of the model
 * - rotation The rotation of the model
 * Returns
 * - The created matrix
 */
Mat4 harmony_model_matrix_3d(Vec3 position, Vec3 scale, Quat rotation);

/**
 * Creates a view matrix
 *
 * Parameters
 * - position The position of the camera
 * - zoom The zoom of the camera
 * - rotation The rotation of the camera
 * Returns
 * - The created matrix
 */
Mat4 harmony_view_matrix(Vec3 position, f32 zoom, Quat rotation);

/**
 * Creates an orthographic projection matrix
 *
 * Parameters
 * - left The left-hand side of the view frustum
 * - right The right-hand side of the view frustum
 * - top The top of the view frustum
 * - bottom The bottom of the view frustum
 * - near The near plane of the view frustum
 * - far The far plane of the view frustum
 * Returns
 * - The created matrix
 */
Mat4 harmony_projection_matrix_orthographic(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);

/**
 * Creates a perspective projection matrix
 *
 * Parameters
 * - fov The field of view of the projection in radians
 * - aspect The aspect ratio of the projection
 * - near The near plane of the projection, must be greater than 0.0f
 * - far The far plane of the projection, must be greater than near
 * Returns
 * - The created matrix
 */
Mat4 harmony_projection_matrix_perspective(f32 fov, f32 aspect, f32 near, f32 far);

// random number generators : TODO

// hash functions : TODO

#endif // HARMONY_CORE_H
