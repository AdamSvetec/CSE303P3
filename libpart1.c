#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "support.h"
// hello() - print some output so we know the shared object loaded.
//
// The function signature takes an untyped parameter, and returns an untyped
// parameter.  In this way, the signature *could* support any behavior, by
// passing in a struct and returning a struct.
void *hello(void *input) {
    printf("hello from a .so\n");
    return NULL;
}

/*
  Make copy of team_t struct pointer and uppercase email and names
  return pointer to newly allocated struct
 */
void *ucase(void *input){
  //allocate a new team struct
  struct team_t * old_team = (struct team_t *) input;
  struct team_t * new_team = malloc(sizeof(struct team_t));

  new_team->name1 = malloc(strlen(old_team->name1)+1);
  new_team->email1 = malloc(strlen(old_team->email1)+1);
  new_team->name2 = malloc(strlen(old_team->name2)+1);
  new_team->email2 = malloc(strlen(old_team->email2)+1);

  strcpy(new_team->name1, old_team->name1);
  strcpy(new_team->email1, old_team->email1);
  strcpy(new_team->name2, old_team->name2);
  strcpy(new_team->email2, old_team->email2);

  for(int i = 0; i < strlen(new_team->name1); i++){
    new_team->name1[i] = toupper(new_team->name1[i]);
  }

  for(int i = 0; i < strlen(new_team->email1); i++){
    new_team->email1[i] = toupper(new_team->email1[i]);
  }

  for(int i = 0; i < strlen(new_team->name2); i++){
    new_team->name2[i] = toupper(new_team->name2[i]);
  }

  for(int i = 0; i < strlen(new_team->email2); i++){
    new_team->email2[i] = toupper(new_team->email2[i]);
  }
  
  return new_team;
}
