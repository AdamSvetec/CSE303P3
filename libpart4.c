#include <dlfcn.h>
#include <execinfo.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*
  parse backtrace and look for printf or scanf calls
 */
int check_backtrace(char ** strings, int length){
  for(int i = 0; i < length; i++){
    if(strstr(strings[i], "printf") != NULL || strstr(strings[i], "scanf") != NULL){
      return 1;
    }
  }
  return 0;
}

/*
 interpositioned pipe
 -if call comes from printf or scanf, make this function unexecutable
 */
int pipe(int pipefd[2]){

  int (*realpipe)(int pipefd[2])=(int(*)(int pipefd[2]))(dlsym (RTLD_NEXT, "pipe"));

  int buffer_size = 1024;
  void *buffer[buffer_size];
  char ** strings;
  int nptrs = backtrace(buffer, buffer_size);
  strings = backtrace_symbols(buffer, nptrs);
  int found = check_backtrace(strings, nptrs);
  if(found){
    void * pipe_func_ptr = pipe;
    int pagesize=sysconf(_SC_PAGE_SIZE);
    uintptr_t space_past_pagesize= (uintptr_t)pipe_func_ptr % (uintptr_t)pagesize;
    mprotect((void*)((uintptr_t)pipe_func_ptr - space_past_pagesize), (size_t)(space_past_pagesize), PROT_NONE);
  }

  return realpipe(pipefd);  
}
