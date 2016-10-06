#include <dlfcn.h>
#include <execinfo.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

/*
 * We aren't providing much code here.  You'll need to implement your own
 * dlsym(), as well as any constructors or destructors for your library.
 */

/* TODO: Your code goes here */

int check_backtrace(char ** strings, int length){
  return 1;
}

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
    mprotect((void*)((uintptr_t)pipe_func_ptr - space_past_pagesize), (size_t)(space_past_pagesize+2), PROT_EXEC);
  }

  return pipe(pipefd);  
}
