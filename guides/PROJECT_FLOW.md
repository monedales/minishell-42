# 🔄 Minishell — Fluxo Completo do Projeto

Este guia explica, em ordem de execução, como o minishell funciona
desde a inicialização até o encerramento.

---

## Visão geral

```
main()
 ├── init_minishell()          → cria t_mini + lista de env
 ├── setup_signals()           → configura SIGINT / SIGQUIT
 ├── repl_loop()               → loop principal (REPL)
 │    └── process_line()       → pipeline de parsing + execução
 │         ├── lexer()
 │         ├── expand_tokens()
 │         ├── remove_quotes_from_tokens()
 │         ├── parser()
 │         └── execute_cmd_list()
 └── cleanup_minishell()       → libera toda a memória
```

---

## 1. Inicialização (`main.c`)

### `init_minishell()`
- Recebe `envp` (array de strings `CHAVE=VALOR` do processo pai).
- Chama `init_env(envp)` que converte cada string em um nó `t_env` (doubly linked list).
- Inicializa `mini.running = TRUE`, `mini.last_exit_status = 0`, `mini.cmd_list = NULL`.

### `setup_signals()`
- Configura dois handlers com `sigaction`:
  - **SIGINT (Ctrl-C)**: chama `handle_sigint()` → escreve `\n`, limpa readline, redesenha prompt.
  - **SIGQUIT (Ctrl-\\)**: ignorado (`SIG_IGN`), igual ao bash interativo.
- Usa `SA_RESTART` para que `readline` não seja interrompido no meio de uma leitura.

---

## 2. Loop principal — REPL (`repl_loop`)

O REPL (**Read–Eval–Print Loop**) é um `while (mini->running)`:

```
readline("minishell$ ")
        │
        ├── NULL (Ctrl-D)         → imprime "exit\n", encerra loop
        ├── g_signal == 130       → atualiza last_exit_status, g_signal = 0, continue
        ├── linha vazia ""        → libera e continua
        └── linha com conteúdo   → add_history() + process_line()
```

---

## 3. Pipeline de parsing — `process_line()`

Para cada linha não-vazia, são executados 4 passos em sequência:

```
  linha bruta (string)
       │
       ▼
  ┌─────────┐
  │  Lexer  │   → t_token *
  └─────────┘
       │
       ▼
  ┌──────────┐
  │ Expander │   → modifica tokens in-place (expande $VAR, $?)
  └──────────┘
       │
       ▼
  ┌───────────────────────┐
  │ remove_quotes_from_   │   → remove aspas dos tokens após expansão
  │ tokens()              │
  └───────────────────────┘
       │
       ▼
  ┌────────┐
  │ Parser │   → t_cmd *
  └────────┘
       │
       ▼
  ┌──────────┐
  │ Executor │   → int (exit status)
  └──────────┘
```

### 3.1 Lexer (`lexer.c`)

Transforma a string bruta em uma lista ligada de `t_token`.

| Token              | Representa         |
|--------------------|--------------------|
| `TOKEN_WORD`       | palavras / args    |
| `TOKEN_PIPE`       | `\|`               |
| `TKN_REDIR_IN`     | `<`                |
| `TKN_REDIR_OUT`    | `>`                |
| `TKN_REDIR_APPEND` | `>>`               |
| `TKN_REDIR_HEREDOC`| `<<`               |

Regras:
- Espaços e tabs são ignorados (separadores).
- `<`, `>`, `>>`, `<<` e `|` viram tokens próprios.
- Aspas (simples e duplas) são respeitadas — delimitadores dentro de aspas fazem parte da palavra.

### 3.2 Expander (`expander.c` + `expander_utils.c`)

Percorre a lista de tokens e substitui variáveis **in-place** em cada `TOKEN_WORD`.

- `$VAR` → busca na lista `t_env` via `get_env_value()`.
- `$?` → converte `mini->last_exit_status` para string.
- **Aspas simples** (`'...'`) desativam a expansão completamente.
- **Aspas duplas** (`"..."`) permitem expansão.
- Filenames de redirecionamento (`< $ARQUIVO`) também são expandidos.
- Usa `t_exp_state` para rastrear o buffer, índice e estado de aspas durante o loop.

### 3.3 Parser (`parser.c`)

Converte a lista de tokens em uma lista ligada de `t_cmd`.

1. `validate_syntax()` — verifica erros como `|` no início/fim, `||`, redirecionamentos sem arquivo.
2. `parse_tokens()` — itera os tokens:
   - `TOKEN_WORD` → cria `t_cmd` se necessário, adiciona argumento via `add_arg_to_cmd()`.
   - `TKN_REDIR_*` → cria `t_redir` e associa ao `t_cmd` atual via `add_redir_to_cmd()`.
   - `TOKEN_PIPE` → o próximo `TOKEN_WORD` iniciará um novo `t_cmd`.

---

## 4. Execução (`executor.c`)

`execute_cmd_list()` decide o caminho de execução:

