# posix-shell-c

A shell written in C, using fork, exec, and pipe directly. Supports running commands, piping output between multiple commands, and two built-ins.

## What it does

- Runs any command available on `PATH`, with arguments. Command length capped at ARGV_SIZE.
- Pipes any number of commands together, e.g. `ls | grep .c | wc -l` (max 10)
- `cd`, including `cd` with no argument (goes to `$HOME`)
- `exit`

## How it works

Each line read from input is split on `|` into a list of commands, and each command is split into its own argument list. For a single command, the shell forks once, and the child execs into that command while the parent waits. For a pipeline of N commands, the shell creates N-1 pipes up front, then forks N children in a loop. Each child redirects its input and output to the correct pipe using `dup2`, closes every pipe descriptor it isn't using, then execs. The parent closes all its own pipe descriptors once every child is forked, then waits for each one.

cd and exit are handled directly in the shell process, since a forked cd would only change the working directory of a process that exits immediately afterward, with no effect on the shell itself.
## Building

```
gcc -Wall -Wextra -o shell shell.c helper.c
```

Tested with ASan:

```
gcc -Wall -Wextra -fsanitize=address -g -o shell shell.c helper.c
```

## What it doesn't do

No I/O redirection (`>`, `<`), no globbing, no environment variable expansion, no job control.
