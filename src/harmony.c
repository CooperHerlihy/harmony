#include "harmony.h"

#include <stdlib.h>

HarmonyString harmony_string(const char* str) {
    if (str == NULL)
        return (HarmonyString){0};

    HarmonyString hstr = {
        .data = str,
        .length = 0,
    };
    for (; hstr.length < 4096;) {
        if (str[hstr.length] == '\0')
            return hstr;
        ++hstr.length;
    }
    return hstr;
}

void harmony_error(const Harmony *harmony, HarmonyString message) {
    if (message.data != NULL)
        harmony_log_error(harmony, message);
    abort(); // replace with non cstdlib version : TODO
}

void harmony_mem_copy(void *dst, const void *src, usize size) {
    if ((usize)dst - (usize)src < size) {
        for (usize i = size - 1; i < size; --i) {
            ((u8 *)dst)[i] = ((u8 *)src)[i];
        }
    } else {
        for (usize i = 0; i < size; ++i) {
            ((u8 *)dst)[i] = ((u8 *)src)[i];
        }
    }
}

void harmony_mem_set(void *memory, u8 value, usize size) {
    for (usize i = 0; i < size; ++i) {
        ((u8 *)memory)[i] = value;
    }
}

bool harmony_mem_equal(void *a, void *b, usize size) {
    for (usize i = 0; i < size; ++i) {
        if (((u8 *)a)[i] != ((u8 *)b)[i])
            return false;
    }
    return true;
}

HarmonyArena harmony_arena_create(const Harmony *harmony, usize capacity) {
    return (HarmonyArena){
        .data = harmony_alloc(harmony, capacity),
    };
}

void harmony_arena_destroy(const Harmony *harmony, HarmonyArena *arena) {
    harmony_free(harmony, arena->data, arena->capacity);
}

void harmony_arena_reset(HarmonyArena *arena) {
    arena->head = 0;
}

void *harmony_arena_alloc(HarmonyArena *allocator, usize size) {
    if (size == 0)
        return NULL;

    usize new_head = allocator->head + harmony_mem_align(size, HARMONY_MAX_ALIGNMENT);
    if (new_head > allocator->capacity)
        return NULL;

    void *allocation = (u8 *)allocator->data + allocator->head;
    allocator->head = new_head;
    return allocation;
}

void *harmony_arena_realloc(HarmonyArena *arena, void *allocation, usize old_size, usize new_size) {
    if (new_size == 0) {
        arena->head = (usize)allocation - (usize)arena->data;
        return NULL;
    }

    if ((usize)allocation + harmony_mem_align(old_size, HARMONY_MAX_ALIGNMENT) == arena->head) {
        usize new_head = (usize)allocation
                       - (usize)arena->data
                       + harmony_mem_align(new_size, HARMONY_MAX_ALIGNMENT);
        if (new_head > arena->capacity)
            return NULL;
        arena->head = new_head;
        return allocation;
    }

    void *new_allocation = harmony_arena_alloc(arena, new_size);
    harmony_mem_copy(new_allocation, allocation, old_size);
    return new_allocation;
}

void harmony_arena_free(HarmonyArena *arena, void *allocation, usize size) {
    if ((usize)allocation + harmony_mem_align(size, HARMONY_MAX_ALIGNMENT) == arena->head)
        arena->head = (usize)allocation - (usize)arena->data;
}

HarmonyPool harmony_pool_create(const Harmony *harmony, usize item_width, usize item_count) {
    item_width = max(item_width, 8);
    HarmonyPool pool = {
        .data = harmony_alloc(harmony, item_width * item_count),
        .capacity = item_width * item_count,
        .item_width = item_width,
        .next_offset = 0,
    };
    harmony_pool_reset(&pool);
    return pool;
}

void harmony_pool_destroy(const Harmony *harmony, HarmonyPool *pool) {
    harmony_free(harmony, pool->data, pool->capacity);
}

void harmony_pool_reset(HarmonyPool *pool) {
    pool->next_offset = 0;
    for (usize i = 0; i < pool->capacity; i += pool->item_width) {
        *(u64 *)((u8 *)pool->data + i * pool->capacity) = (u64)i + 1;
    }
}

void *harmony_pool_alloc(HarmonyPool *pool) {
    if (pool->next_offset > pool->capacity - pool->item_width)
        return NULL;

    void *allocation = (u8 *)pool->data + pool->next_offset;
    pool->next_offset = *(u64 *)allocation;

    return allocation;
}

void harmony_pool_free(HarmonyPool *pool, void *allocation) {
    *(u64 *)allocation = (u64)pool->next_offset;
    pool->next_offset = (usize)allocation - (usize)pool->data;
}

bool harmony_pool_is_valid(HarmonyPool *pool) {
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

Mat4 harmony_model_matrix_2d(Vec3 position, Vec2 scale, f32 rotation) {
    Mat2 m2 = smat2(1.0f);
    m2.x.x = scale.x;
    m2.y.y = scale.y;
    m2 = mmul2((Mat2){
        {cosf(rotation), sinf(rotation)},
        {-sinf(rotation), cosf(rotation)},
    }, m2);
    Mat4 m4 = mat2to4(m2);
    m4.w.x = position.x;
    m4.w.y = position.y;
    m4.w.z = position.z;
    return m4;
}

Mat4 harmony_model_matrix_3d(Vec3 position, Vec3 scale, Quat rotation) {
    Mat3 m3 = smat3(1.0f);
    m3.x.x = scale.x;
    m3.y.y = scale.y;
    m3.z.z = scale.z;
    m3 = rotate_mat3(rotation, m3);
    Mat4 m4 = mat3to4(m3);
    m4.w.x = position.x;
    m4.w.y = position.y;
    m4.w.z = position.z;
    return m4;
}

Mat4 harmony_view_matrix(Vec3 position, f32 zoom, Quat rotation) {
    Mat4 rot = mat3to4(rotate_mat3(qconj(rotation), smat3(1.0f)));
    Mat4 pos = smat4(1.0f);
    pos.x.x = zoom;
    pos.y.y = zoom;
    pos.w.x = -position.x;
    pos.w.y = -position.y;
    pos.w.z = -position.z;
    return mmul4(rot, pos);
}

Mat4 harmony_projection_matrix_orthographic(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
    return (Mat4){
        {2.0f / (right - left), 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f / (bottom - top), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f / (far - near), 0.0f},
        {-(right + left) / (right - left), -(bottom + top) / (bottom - top), -(near) / (far - near), 1.0f},
    };
}

Mat4 harmony_projection_matrix_perspective(f32 fov, f32 aspect, f32 near, f32 far) {
    f32 scale = 1 / tanf(fov / 2);
    return (Mat4){
        {scale / aspect, 0.0f, 0.0f, 0.0f},
        {0.0f, scale, 0.0f, 0.0f},
        {0.0f, 0.0f, far / (far - near), 1.0f},
        {0.0f, 0.0f, -(far * near) / (far - near), 0.0f},
    };
}

