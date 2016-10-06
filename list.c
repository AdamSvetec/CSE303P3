
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>

/* called upon library unload */
void map_dump() __attribute__((destructor));

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
  struct map_node * next;
}map_node_t;

map_node_t * create_map_node(uintptr_t pointer, char * module, char * line){
  map_node_t * node = malloc(sizeof(map_node_t));
  node->allocated_pointer = pointer;
  node->call_site = malloc(strlen(module)+1);
  node->program_counter = malloc(strlen(line)+1);
  strcpy(node->call_site, module);
  strcpy(node->program_counter, line);
  node->next = NULL;
  return node;
}

void free_map_node(map_node_t * node){
  free(node->call_site);
  free(node->program_counter);
  free(node);
  node = NULL;
}

static int interop_skip = 0;

int get_interop_skip(){
  return interop_skip;
}

void set_interop_skip(int val){
  interop_skip = val;
}

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
  map_node_t * insert = create_map_node(pointer, module, line);
  if(alloc_info == NULL){
    alloc_info = insert;
    return 1;
  }

  map_node_t * node_ptr = alloc_info;
  while(node_ptr->next != NULL){
    node_ptr = node_ptr->next;
  }
  node_ptr->next = insert;
  return 1;
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
  if(alloc_info == NULL){
    return 0;
  }else if(alloc_info->allocated_pointer == pointer){
    map_node_t * to_delete = alloc_info;
    alloc_info = alloc_info->next;
    free_map_node(to_delete);
    return 1;
  }
  map_node_t * prev_ptr = alloc_info;
  map_node_t * node_ptr = alloc_info->next;
  while(node_ptr != NULL){
    if(node_ptr->allocated_pointer == pointer){
      prev_ptr->next = node_ptr->next;
      free_map_node(node_ptr);
      return 1;
    }
    prev_ptr = node_ptr;
    node_ptr = node_ptr->next;
  }
  return 0;
}

/*
 * count() - return the number of elements in the map.  This can indicate
 *           that there are un-freed allocations (memory leaks).
 */
int map_count() {
  int count = 0;
  map_node_t *node_ptr = alloc_info;
  while(node_ptr != NULL){
    count++;
    node_ptr = node_ptr->next;
  }
  return count;
}

/*
 * dump() - output the contents of the list
 */
void map_dump() {
  //map_node_t* curr = alloc_info;
  map_node_t* to_free;
  set_interop_skip(1);
  printf("\nnumber of un-freed allocations: %d\n", map_count());
  while(alloc_info) {
    printf("  0x%x allocated by %s::%s\n", alloc_info->allocated_pointer, alloc_info->call_site, alloc_info->program_counter);
    to_free = alloc_info;
    alloc_info = alloc_info->next;
    free_map_node(to_free);
  }
}
