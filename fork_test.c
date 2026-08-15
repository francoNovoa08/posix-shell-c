#include <stdbool.h>
#include <stdio.h>
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
      printf("Token: %s\n", token);
      argv[counter] = token;
      token = strtok(NULL, delimiters);
      counter++;
    }
    argv[counter] = NULL;
    
    if (strcmp(argv[0], "exit") == 0) {
      break;
    }

    printf("Before fork, my PID is %d\n", getpid());

    pid_t result = fork();

    if (result == 0) {
      printf("I'm the child, my PID is %d, fork() returned %d\n", getpid(),
             result);
      execvp(argv[0], argv);
      return -1;
    } else if (result > 0) {
      wait(NULL);
      printf(
          "I'm the parent, my PID is %d, fork() returned %d (my child's PID)\n",
          getpid(), result);
    } else {
      printf("fork() failed\n");
    }
  }

  return 0;
}