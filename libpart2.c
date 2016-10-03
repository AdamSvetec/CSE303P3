#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>

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

int get_interop_skip();
void set_interop_skip(int val);
/* TODO: Your code goes here */

void parse_backtrace_line(char * buffer, char * module, char * program_counter){
  int buffercounter = 0;
  while(buffer[buffercounter] != '\0' && buffer[buffercounter] != '['){
    module[buffercounter] = buffer[buffercounter];
    buffercounter++;
  }
  int start_count = buffercounter;
  module[buffercounter-1] = '\0'; //there's a space before '['
  while(buffer[buffercounter] != '\0'){
    program_counter[buffercounter-start_count] = buffer[buffercounter];
    buffercounter++;
  }
  program_counter[buffercounter-start_count] = '\0';
}

/* interop malloc */
void * malloc(size_t size){
  void * (*realmalloc)(size_t)=(void*(*)(size_t))(dlsym (RTLD_NEXT, "malloc"));
  void * malloced_ptr = realmalloc(size);
  if(get_interop_skip()){
    //printf("untracked malloc\n");
  }else{
    //printf("tracked malloc\n");
    set_interop_skip(1);
    int buffer_size = 2;
    void *buffer[buffer_size];
    char ** strings;
    int nptrs = backtrace(buffer, buffer_size);
    strings = backtrace_symbols(buffer, nptrs);
    char module[1024], program_counter[1024];
    parse_backtrace_line(strings[1], module, program_counter);
    map_insert((uintptr_t)malloced_ptr, module, program_counter);
    set_interop_skip(0);
  }
  return malloced_ptr;
}

/* interop free */
void free(void *ptr){
  void (*realfree)(void *ptr)=(void(*)(void * ptr))(dlsym (RTLD_NEXT, "free"));
  realfree(ptr);
  if(get_interop_skip()){
    //printf("untracked free\n");
  }else{
    //printf("tracked free\n");
    set_interop_skip(1);
    if(0 == map_remove((uintptr_t)ptr)){
      //this node has already been removed
    }
    set_interop_skip(0);
  }
}
