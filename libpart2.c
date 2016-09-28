#include <stdint.h>

/*
 * We aren't providing much code here.  You'll need to implement your own
 * malloc() and free(), as well as any constructors or destructors for your
 * library.
 */

/*
 * Declarations for the functions in list.c, so that we don't need an
 * extra header file.
 */
int map_insert(uintptr_t pointer, char *module, char *line);
int map_remove(uintptr_t pointer);
int map_count();
void map_dump();

/* TODO: Your code goes here */
