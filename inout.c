#include <stdio.h>
#include <stdlib.h>

/*
 * some adjectives, so we can have a bit more fun with the output
 */
char *adjectives[] = {
  "funny", "silly", "cool", "lame", "smart", "amazing"
};

/*
 * main() - just read from stdin and write to stdout, using scanf and printf
 */
int main(int argc, char **argv) {
  char line[4096];
  int nread = 0;
  unsigned int seed = 1;

  printf("enter some text and I'll repeat it, until you press ctrl-d\n");
  while (1) {
    int r = rand_r(&seed) % 6;
    nread = scanf(" %[^\n]", line);
    if (nread == EOF) break;
    printf("You typed: %s\n", line);
    printf("That's %s!\n", adjectives[r]);
  }
}
