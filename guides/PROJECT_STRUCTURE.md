# 🐚 Minishell - Estrutura do Projeto

## 📋 Descrição

Implementação de um shell minimalista (como bash) que suporta execução
de comandos, pipes, redirecionamentos, variáveis de ambiente e built-ins.

## 🏗️ Estrutura de Arquivos

```
minishell/
├── Makefile
├── README.md
├── include/
│   └── minishell.h           # Header principal — structs, enums, protótipos
│
├── src/
│   ├── main.c                # Entry point + REPL loop
│   │
│   ├── parsing/              # Interpretação da linha de comando
│   │   ├── lexer.c           # Tokenização da string bruta
│   │   ├── expander.c        # Expansão de variáveis ($VAR, $?)
│   │   ├── expander_utils.c  # Helpers do expander (buffers, extract_var)
│   │   ├── parser.c          # Construção da lista de comandos (t_cmd)
│   │   ├── parser_utils.c    # Helpers do parser (add_arg, add_redir, etc)
│   │   ├── parser_free.c     # Free de tokens/cmds + remove_quotes_from_tokens
│   │   ├── quotes.c          # Máquina de estados de aspas
│   │   └── tokens.c          # Criação e free de tokens
│   │
│   ├── env/                  # Variáveis de ambiente
│   │   ├── env_init.c        # init_env (envp → t_env list)
│   │   ├── env_get.c         # get_env_value
│   │   ├── env_set.c         # set_env_value
│   │   ├── env_unset.c       # unset_env_value
│   │   └── env_utils.c       # env_to_array, print_env, free_env
│   │
│   ├── execution/            # Execução de comandos
│   │   ├── executor.c        # Dispatcher + fork_pipeline + handle_signal_status
│   │   ├── executor_simple.c # Comando único (fork + execve + wait_child)
│   │   ├── executor_pipeline.c # Pipeline (child_process, wait_all)
│   │   ├── path_finder.c     # Resolução do PATH
│   │   ├── redirections.c    # redir_heredoc, apply_redir, setup_redirections
│   │   └── redirections_utils.c # restore_fds, redir_in, redir_out
│   │
│   ├── builtins/             # Comandos built-in
│   │   ├── builtin_checker.c # is_builtin + execute_builtin (dispatcher)
│   │   ├── builtin_echo.c    # echo com -n
│   │   ├── builtin_cd.c      # cd + update PWD/OLDPWD
│   │   ├── builtin_pwd.c     # pwd via getcwd
│   │   ├── builtin_export.c  # export (set env + print declare)
│   │   ├── builtin_unset.c   # unset
│   │   ├── builtin_env.c     # env (print env)
│   │   └── builtin_exit.c    # exit
│   │
│   ├── signals/              # Tratamento de sinais
│   │   └── signals.c         # setup_signals, setup_exec_signals,
│   │                         # setup_child_signals, handle_sigint
│   │
│   └── utils/                # Utilitários gerais
│       ├── error_utils.c     # handle_error, exit_error
│       └── free_utils.c      # free_array, safe_free
│
└── libraries/
    └── libft/                # Biblioteca própria
```

## 📦 Estruturas de Dados

```c
t_mini    // Estado global do shell (env, cmd_list, last_exit_status, running)
t_env     // Nó da lista de variáveis de ambiente (key, value, prev, next)
t_token   // Token do lexer (type, value, next)
t_cmd     // Nó de comando (args, redirs, pid, next)
t_redir   // Redirecionamento (type, file, next)
t_exp_state // Estado interno do expander (result, i, quote_state)
```

## ✅ Checklist de Features

### Obrigatórias
- [x] Prompt interativo (readline)
- [x] Histórico de comandos (add_history)
- [x] Buscar e executar binários via PATH
- [x] Caminhos relativos e absolutos
- [x] Pipes (`|`)
- [x] Redirecionamentos (`<`, `>`, `>>`)
- [x] Heredoc (`<<`)
- [x] Expansão de variáveis (`$VAR`)
- [x] Status de saída (`$?`)
- [x] Aspas simples (sem expansão)
- [x] Aspas duplas (com expansão de variáveis)
- [x] Sinais (Ctrl-C, Ctrl-D, Ctrl-\\)

### Built-ins
- [x] `echo` (com opção `-n`)
- [x] `cd` (com caminho relativo/absoluto)
- [x] `pwd`
- [x] `export`
- [x] `unset`
- [x] `env`
- [x] `exit`

## ⚠️ Cuidados Importantes

- **Memory leaks**: `readline()` causa leaks internos — não são de responsabilidade do projeto. Todo o resto deve ser liberado corretamente.
- **File descriptors**: Sempre feche os FDs que abrir.
- **Sinais**: Três modos — prompt interativo, filho rodando, child process.
- **Norminette**: Rodar antes de cada commit. Norminette está em `~/Library/Python/3.9/bin/norminette`.
- **Uma variável global**: Apenas `g_signal` (volatile sig_atomic_t) é permitida.
