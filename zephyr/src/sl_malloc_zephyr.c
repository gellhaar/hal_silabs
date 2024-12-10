/*
 * Copyright (c) 2024 Jonny Gellhaar
 * 
 * SPDX-License-Identifier: Apache-2.0
 */

#if CONFIG_HEAP_MEM_POOL_SIZE > 0

#include <zephyr/kernel.h>
#include <sl_malloc_zephyr.h>

void *sl_malloc(size_t size)
{
	return k_malloc(size);
}

void sl_free(void *ptr)
{
	k_free(ptr);
}

#endif /* CONFIG_HEAP_MEM_POOL_SIZE */
