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

void *ucase(struct team_t team){
  //allocate a new team struct
  struct team_t new_team;
  new_team.name1 = strdup(team.name1);
  new_team.email1 = strdup(team.email1);
  new_team.name2 = strdup(team.name2);
  new_team.email2 = strdup(team.email2);
 
  //Copy fields of parameter struck to new team struct
  memcpy(new_team.name1, team.name1, sizeof(team.name1));
  memcpy(new_team.email1, team.email1, sizeof(team.email1));
  memcpy(new_team.name2, team.name2, sizeof(team.name2));
  memcpy(new_team.email2, team.email2, sizeof(team.email2));  

  //Capitalize team member names
  for(int i = 0; new_team.name1[i] != '\0'; i++){
    new_team.name1[i] = toupper(new_team.name1[i]);
  }
  for(int j = 0; new_team.name2[j] != '\0'; j++){
    new_team.name2[j] = toupper(new_team.name2[j]);
    }

  //Print contents of new struct
  printf("Student1 : %s\n", new_team.name1);
  printf("Email1 : %s\n", new_team.email1);
  printf("Student2 : %s\n", new_team.name2);
  printf("Email2 : %s\n", new_team.email2);

  //Deallocate memory
  free(new_team.name1);
  free(new_team.email1);
  free(new_team.name2);
  free(new_team.email2);
}
