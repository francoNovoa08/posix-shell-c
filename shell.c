#include "helper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char *command_delimiters = "|";

int main() {

  int pipefd[WRAPPER_SIZE - 1][2];

  Argv_Wrapper *argv_wrapper[WRAPPER_SIZE];
  char buffer[ARGV_SIZE];

  int command_counter = 0;

  while (true) {
    for (int i = 0; i < command_counter; i++) {
      free(argv_wrapper[i]);
    }

    command_counter = 0;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      return -1;
    }
    buffer[strcspn(buffer, "\n")] = '\0';


    char *commands[WRAPPER_SIZE];
    char *command_token = strtok(buffer, command_delimiters);

    if (command_token == NULL) {
      continue;
    }

    while (command_token != NULL && command_counter < WRAPPER_SIZE) {
      commands[command_counter] = command_token;
      command_counter++;

      command_token = strtok(NULL, command_delimiters);
    }

    for (int i = 0; i < command_counter; i++) {
      argv_wrapper[i] = build_argv_wrapper(commands[i]);
    }

    if (strcmp(argv_wrapper[0]->argv[0], "exit") == 0) {
      break;
    } else if (strcmp(argv_wrapper[0]->argv[0], "cd") == 0) {
      if (argv_wrapper[0]->argv[1] == NULL) {
        char *home = getenv("HOME");
        if (home != NULL) {
          chdir(home);
        } else {
          perror("Could not find HOME environment variable");
        }

      } else if (chdir(argv_wrapper[0]->argv[1]) != 0) {
        perror("Failed to change directory");
      }
      continue;
    }

    for (int i = 0; i < command_counter - 1; i++) {
      if (pipe(pipefd[i]) < 0) {
        perror("Pipe creation failed.");
        return -1;
      }
    }

    for (int i = 0; i < command_counter; i++) {
      pid_t result = fork();

      if (result == 0) {
        if (i != 0) {
          if (dup2(pipefd[i - 1][0], STDIN_FILENO) < 0) {
            perror("Failed to duplicate input file descriptor.");
            close(pipefd[i][0]);
            return -1;
          }
        }

        if (i != command_counter - 1) {
          if (dup2(pipefd[i][1], STDOUT_FILENO) < 0) {
            perror("Failed to duplicate output file descriptor.");
            close(pipefd[i][1]);
            return -1;
          }
        }

        free_pipes(command_counter, pipefd);

        execvp(argv_wrapper[i]->argv[0], argv_wrapper[i]->argv);
        perror("execvp");
        return -1;
      } else if (result < 0) {
        printf("fork() failed\n");
      }
    }

    free_pipes(command_counter, pipefd);

    for (int j = 0; j < command_counter; j++) {
      wait(NULL);
    }
  }

  return 0;
}