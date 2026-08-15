#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  const int SIZE = 100;

  char buffer[SIZE];
  char *argv[SIZE];
  const char *delimiters = "  ";
  int counter = 0;

  while (true) {
    counter = 0;
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      return -1;
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    char *token = strtok(buffer, delimiters);
    if (token == NULL) {
      continue;
    }

    while (token != NULL) {
      argv[counter] = token;
      token = strtok(NULL, delimiters);
      counter++;
    }
    argv[counter] = NULL;

    if (strcmp(argv[0], "exit") == 0) {
      break;
    } else if (strcmp(argv[0], "cd") == 0) {
      if (argv[1] == NULL) {
        char *home = getenv("HOME");
        if (home != NULL) {
          chdir(home);
        } else {
          perror("Could not find HOME environment variable");
        }

      } else if (chdir(argv[1]) != 0) {
        perror("Failed to change directory");
      }
      continue;
    }

    pid_t result = fork();

    if (result == 0) {
      execvp(argv[0], argv);
      perror("execvp");
      return -1;
    } else if (result > 0) {
      wait(NULL);
    } else {
      printf("fork() failed\n");
    }
  }

  return 0;
}