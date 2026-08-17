#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARGV_SIZE 100
#define WRAPPER_SIZE 10

const char *inner_delimiters = "  ";

void free_pipes(int n, int pipefd[][2]) {
  for (int j = 0; j < n - 1; j++) {
    close(pipefd[j][0]);
    close(pipefd[j][1]);
  }
}

Argv_Wrapper *build_argv_wrapper(char *command) {
  int counter = 0;

  char *inner_token = strtok(command, inner_delimiters);
  Argv_Wrapper *new = malloc(sizeof(Argv_Wrapper));

  if (new == NULL) {
    printf("Wrapper memory allocation failed.");
    return new;
  }

  while (inner_token != NULL) {
    new->argv[counter] = inner_token;
    inner_token = strtok(NULL, inner_delimiters);
    counter++;
  }

  new->argv[counter] = NULL;
  new->length = counter;

  return new;
}