#include "heap.h"

static size_t availableMemory = HEAP_CAPACITY;
static uintptr_t heap[HEAP_CAPACITY] = {0};
static hChunk chunks[HEAP_CAPACITY] = {0};
static size_t chunkCount = 0;

// Returns the next avaiable position of the chunks array
static unsigned int getNextChunkPosition()
{
	for (unsigned int i = 0; i < chunkCount; i++) {
		if (chunks[i].chunkSize == 0 && chunks[i].start == NULL && chunks[i].next == NULL) {
			return i;
		}
	}

	return (unsigned int)chunkCount;
}

/**
 * Adds a chunk to our chunks list
 * @param hChunk *chunk - pointer to a constant chunk
 */
static void addChunk(hChunk *chunk)
{
	assert(chunk != NULL && "Cannot add a null pointer!");

	if (availableMemory == HEAP_CAPACITY && chunkCount == 0) {
		chunks[0] = *chunk;
	}
	else {
		const unsigned int pos = getNextChunkPosition();
		chunks[pos] = *chunk;
		hChunk *current = chunks;

		while (current != NULL) {
			if ((current->start - heap) < (chunk->start - heap)) {
				if (current->next == NULL) {
					current->next = chunks + chunkCount;
					break;
				}
				else if ((current->next->start - heap) > (chunk->start - heap)) {
					chunks[pos].next = current->next;
					current->next = chunks + pos;
					break;
				}
			}

			current = current->next;
		}
	}

	chunkCount++;
}

/**
 * Finds the start of the next memory point
 * that is big enough to contain the given size
 * @param size_t size - size to allocate
 * @return uintptr_t* - pointer to the heap memory address
 */
static uintptr_t *getStart(size_t size)
{
	if (availableMemory == HEAP_CAPACITY && chunkCount == 0)
		return heap;

	hChunk *current = chunks;

	while (current != NULL) {
		if (current->next != NULL) {
			// Space gap between chunks in the list
			const size_t gap = (size_t)(current->next->start - current->start);

			if ((gap - current->chunkSize) > size)
				return current->start + size;
			else
				current = current->next;
		}
		else
			return current->start + size;
	}

	LOG_ERROR_AND_EXIT("Not enough avaiable space in the heap!");
}

void *hAlloc(size_t size)
{
	assert((size < availableMemory) &&
		"Entered size exceeds the available memory limit!");

	uintptr_t *start;
	if (size < MIN_ALLOC_SIZE || (start = getStart(size)) == NULL)
		return NULL;

	hChunk chunk = {
		.chunkSize = (int)size,
		.start = start,
		.next = NULL
	};

	addChunk(&chunk);

	availableMemory -= size;

	return chunk.start;
}

void hFree(void *ptr)
{
	hChunk *current = chunks, *previous = chunks;

	while (current != NULL && current->start != ptr) {
		previous = current;
		current = current->next;
	}

	if (current == NULL) {
		LOG_ERROR_AND_EXIT("Value not found in chunks list");
	}

	for (size_t i = 0; i < current->chunkSize; i++) {
		*(current->start + i) = 0;
	}

	previous->next = current->next;
	for (int i = 0; i < chunkCount; i++) {
		if (chunks[i].start == ptr) {
			chunks[i].chunkSize = 0;
			chunks[i].start = NULL;
			chunks[i].next = NULL;
			chunkCount--;
			break;
		}
	}

	availableMemory += current->chunkSize;
}
