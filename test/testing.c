#include <stdio.h>
#include <stdlib.h>

// Testing file for interop library

int main(){
  char * ptr = malloc(20*sizeof(char));
  char * ptr2 = malloc(20*sizeof(char));
  free(ptr);
  free(ptr2);
}
