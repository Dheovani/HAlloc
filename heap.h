#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#ifndef HEAP_CAPACITY
#define HEAP_CAPACITY 64000
#endif // HEAP_CAPACITY

#define MIN_ALLOC_SIZE 1

#define LOG_ERROR_AND_EXIT(msg)	\
	printf(msg);				\
	exit(EXIT_FAILURE);

typedef struct hChunk {
	uintptr_t *start;
	size_t chunkSize;
	struct hChunk *next;
} hChunk;

/**
 * Allocates the memory in the heap
 * @param size_t size - amount of memory allocated from the heap
 * @return void* - pointer to allocated memory
 */
void *hAlloc(size_t size);

/**
 * Deallocates the memory from the heap
 * @param void *ptr - ptr to be deallocated
 */
void hFree(void *ptr);

#endif // _HEAP_H_