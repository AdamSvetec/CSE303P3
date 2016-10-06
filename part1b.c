#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "support.h"
#include <dlfcn.h>

/*
 * load_and_invoke() - load the given .so and execute the specified function
 */
typedef void* (*any)();

void load_and_invoke(char *libname, char *funcname) {
  /* TODO: complete this function */
  void *handle;
  char *error;
  any function;;
 
  handle = dlopen(libname, RTLD_NOW);;
  if(!handle) {
    fputs(dlerror(), stderr);
    exit(1);
    }

  *(void **) (&function) = dlsym(handle, funcname);
  if((error = dlerror()) != NULL){
    fputs(error, stderr);
    exit(1);
    }
  
  (void)function();
  dlclose(handle);
}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS]\n", progname);
    printf("Load the given .so and run the requested function from that .so\n");
    printf("  -l [string] The name of the .so to load\n");
    printf("  -f [string] The name of the function within that .so to run\n");
}

/*
 * main() - The main routine parses arguments and invokes hello
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;
    char *libname;
    char *funcname;

    /* run a student name check */
    check_team(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    while ((opt = getopt(argc, argv, "hl:f:")) != -1) {
        switch(opt) {
	case 'h': help(argv[0]); break;
	case 'l': libname = optarg; break;
	case 'f': funcname = optarg; break;
	}
    }

    /* call load_and_invoke() to run the given function of the given library */
    load_and_invoke(libname, funcname);

    exit(0);
}
