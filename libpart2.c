#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <stdio.h>

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

/* interop malloc */
void * malloc(size_t size){
  void * (*realmalloc)(size_t)=(void*(*)(size_t))(dlsym (RTLD_NEXT, "malloc"));
  void * malloced_ptr = realmalloc(size);
  map_insert((uintptr_t)malloced_ptr, "TODO", "TODO");
  printf("malloc\n");
  return malloced_ptr;
}

/* interop free */
void free(void *ptr){
  printf("free\n");
  void (*realfree)(void *ptr)=(void(*)(void * ptr))(dlsym (RTLD_NEXT, "free"));
  realfree(ptr);
  map_remove((uintptr_t)ptr);
}
