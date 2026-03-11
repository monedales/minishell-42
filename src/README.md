*This project was created as part of the 42 curriculum by maria-ol, mgomes-t.*

# minishell 🐚

A minimalist Unix shell interpreter built in C, replicating core bash behavior.

---

## Description

Minishell is one of the most complex projects in the 42 curriculum. The goal is to
build a functional command-line interpreter from scratch in C, without relying on
any existing shell implementation.

The shell reads user input, tokenizes it, expands variables, parses the command
structure, and finally executes the commands — reproducing the behavior of bash for
the supported feature set.

**Key challenge:** Correctly managing process creation with `fork`/`execve`, handling
inter-process communication via pipes, maintaining an isolated environment variable
store as a linked list, and implementing signal handling that precisely matches bash's
interactive behavior.

### Features

- Interactive prompt with command history (readline)
- Variable expansion: `$VAR`, `$?`
- Single and double quote handling
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Pipelines: `cmd1 | cmd2 | cmd3`
- Signal handling: Ctrl+C, Ctrl+D, Ctrl+\
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- PATH resolution for external commands

---

## Instructions

### Requirements

- GCC or Clang
- GNU readline (`brew install readline` on macOS, pre-installed on Linux)
- make

### Compilation

```bash
# Clone the repository
git clone <repository-url>
cd minishell

# Compile
make

# Rebuild from scratch
make re

# Clean object files
make clean

# Full clean (objects + binary)
make fclean
```

> **macOS note:** The Makefile automatically detects the OS and links against
> Homebrew's GNU readline instead of the system's libedit, which lacks required
> symbols such as `rl_replace_line`.

### Running

```bash
./minishell
```

### Usage Examples

```bash
# Simple commands
minishell$ ls -la
minishell$ pwd
minishell$ echo "Hello, $USER"

# Pipelines
minishell$ ls | grep .c | wc -l

# Redirections
minishell$ echo "log entry" >> log.txt
minishell$ cat < log.txt

# Heredoc
minishell$ cat << EOF
> line one
> line two
> EOF

# Variables
minishell$ export NAME=minishell
minishell$ echo "Running $NAME"
minishell$ unset NAME

# Exit status
minishell$ ls /nonexistent
minishell$ echo $?       # prints 1

# Exit
minishell$ exit 0
```

### Checking for memory leaks

```bash
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=include/valgrind.sup \
         ./minishell
```

### Code style

```bash
norminette src/ include/
```

---

## Architecture

### Data flow

```
User input (readline)
        │
        ▼
   ┌─────────┐
   │  LEXER  │  Breaks input into tokens (words, pipes, redirections)
   └────┬────┘
        │
        ▼
   ┌──────────┐
   │ EXPANDER │  Replaces $VAR and $? with their values
   └────┬─────┘
        │
        ▼
   ┌─────────┐
   │ PARSER  │  Builds a linked list of t_cmd nodes
   └────┬────┘
        │
        ▼
   ┌──────────────┐
   │   EXECUTOR   │  fork() + execve() or builtin dispatch
   └────┬─────────┘
        │
   $? updated ──► next prompt
```

### Key data structures

```c
// Environment variable (doubly linked list)
typedef struct s_env
{
    char          *key;
    char          *value;
    struct s_env  *prev;
    struct s_env  *next;
}   t_env;

// Parsed command node
typedef struct s_cmd
{
    char         **args;    // ["ls", "-la", NULL]
    t_redir       *redirs;  // linked list of redirections
    pid_t          pid;
    struct s_cmd  *next;    // next command in pipeline
}   t_cmd;

// Main shell state
typedef struct s_mini
{
    t_env  *env;
    t_cmd  *cmd_list;
    int     last_exit_status;
    int     running;
}   t_mini;
```

### Source layout

