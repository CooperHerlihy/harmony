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

#ifndef HARMONY_FILES_H
#define HARMONY_FILES_H

#include "harmony.h"

/**
 * Loads a binary file
 *
 * Parameters
 * - allocator Where to allocate the memory from
 * - data A pointer to store the loaded data, must not be NULL
 * - size A pointer to store the size of the loaded data, must not be NULL
 * - path The null terminated path to the file to load, must not be NULL
 * Returns
 * - true if the file was loaded successfully
 * - false if the file was not found or could not be read
 */
bool harmony_file_load_binary(const HarmonyAllocator *allocator, u8** data, usize* size, const char *path);

/**
 * Unloads a binary file
 *
 * Parameters
 * - allocator Where to free the memory from
 * - data The data to unload, or NULL to do nothing
 * - size The size of the data to unload, must be 0 if data is NULL
 */
void harmony_file_unload_binary(const HarmonyAllocator *allocator, u8* data, usize size);

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
bool harmony_file_save_binary(const u8* data, usize size, const char *path);

// text files : TODO
// json files : TODO
// image files : TODO
// 3d model files : TODO
// audio files : TODO

#if defined(HARMONY_IMPLEMENTATION_FILES) || defined(HARMONY_IMPLEMENTATION_ALL)

bool harmony_file_load_binary(const HarmonyAllocator *allocator, u8** data, usize* size, const char *path) {
    harmony_assert(data != NULL);
    harmony_assert(size != NULL);
    harmony_assert(path != NULL);
    *data = NULL;
    *size = 0;

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        harmony_log_warning("Could not find file to read binary: %s", path);
        return false;
    }

    fseek(file, 0, SEEK_END);
    usize file_size = (usize)ftell(file);
    rewind(file);

    u8* file_data = harmony_alloc(allocator, file_size);
    if (fread(file_data, 1, file_size, file) != file_size) {
        fclose(file);
        harmony_log_warning("Failed to read binary from file: %s", path);
        return false;
    }

    *data = file_data;
    *size = file_size;
    fclose(file);
    return true;
}

void harmony_file_unload_binary(const HarmonyAllocator *allocator, u8* data, usize size) {
    if (size == 0)
        harmony_assert(data == NULL);
    harmony_free(allocator, data, size);
}

bool harmony_file_save_binary(const u8* data, usize size, const char *path) {
    harmony_assert(data != NULL);
    harmony_assert(size > 0);
    harmony_assert(path != NULL);

    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        harmony_log_warning("Failed to create file to write binary: %s", path);
        return false;
    }

    if (fwrite(data, 1, size, file) != size) {
        fclose(file);
        harmony_log_warning("Failed to write binary data to file: %s", path);
        return false;
    }

    fclose(file);
    return true;
}

#endif // defined(HARMONY_IMPLEMENTATION_FILES) || defined(HARMONY_IMPLEMENTATION_ALL)

#endif // HARMONY_FILES_H
