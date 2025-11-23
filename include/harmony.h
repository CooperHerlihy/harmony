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

#ifndef HARMONY_H
#define HARMONY_H

#include <float.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

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

#ifdef NDEBUG

/**
 * Runs code only in debug mode
 */
#define harmony_debug_mode(code)

#else // NDEBUG

/**
 * Runs code only in debug mode
 */
#define harmony_debug_mode(code) code

#endif // NDEBUG

#ifndef HARMONY_LOG_FUNCTION

/**
 * The log function used by Harmony, can be defined to either nothing, or to
 * something else
 *
 * Parameters
 * - first The format string, following printf conventions
 * - rest The values to insert into the format string
 */
#define HARMONY_LOG_FUNCTION(...) (void)fprintf(stderr, __VA_ARGS__)

#endif // HARMONY_LOG_FUNCTION

/**
 * A macro to log debug messages, only in debug mode
 */
#define harmony_log_debug(...) harmony_debug_mode(HARMONY_LOG_FUNCTION("Harmony Debug: " __VA_ARGS__))

/**
 * A macro to log info messages
 */
#define harmony_log_info(...) HARMONY_LOG_FUNCTION("Harmony Info: " __VA_ARGS__)

/**
 * A macro to log warning messages
 */
#define harmony_log_warning(...) HARMONY_LOG_FUNCTION("Harmony Warning: " __VA_ARGS__)

/**
 * A macro to log error messages
 */
#define harmony_log_error(...) HARMONY_LOG_FUNCTION("Harmony Error: " __VA_ARGS__)

/**
 * A macro to log error messages and abort the program
 */
#define harmony_error(...) { \
    harmony_log_error(__VA_ARGS__); \
    abort(); \
}

/**
 * A macro to assert a condition, then log and crash on failure
 */
#define harmony_assert(condition) harmony_debug_mode({ \
    if (!(condition)) { \
        HARMONY_LOG_FUNCTION("Harmony Assertion Failed: " __FILE__ ":%d " #condition, __LINE__); \
        abort(); \
    } \
})

/**
 * A high precision clock for timers and game deltas
 */
typedef struct HarmonyClock {
    struct timespec time;
} HarmonyClock;

/**
 * Resets the clock and gets the delta since the last tick in seconds
 *
 * Parameters
 * - clock The clock to tick, must not be NULL
 * Returns
 * - Seconds since last tick
 */
inline f64 harmony_clock_tick(HarmonyClock *hclock) {
    f64 prev = (f64)hclock->time.tv_sec + (f64)hclock->time.tv_nsec / 1.0e9;
    timespec_get(&hclock->time, TIME_UTC);
    return ((f64)hclock->time.tv_sec + (f64)hclock->time.tv_nsec / 1.0e9) - prev;
}

#ifdef __unix__

#include <dlfcn.h>

/**
 * Opens a dynamic library at the path
 *
 * Parameters
 * - path The path to the library, must not be NULL
 * Returns
 * - An opaque pointer to the library
 * - NULL on failure
 */
inline void *harmony_dynamic_lib_open(const char *path) {
    harmony_assert(path != NULL);

    void *lib = dlopen(path, RTLD_LAZY);
    if (lib == NULL)
        harmony_log_warning("Could not open dynamic lib %s: %s", path, dlerror());

    return lib;
}

/**
 * Closes a dynamic library
 *
 * Parameters
 * - lib The dynamic library to close, must not be NULL
 */
inline void harmony_dynamic_lib_close(void *lib) {
    harmony_assert(lib != NULL);
    dlclose(lib);
}

/**
 * Loads a symbol from a dynamic library
 *
 * Parameters
 * - lib The library to load from, must not be NULL
 * - symbol The name of the symbol, must not be NULL
 * Returns
 * - A pointer to the loaded symbol
 * - NULL on failure
 */
inline void *harmony_dynamic_lib_load_symbol(void *lib, const char *symbol) {
    harmony_assert(lib != NULL);
    harmony_assert(symbol != NULL);

    void *sym = dlsym(lib, symbol);
    if (sym == NULL)
        harmony_log_warning("Could not load symbol %s from dynamic lib: %s", symbol, dlerror());

    return sym;
}

#else // __unix__

#error "harmony dynamic library loading only implemented for unix"

