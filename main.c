#include <string.h>
#include "heap.h"

int main(int argc, char **argv)
{
	void *ptr = hAlloc(10), *ptr4 = hAlloc(4), *ptr15 = hAlloc(15);
	
	strcpy((char*)ptr, "123456789");
	printf("%s\n", (char*)ptr);
	assert(strcmp(ptr, "123456789") == 0);
	
	strcpy((char*)ptr4, "123");
	printf("%s\n", (char*)ptr4);
	assert(strcmp(ptr4, "123") == 0);

	hFree(ptr4);

	printf("%s\n", (char*)ptr4);
	printf("%s\n", (char*)ptr);
	assert(strcmp(ptr4, "123") != 0);

	void *ptr3 = hAlloc(3);

	return EXIT_SUCCESS;
}
