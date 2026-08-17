#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>
#define ARGV_SIZE 100
#define WRAPPER_SIZE 10

typedef struct {
  size_t length;
  char *argv[ARGV_SIZE];
} Argv_Wrapper;

/**
 * @brief Frees N - 1 pipes
 *
 * @param n The number of pipes plus 1
 * @param pipefd The 2D array containing the pipes
 */
void free_pipes(int n, int pipefd[][2]);

/**
 * @brief Builds and Argv_Wrapper struct from a string command
 *
 * @param command The string command
 *
 * @returns The built Argv_Wrapper
 */
Argv_Wrapper *build_argv_wrapper(char *command);

#endif // HELPER_H