#endif

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
inline void *harmony_alloc(const HarmonyAllocator *allocator, usize size) {
    harmony_assert(allocator != NULL);
    return allocator->alloc(allocator->data, size);
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
inline void *harmony_realloc(const HarmonyAllocator *allocator, void *allocation, usize old_size, usize new_size) {
    harmony_assert(allocator != NULL);
    return allocator->realloc(allocator->data, allocation, old_size, new_size);
}

/**
 * A convenience to call alloc from Harmony context
 *
 * Parameters
 * - context The Harmony constext, must not be NULL
 * - allocation The allocation to resize
 * - size The size in bytes of the allocation
 */
inline void harmony_free(const HarmonyAllocator *allocator, void *allocation, usize size) {
    harmony_assert(allocator != NULL);
    allocator->free(allocator->data, allocation, size);
}

/**
 * Calls malloc, checking for NULL in debug mode
 *
 * Parameters
 * - dummy A dummy value to fit the HarmonyAllocator interface
 * - size The size of the allocation in bytes
 * Returns
 * - The allocated memory
 */
inline void *harmony_default_alloc(void *dummy, usize size) {
    (void)dummy;
    void *allocation = malloc(size);
    harmony_assert(allocation != NULL);
    return allocation;
}

/**
 * Calls realloc, checking for NULL in debug mode
 *
 * Parameters
 * - dummy A dummy value to fit the HarmonyAllocator interface
 * - allocation The allocation to resize
 * - old_size The size of the original allocation in bytes
 * - new_size The size of the new allocation in bytes
 * Returns
 * - The allocated memory
 */
inline void *harmony_default_realloc(void *dummy, void *allocation, usize old_size, usize new_size) {
    (void)dummy;
    (void)old_size;
    void *new_allocation = realloc(allocation, new_size);
    harmony_assert(new_allocation != NULL);
    return new_allocation;
}

/**
 * Calls free
 *
 * Parameters
 * - dummy A dummy value to fit the HarmonyAllocator interface
 * - allocation The allocation to free
 * - size The size of the allocation in bytes
 */
inline void harmony_default_free(void *dummy, void *allocation, usize size) {
    (void)dummy;
    (void)size;
    free(allocation);
}

/**
 * Creates the interface for a HarmonyAllocator using malloc, realloc, and free
 */
inline HarmonyAllocator harmony_default_allocator(void) {
    return (HarmonyAllocator){
        .data = NULL,
        .alloc = (void *(*)(void *, usize))&harmony_default_alloc,
        .realloc = (void *(*)(void *, void *, usize, usize))&harmony_default_realloc,
        .free = (void (*)(void *, void *, usize))&harmony_default_free,
    };
}

/**
 * Aligns a pointer to an alignment
 *
 * Parameters
 * - value The value to align
 * - alignment The alignment, must be a multiple of 2 greater than 0
 * Returns
 * - The aligned size
 */
inline usize harmony_align(usize value, usize alignment) {
    harmony_assert(alignment > 0 && (alignment & (alignment - 1)) == 0);
    return (value + alignment - 1) & ~(alignment - 1);
}

#define PI      3.1415926535897932
#define TAU     6.2831853071795864
#define EULER   2.7182818284590452
#define ROOT2   1.4142135623730951
#define ROOT3   1.7320508075688772

/**
 * Gets the number of elements in a stack allocated array
 *
 * Parameters
 * - array The array to take the count of
 * Returns
 * - The nuymber of elements
 */
#define harmony_countof(array) (sizeof(array) / sizeof((array)[0]))

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
#define harmony_min(a, b) ((a) < (b) ? (a) : (b))

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
#define harmony_max(a, b) ((a) > (b) ? (a) : (b))

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
#define harmony_clamp(x, a, b) harmony_max((a), harmony_min((b), (x)))

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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(vec != NULL);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(vec != NULL);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
inline void vlen(u32 size, f32* dst, f32* vec){
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(vec != NULL);
    vdot(size, dst, vec, vec);
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
    harmony_assert(size > 0);
    harmony_assert(dst != NULL);
    harmony_assert(vec != NULL);
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
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(width > 0);
    harmony_assert(height > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(width > 0);
    harmony_assert(height > 0);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
    harmony_assert(wl > 0);
    harmony_assert(hl > 0);
    harmony_assert(wr > 0);
    harmony_assert(hr > 0);
    harmony_assert(hr == wl);
    harmony_assert(dst != NULL);
    harmony_assert(lhs != NULL);
    harmony_assert(rhs != NULL);
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
inline void mvmul(u32 width, u32 height, f32* dst, f32* mat, f32* vec) {
    harmony_assert(width > 0);
    harmony_assert(height > 0);
    harmony_assert(dst != NULL);
    harmony_assert(mat != NULL);
    harmony_assert(vec != NULL);
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
    mvmul(2, 2, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
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
    mvmul(3, 3, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
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
    mvmul(4, 4, (f32*)&result, (f32*)&lhs, (f32*)&rhs);
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

#endif // HARMONY_H