```
minishell/
├── Makefile
├── include/
│   └── minishell.h          # All structs, enums and prototypes
└── src/
    ├── main.c               # Entry point + REPL loop
    ├── parsing/
    │   ├── lexer.c          # Tokenization
    │   ├── expander.c       # Variable expansion ($VAR, $?)
    │   ├── expander_utils.c # Buffer helpers for expander
    │   ├── parser.c         # Command list construction + syntax validation
    │   ├── parser_utils.c   # Argument and redirection helpers
    │   ├── parser_free.c    # Memory cleanup for parser structures
    │   ├── quotes.c         # Quote state machine and removal
    │   └── tokens.c         # Token node creation and list management
    ├── env/
    │   ├── env_init.c       # Parse envp into t_env linked list
    │   ├── env_get.c        # Lookup variable by key
    │   ├── env_set.c        # Set or update variable
    │   ├── env_unset.c      # Remove variable
    │   └── env_utils.c      # Convert to char**, print
    ├── execution/
    │   ├── executor.c       # Main dispatcher (builtin vs external vs pipeline)
    │   ├── executor_simple.c# Single external command via fork/execve
    │   ├── executor_pipeline.c # Multi-command pipeline with pipes
    │   ├── path_finder.c    # Resolve command name to absolute path via PATH
    │   └── redirections.c   # Setup file descriptors for <, >, >>, <<
    ├── builtins/
    │   ├── builtin_checker.c# is_builtin() and execute_builtin() dispatcher
    │   ├── builtin_echo.c
    │   ├── builtin_cd.c
    │   ├── builtin_pwd.c
    │   ├── builtin_export.c
    │   ├── builtin_unset.c
    │   ├── builtin_env.c
    │   └── builtin_exit.c
    ├── signals/
    │   └── signals.c        # SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) handlers
    └── utils/
        ├── error_utils.c    # Bash-style error messages via t_error enum
        └── free_utils.c     # Safe memory release helpers
```

---

## Resources

### Shell and Bash

- [What is Bash?](https://opensource.com/resources/what-bash) — Accessible introduction
  to what a shell is and the role of bash
- [Bash in 100 Seconds](https://www.youtube.com/watch?v=TJzltwv7jJs) — Quick visual
  overview of how the shell works
- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/) — Full
  specification of bash behavior (the ground truth for this project)
- `man bash` — Local reference, especially sections: EXPANSION, REDIRECTION, SIGNALS

### Readline

- [GNU Readline Library — Command Line Editing](https://web.mit.edu/gnu/doc/html/rlman_2.html#SEC31)
  — API reference for readline, rl_on_new_line, rl_replace_line, add_history

### System calls

- `man 2 fork` — Process creation
- `man 3 execve` — Process replacement (and why `char **envp` is required)
- `man 2 pipe` — Inter-process communication
- `man 2 dup2` — Redirecting file descriptors
- `man 2 sigaction` — Reliable signal handling (used instead of `signal()`)
- `man 2 waitpid` — Retrieving child exit status

### Additional reading

- [Writing a Unix Shell (series)](https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/)
  — Step-by-step walkthrough of building a shell in C (concepts align with this project)
- [Bash Hackers Wiki](https://wiki.bash-hackers.org/) — In-depth articles on quoting,
  expansion, and parameter handling

---

### AI Usage

GitHub Copilot (Claude Sonnet 4.6) was used throughout this project for the
following tasks:

| Area | How AI was used |
|---|---|
| **Documentation** | Generated Doxygen-style doc comments for all functions, reviewed and corrected existing docs for accuracy |
| **Signal handling** | Explained the `128 + N` exit status convention, `sigaction` vs `signal()` trade-offs, and the macOS libedit vs GNU readline incompatibility; implemented `setup_signals` and `handle_sigint` |
| **Bug detection** | Identified a key/value swap bug in `sort_env` (builtin_export.c) where the original key was overwritten before being saved |
| **Makefile** | Added OS detection (`uname`) to link against Homebrew's GNU readline on macOS while keeping Linux behavior unchanged |
| **Study planning** | Generated structured study plans and resource explanations for the env, parsing, and error handling modules |
| **Code review** | Reviewed builtin implementations for behavioral accuracy against bash (e.g., removed false claim that `exit` prints to stderr) |

AI was not used to write core logic from scratch. All algorithms, data structures,
and architecture decisions were made by the team; AI assisted with documentation,
debugging, and infrastructure tasks.