```
count_cmds == 1 && builtin?  →  exec_builtin_parent()   (no processo pai)
count_cmds == 1 && externo?  →  execute_simple_cmd()     (fork + execve)
count_cmds >= 2?             →  execute_pipeline()       (N forks + pipes)
```

### 4.1 Builtin no pai (`exec_builtin_parent`)

Builtins rodam no processo pai — precisam modificar o estado do shell.

```
dup() stdin/stdout originais
        ↓
setup_redirections()
        ↓
execute_builtin()
        ↓
restore_fds()
```

### 4.2 Comando externo simples (`execute_simple_cmd`)

```
fork()
  ├── filho: exec_child()
  │     ├── setup_child_signals()   ← reseta SIGINT/SIGQUIT para SIG_DFL
  │     ├── setup_redirections()
  │     ├── find_command_path()
  │     └── execve()
  └── pai: setup_exec_signals()     ← ignora SIGINT/SIGQUIT durante wait
           wait_child()             ← waitpid + setup_signals() + exit status
```

### 4.3 Pipeline (`execute_pipeline` + `fork_pipeline`)

```
fork_pipeline():
  Para cada cmd: pipe() → fork()
    filho: child_process() → setup_child_signals() → redirections → exec
    pai: fecha fds, passa prev_fd para próximo

setup_exec_signals()   ← pai ignora sinais durante wait

wait_all():
  waitpid() para cada filho
  último cmd determina exit status
  setup_signals()        ← restaura handlers do prompt
```

### 4.4 Sinais — três modos (`signals.c`)

| Modo         | Função              | SIGINT          | SIGQUIT    |
|--------------|---------------------|-----------------|------------|
| Prompt       | `setup_signals()`   | `handle_sigint` | `SIG_IGN`  |
| Pai c/ filho | `setup_exec_signals()` | `SIG_IGN`    | `SIG_IGN`  |
| Filho        | `setup_child_signals()` | `SIG_DFL`   | `SIG_DFL`  |

### 4.5 Redirections

| Tipo               | Arquivo                    | Operação                              |
|--------------------|----------------------------|---------------------------------------|
| `TKN_REDIR_IN`     | `redirections_utils.c`     | `open(O_RDONLY)` + `dup2(stdin)`      |
| `TKN_REDIR_OUT`    | `redirections_utils.c`     | `open(O_WRONLY\|O_CREAT\|O_TRUNC)`   |
| `TKN_REDIR_APPEND` | `redirections_utils.c`     | `open(O_WRONLY\|O_CREAT\|O_APPEND)`  |
| `TKN_REDIR_HEREDOC`| `redirections.c`           | `pipe()` + `readline()` até delimitador |

---

## 5. Builtins (`src/builtins/`)

| Builtin  | Comportamento principal                                   |
|----------|-----------------------------------------------------------|
| `echo`   | Imprime args; `-n` suprime `\n`                           |
| `cd`     | `chdir()`, atualiza `PWD`/`OLD_PWD` no `t_env`           |
| `pwd`    | Imprime diretório atual via `getcwd()`                    |
| `export` | Adiciona/atualiza variável no `t_env`                     |
| `unset`  | Remove variável do `t_env`                                |
| `env`    | Imprime todos os pares `KEY=VALUE` do `t_env`             |
| `exit`   | Seta `mini->running = FALSE`, retorna o código            |

> `exit` não chama `exit()` diretamente — sinaliza o loop para encerrar,
> garantindo que `cleanup_minishell()` rode normalmente.

---

## 6. Ambiente (`src/env/`)

O ambiente é uma **doubly linked list** de `t_env`:

```c
typedef struct s_env {
    char        *key;
    char        *value;
    struct s_env *prev;
    struct s_env *next;
} t_env;
```

Operações principais:
- `init_env(envp)` — constrói a lista a partir do `envp` do processo.
- `get_env_value(env, key)` — busca linear por `key`.
- `set_env_value(&env, key, value)` — atualiza ou cria nó.
- `unset_env_value(&env, key)` — remove nó, ajusta ponteiros.
- `env_to_array(env)` — converte para `char **` para o `execve`.
- `free_env(env)` — libera toda a lista.

---

## 7. Encerramento (`cleanup_minishell`)

Chamado após o REPL encerrar (Ctrl-D ou `exit`):

```
free_cmd_list(mini->cmd_list)
free_env(mini->env)
clear_history()
```

`main` retorna `mini.last_exit_status` como exit code do processo.

---

## 8. Ciclo de vida da memória por linha

```
readline()              → string bruta (heap, free após process_line)
lexer()                 → t_token list (heap, free_tokens após parser)
expander()              → modifica valores dos tokens in-place
remove_quotes_from_tokens() → modifica tokens in-place
parser()                → t_cmd list (heap, free_cmd_list após execute)
executor()              → fork/execve ou builtin direto
free_cmd_list()         → libera t_cmd + t_redir + args
free(line)              → libera string do readline
```

Nenhum desses recursos sobrevive entre iterações do REPL.
