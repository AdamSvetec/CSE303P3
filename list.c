#include <assert.h>
#include <stdio.h>
#include <stdint.h>

/*
 * Note: 'uintptr_t' is a special type of unsigned integer that is guaranteed
 *       to be the same size as pointers.  This is the preferred way to cast
 *       between pointers and integers in code that could be compiled in
 *       32-bit or 64-bit mode.
 */

/*
 * A type for nodes in a linked list.  The linked list will be used to map
 * pointers to strings, so we can save information about the state of the
 * program each time a pointer was returned by malloc()
 */
typedef struct map_node
{
  uintptr_t allocated_pointer;
  char      *call_site;
  char      *program_counter;
} map_node_t;

/*
 * A list, based on map_node_t
 *
 * NB: The list is static, so that its name isn't visible from other .o files
 */
static map_node_t* alloc_info;

/*
 * insert() - when malloc() is called, your interpositioning library
 *            should use this to store the pointer that malloc returned,
 *            along with the module name and line of code that
 *            backtrace() returned.  Return 1 if the pointer was
 *            successfully added, and 0 if the pointer was already in the
 *            map.
 */
int map_insert(uintptr_t pointer, char *module, char *line) {
  /* TODO: complete this code */
}

/*
 * remove() - when free() is called, your interpositioning library should use
 *            this to remove the pointer and its strings from the list
 *            declared above.  In this way, you'll be able to track, at all
 *            times, which allocations are outstanding, and what line of code
 *            caused those allocations.  Return 1 if the remove was
 *            successful, and 0 if the pointer was already removed from the
 *            map (which would suggest a double-free).
 */
int map_remove(uintptr_t pointer) {
  /* TODO: complete this code */
}

/*
 * count() - return the number of elements in the map.  This can indicate
 *           that there are un-freed allocations (memory leaks).
 */
int map_count() {
  /* TODO: complete this code */
  return -1;
}

/*
 * dump() - output the contents of the list
 */
void map_dump() {
  /* TODO: complete this code */
  map_node_t* curr = alloc_info;
  while (curr) {
    printf("  0x%x allocated by %s::%s", -1, "-1", "-1");
  }
}
