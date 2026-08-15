#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Before fork, my PID is %d\n", getpid());

  pid_t result = fork();

  if (result == 0) {
    printf("I'm the child, my PID is %d, fork() returned %d\n", getpid(),
           result);
  } else if (result > 0) {
    printf(
        "I'm the parent, my PID is %d, fork() returned %d (my child's PID)\n",
        getpid(), result);
  } else {
    printf("fork() failed\n");
  }

  return 0;
}