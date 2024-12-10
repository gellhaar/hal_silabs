/*
 * Copyright (c) 2024 Jonny Gellhaar
 * 
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>

/**
 * @brief Allocate memory from the heap.
 * @param size Number of unaligned bytes to allocate.
 * @return Pointer to allocated memory, or NULL if allocation failed.
 */
void *sl_malloc(size_t size);

/**
 * @brief Return allocated memory to the heap.
 * @param ptr Memory buffer previously allocated with sl_malloc().
 */
void sl_free(void * ptr);
