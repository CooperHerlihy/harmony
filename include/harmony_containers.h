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

#ifndef HARMONY_CONTAINERS_H
#define HARMONY_CONTAINERS_H

#include "harmony.h"

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
void *harmony_default_alloc(void *dummy, usize size);

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
void *harmony_default_realloc(void *dummy, void *allocation, usize old_size, usize new_size);

/**
 * Calls free
 *
 * Parameters
 * - dummy A dummy value to fit the HarmonyAllocator interface
 * - allocation The allocation to free
 * - size The size of the allocation in bytes
 */
void harmony_default_free(void *dummy, void *allocation, usize size);

/**
 * Creates the interface for a HarmonyAllocator using malloc, realloc, and free
 */
HarmonyAllocator harmony_default_allocator(void);

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
inline HarmonyArena harmony_arena_create(const HarmonyAllocator *allocator, usize capacity) {
    harmony_assert(allocator != NULL);
    return (HarmonyArena){
        .data = harmony_alloc(allocator, capacity),
    };
}


/**
 * Frees an arena's memory
 *
 * Parameters
 * - harmony The harmony context to free to
 * - arena The arena to destroy
 */
inline void harmony_arena_destroy(const HarmonyAllocator *allocator, HarmonyArena *arena) {
    harmony_assert(allocator != NULL);
    harmony_free(allocator, arena->data, arena->capacity);
}

/**
 * Frees all allocations from an arena
 *
 * Parameters
 * - arena The arena to reset, must not be NULL
 */
inline void harmony_arena_reset(HarmonyArena *arena) {
    harmony_assert(arena != NULL);
    arena->head = 0;
}

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
    harmony_assert(arena != NULL);
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
HarmonyPool harmony_pool_create(const HarmonyAllocator *allocator, usize item_width, usize item_count);

/**
 * Frees the object pool's memory 
 *
 * Parameters
 * - harmony The harmony context to free to
 * - pool The pool to destroy, must not be NULL
 */
void harmony_pool_destroy(const HarmonyAllocator *allocator, HarmonyPool *pool);

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
 * Resets a pool, freeing all items, and (re)initializing slots
 *
 * Parameters
 * - pool The pool to reset, must not be NULL
 */
void harmony_pool_reset(HarmonyPool *pool);

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
 * A dynamic array
 */

// dynamic array api : TODO

/**
 * A hash map
 */

// hash map api : TODO

/**
 * A hash set
 */

// hash set api : TODO

#if defined(HARMONY_IMPLEMENTATION_CONTAINERS) || defined(HARMONY_IMPLEMENTATION_ALL)

void *harmony_default_alloc(void *dummy, usize size) {
    (void)dummy;
    void *allocation = malloc(size);
    harmony_assert(allocation != NULL);
    return allocation;
}

void *harmony_default_realloc(void *dummy, void *allocation, usize old_size, usize new_size) {
    (void)dummy;
    (void)old_size;
    void *new_allocation = realloc(allocation, new_size);
    harmony_assert(new_allocation != NULL);
    return new_allocation;
}

void harmony_default_free(void *dummy, void *allocation, usize size) {
    (void)dummy;
    (void)size;
    free(allocation);
}

HarmonyAllocator harmony_default_allocator(void) {
    return (HarmonyAllocator){
        .data = NULL,
        .alloc = (void *(*)(void *, usize))&harmony_default_alloc,
        .realloc = (void *(*)(void *, void *, usize, usize))&harmony_default_realloc,
        .free = (void (*)(void *, void *, usize))&harmony_default_free,
    };
}

void *harmony_arena_alloc(HarmonyArena *arena, usize size) {
    harmony_assert(arena != NULL);
    if (size == 0)
        return NULL;

    usize new_head = arena->head + harmony_align(size, 16);
    if (new_head > arena->capacity)
        return NULL;

    void *allocation = (u8 *)arena->data + arena->head;
    arena->head = new_head;
    return allocation;
}

void *harmony_arena_realloc(HarmonyArena *arena, void *allocation, usize old_size, usize new_size) {
    harmony_assert(arena != NULL);
    if (new_size == 0) {
        arena->head = (usize)allocation - (usize)arena->data;
        return NULL;
    }

    if ((usize)allocation + harmony_align(old_size, 16) == arena->head) {
        usize new_head = (usize)allocation
                       - (usize)arena->data
                       + harmony_align(new_size, 16);
        if (new_head > arena->capacity)
            return NULL;
        arena->head = new_head;
        return allocation;
    }

    void *new_allocation = harmony_arena_alloc(arena, new_size);
    memcpy(new_allocation, allocation, old_size);
    return new_allocation;
}

void harmony_arena_free(HarmonyArena *arena, void *allocation, usize size) {
    harmony_assert(arena != NULL);
    if ((usize)allocation + harmony_align(size, 16) == arena->head)
        arena->head = (usize)allocation - (usize)arena->data;
}

HarmonyPool harmony_pool_create(const HarmonyAllocator *allocator, usize item_width, usize item_count) {
    harmony_assert(allocator != NULL);
    item_width = harmony_max(item_width, 8);
    HarmonyPool pool = {
        .data = harmony_alloc(allocator, item_width * item_count),
        .capacity = item_width * item_count,
        .item_width = item_width,
        .next_offset = 0,
    };
    harmony_pool_reset(&pool);
    return pool;
}

void harmony_pool_destroy(const HarmonyAllocator *allocator, HarmonyPool *pool) {
    harmony_assert(allocator != NULL);
    harmony_free(allocator, pool->data, pool->capacity);
}

void *harmony_pool_alloc(HarmonyPool *pool) {
    harmony_assert(pool != NULL);
    if (pool->next_offset > pool->capacity - pool->item_width)
        return NULL;

    void *allocation = (u8 *)pool->data + pool->next_offset;
    pool->next_offset = *(u64 *)allocation;

    return allocation;
}

void harmony_pool_free(HarmonyPool *pool, void *allocation) {
    harmony_assert(pool != NULL);
    harmony_assert((usize)allocation - (usize)pool->data < pool->capacity);
    *(u64 *)allocation = (u64)pool->next_offset;
    pool->next_offset = (usize)allocation - (usize)pool->data;
}

void harmony_pool_reset(HarmonyPool *pool) {
    harmony_assert(pool != NULL);
    pool->next_offset = 0;
    for (usize i = 0; i < pool->capacity; i += pool->item_width) {
        *(u64 *)((u8 *)pool->data + i * pool->capacity) = (u64)i + 1;
    }
}

bool harmony_pool_is_valid(HarmonyPool *pool) {
    harmony_assert(pool != NULL);
    usize slot_count = (usize)floor((f64)pool->capacity / (f64)pool->item_width);

    for (usize i = 0; i < slot_count; ++i) {
        if (pool->next_offset > pool->capacity - pool->item_width)
            return false;
        pool->next_offset = *((u64 *)pool->data + pool->next_offset);
    }

    if (pool->next_offset > pool->capacity - pool->item_width)
        return true;

    return false;
}

#endif // defined(HARMONY_IMPLEMENTATION_CONTAINERS) || defined(HARMONY_IMPLEMENTATION_ALL)

#endif // HARMONY_CONTAINERS_H
