*This project was created as part of the 42 curriculum by mona, maria-ol.*

# 🐚 Minishell

> A minimalist Unix shell interpreter built in C, replicating core bash behavior.

---

## Description

Minishell is one of the most complex projects in the 42 curriculum. The goal is to
build a functional command-line interpreter from scratch in C, without relying on
any existing shell implementation.

The shell reads user input, tokenizes it, expands variables, parses the command
structure, and executes commands — reproducing bash's behavior for the supported
feature set. It handles pipelines, redirections, environment variables, signal
processing, and seven built-in commands.

**Key challenge:** Correctly managing process creation with `fork`/`execve`,
inter-process communication via pipes, an isolated environment variable store as
a doubly linked list, and signal handling that precisely matches bash's interactive
behavior.

## Project Structure

```
minishell/
├── Makefile
├── include/
│   └── minishell.h               # All structs, enums and prototypes
├── src/
│   ├── main.c                    # Entry point + REPL loop
│   ├── parsing/
│   │   ├── lexer.c               # Tokenization
│   │   ├── expander.c            # Variable expansion ($VAR, $?)
│   │   ├── expander_utils.c      # Buffer helpers for expander
│   │   ├── parser.c              # Command list construction + syntax validation
│   │   ├── parser_utils.c        # Argument and redirection helpers
│   │   ├── parser_free.c         # Memory cleanup for parser structures
│   │   ├── quotes.c              # Quote state machine and removal
│   │   └── tokens.c              # Token node creation and list management
│   ├── env/
│   │   ├── env_init.c            # Parse envp into t_env linked list
│   │   ├── env_get.c             # Lookup variable by key
│   │   ├── env_set.c             # Set or update variable
│   │   ├── env_unset.c           # Remove variable
│   │   └── env_utils.c           # Convert to char**, print
│   ├── execution/
│   │   ├── executor.c            # Main dispatcher
│   │   ├── executor_simple.c     # Single command via fork/execve
│   │   ├── executor_pipeline.c   # Multi-command pipeline with pipes
│   │   ├── path_finder.c         # Resolve command via PATH
│   │   └── redirections.c        # File descriptor setup for redirections
│   ├── builtins/
│   │   ├── builtin_checker.c     # is_builtin() and execute_builtin()
│   │   ├── builtin_echo.c
│   │   ├── builtin_cd.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_export.c
│   │   ├── builtin_unset.c
│   │   ├── builtin_env.c
│   │   └── builtin_exit.c
│   ├── signals/
│   │   └── signals.c             # SIGINT and SIGQUIT handlers
│   └── utils/
│       ├── error_utils.c         # Bash-style error messages
│       └── free_utils.c          # Memory release helpers
└── libraries/
    └── libft/                    # Custom C library
```

## Instructions

### Requirements

- GCC or Clang
- GNU readline (`brew install readline` on macOS, pre-installed on Linux)
- make

### Building

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
> Homebrew's GNU readline instead of the system's libedit.

### Running

```bash
./minishell
```

### Usage Examples

```bash
minishell$ echo "Hello, $USER"
minishell$ ls -la | grep .c
minishell$ export VAR=42 && echo $VAR
minishell$ echo "test" > file.txt && cat < file.txt
minishell$ echo $?          # last exit status
minishell$ exit
```

### Norminette

```bash
norminette src/ include/
```

### Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=include/valgrind.sup \
         ./minishell
```




# Sair
```
minishell$ exit
```

## Features

| Category | Feature |
|---|---|
| **Execution** | Interactive prompt, command history, PATH resolution |
| **Redirections** | `<` input, `>` output, `>>` append, `<<` heredoc |
| **Pipelines** | Multiple commands: `cmd1 \| cmd2 \| cmd3` |
| **Variables** | `$VAR` expansion, `$?` exit status |
| **Quotes** | Single quotes (no expansion), double quotes (with expansion) |
| **Built-ins** | `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| **Signals** | Ctrl+C new prompt, Ctrl+D exit, Ctrl+\ ignored |

---

## Resources

### Shell and Bash

- [What is Bash?](https://opensource.com/resources/what-bash) — Accessible introduction
  to what a shell is and the role of bash
- [Bash in 100 Seconds](https://www.youtube.com/watch?v=TJzltwv7jJs) — Quick visual
  overview of how the shell works
- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/) — Full
  specification of bash behavior (the ground truth for this project)
- `man bash` — Especially sections: EXPANSION, REDIRECTION, SIGNALS

### Readline

- [GNU Readline Library — Command Line Editing](https://web.mit.edu/gnu/doc/html/rlman_2.html#SEC31)
  — API reference for readline, `rl_on_new_line`, `rl_replace_line`, `add_history`

### System calls

- `man 2 fork`, `man 3 execve`, `man 2 pipe`, `man 2 dup2`, `man 2 sigaction`, `man 2 waitpid`

### Additional reading

- [Writing a Unix Shell (series)](https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/)
  — Step-by-step walkthrough of building a shell in C

### AI Usage

GitHub Copilot (Claude Sonnet 4.6) was used throughout this project for the
following tasks:

| Area | How AI was used |
|---|---|
| **Documentation** | Generated Doxygen-style doc comments for all functions; reviewed existing docs for accuracy |
| **Signal handling** | Explained `128 + N` exit codes, `sigaction` vs `signal()`, macOS libedit incompatibility; implemented `setup_signals` and `handle_sigint` |
| **Bug detection** | Found key/value swap bug in `sort_env` (builtin_export.c) |
| **Makefile** | Added OS detection to link against Homebrew readline on macOS |
| **Study planning** | Generated structured study plans for env, parsing, and error handling modules |
| **Code review** | Reviewed builtins for behavioral accuracy against bash |

AI was not used to write core logic from scratch. All algorithms, data structures,
and architecture decisions were made by the team.

---

## Authors

- **maria-ol** — Parsing (lexer, expander, parser, quotes), Environment & Signals
- **mgomes-t** — Execution (executor, pipes, redirections) & Built-ins  

---

*This project is part of the 42 School curriculum.*
