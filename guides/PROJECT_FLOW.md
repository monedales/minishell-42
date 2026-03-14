# 🔄 Minishell — Fluxo Completo do Projeto

Este guia explica, em ordem de execução, como o minishell funciona desde a inicialização até o encerramento.

---

## Visão geral

```
main()
 ├── init_minishell()       → cria t_mini + lista de env
 ├── setup_signals()        → configura SIGINT / SIGQUIT
 ├── repl_loop()            → loop principal (REPL)
 │    └── process_line()    → pipeline de parsing + execução
 │         ├── lexer()
 │         ├── expand_tokens()
 │         ├── parser()
 │         └── execute_cmd_list()
 └── cleanup_minishell()    → libera toda a memória
```

---

## 1. Inicialização (`main.c`)

### `init_minishell()`
- Recebe `envp` (array de strings `CHAVE=VALOR` do processo pai).
- Chama `init_env(envp)` que converte cada string em um nó `t_env` (doubly linked list).
- Inicializa `mini.running = TRUE`, `mini.last_exit_status = 0`, `mini.cmd_list = NULL`.

### `setup_signals()`
- Configura dois handlers com `sigaction`:
  - **SIGINT (Ctrl-C)**: chama `handle_sigint()` → seta `g_signal = 130`, reescreve o prompt.
  - **SIGQUIT (Ctrl-\\)**: ignorado (`SIG_IGN`), igual ao bash interativo.
- Usa `SA_RESTART` para que `readline` não seja interrompido no meio de uma leitura.

---

## 2. Loop principal — REPL (`repl_loop`)

O REPL (**Read–Eval–Print Loop**) é um `while (mini->running)`:

```
readline("minishell$ ")
        │
        ├── NULL (Ctrl-D)         → imprime "exit\n", encerra loop
        ├── g_signal == 130       → atualiza last_exit_status, g_signal = 0
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
  │ Expander │   → modifica tokens in-place
  └──────────┘
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

| Token         | Representa        |
|---------------|-------------------|
| `TOKEN_WORD`  | palavras / args   |
| `TOKEN_PIPE`  | `\|`              |
| `TKN_REDIR_IN`| `<`               |
| `TKN_REDIR_OUT`| `>`              |
| `TKN_REDIR_APPEND`| `>>`         |
| `TKN_REDIR_HEREDOC`| `<<`        |

Regras:
- Espaços e tabs são ignorados (separadores).
- `<`, `>`, `>>`, `<<` e `|` viram tokens próprios.
- Aspas (simples e duplas) são respeitadas: delimitadores dentro de aspas fazem parte da palavra.

Exemplo:
```
"echo hello | cat -n"
→ [WORD:"echo"] [WORD:"hello"] [PIPE] [WORD:"cat"] [WORD:"-n"]
```

### 3.2 Expander (`expander.c`)

Percorre a lista de tokens e substitui variáveis **in-place** em cada `TOKEN_WORD`.

- `$VAR` → busca na lista `t_env` via `get_env_value()`.
- `$?` → converte `mini->last_exit_status` para string.
- **Aspas simples** (`'...'`) desativam a expansão completamente.
- **Aspas duplas** (`"..."`) permitem expansão.
- Filenames de redirecionamento (`< $ARQUIVO`) também são expandidos.

Exemplo:
```
USER=mona, last_exit_status=0
"echo $USER $?"  →  "echo mona 0"
'echo $USER'     →  'echo $USER'   (sem expansão)
```

### 3.3 Parser (`parser.c`)

Converte a lista de tokens em uma lista ligada de `t_cmd`.

**Etapas internas:**
1. `validate_syntax()` — verifica erros como `|` no início/fim, `||`, redirecionamentos sem arquivo.
2. `parse_tokens()` — itera os tokens:
   - `TOKEN_WORD` → cria `t_cmd` se necessário, adiciona argumento via `add_arg_to_cmd()`.
   - `TKN_REDIR_*` → cria `t_redir` e associa ao `t_cmd` atual via `add_redir_to_cmd()`.
   - `TOKEN_PIPE` → o próximo `TOKEN_WORD` iniciará um novo `t_cmd`.

Estrutura resultante para `echo hi | cat -n`:
```
t_cmd[0]: args=["echo","hi"], redirs=NULL, next→
t_cmd[1]: args=["cat","-n"], redirs=NULL, next=NULL
```

---

## 4. Execução (`executor.c`)

`execute_cmd_list()` decide o caminho de execução:

```
count_cmds == 1 && builtin?  →  exec_builtin_parent()   (no processo pai)
count_cmds == 1 && externo?  →  execute_simple_cmd()     (fork + execve)
count_cmds >= 2?             →  execute_pipeline()       (N forks + pipes)
```

### 4.1 Builtin no pai (`exec_builtin_parent`)

Builtins (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`) rodam no processo pai pois precisam modificar o estado do shell (variáveis de ambiente, diretório atual, etc.).

Fluxo:
```
dup() de stdin/stdout originais
        ↓
setup_redirections()   (aplica <, >, >>, <<)
        ↓
execute_builtin()      (chama a função builtin)
        ↓
restore_fds()          (restaura stdin/stdout)
```

### 4.2 Comando externo simples (`execute_simple_cmd`)

```
fork()
  ├── filho: exec_child()
  │     ├── setup_redirections()
  │     ├── find_command_path()   (percorre PATH)
  │     └── execve()
  └── pai: wait_child()   (waitpid + extrai exit status)
```

Códigos de saída:
- `127` — comando não encontrado.
- `126` — encontrado mas não executável (falha de `execve`).
- `128 + N` — processo terminado por sinal N.

### 4.3 Pipeline (`execute_pipeline`)

Para `cmd1 | cmd2 | cmd3`:

```
Iteration 1: pipe() → fork() filho executa cmd1 com stdout→pipe[1]
Iteration 2: pipe() → fork() filho executa cmd2 com stdin←pipe[0], stdout→pipe[1]
Iteration 3: fork()  → filho executa cmd3 com stdin←pipe[0]
Pai: wait_all() → coleta todos os filhos, retorna status do último
```

### 4.4 Redirections (`redirections.c`)

| Tipo              | Operação                            |
|-------------------|-------------------------------------|
| `TKN_REDIR_IN`    | `open(O_RDONLY)` + `dup2(stdin)`    |
| `TKN_REDIR_OUT`   | `open(O_WRONLY\|O_CREAT\|O_TRUNC)` + `dup2(stdout)` |
| `TKN_REDIR_APPEND`| `open(O_WRONLY\|O_CREAT\|O_APPEND)` + `dup2(stdout)` |
| `TKN_REDIR_HEREDOC`| `pipe()` + `readline()` até delimitador + `dup2(stdin)` |

### 4.5 Path resolution (`path_finder.c`)

Para resolver `ls` → `/usr/bin/ls`:
1. Se `cmd` contém `/`, usa diretamente (`ft_strdup`).
2. Busca `PATH` no `t_env`.
3. `ft_split(PATH, ':')` → array de diretórios.
4. Para cada dir, testa `dir/cmd` com `access(X_OK)`.
5. Retorna o primeiro caminho válido ou `NULL`.

---

## 5. Builtins (`src/builtins/`)

| Builtin   | Comportamento principal                                |
|-----------|--------------------------------------------------------|
| `echo`    | Imprime args; `-n` suprime `\n`                        |
| `cd`      | `chdir()`, atualiza `PWD`/`OLDPWD` no `t_env`          |
| `pwd`     | Imprime diretório atual via `getcwd()`                 |
| `export`  | Adiciona/atualiza variável no `t_env`                  |
| `unset`   | Remove variável do `t_env`                             |
| `env`     | Imprime todos os pares `KEY=VALUE` do `t_env`          |
| `exit`    | Seta `mini->running = FALSE`, retorna o código         |

> `exit` não chama `exit()` direto — sinaliza o loop para encerrar, garantindo que `free_cmd_list()` e `cleanup_minishell()` rodem normalmente.

---

## 6. Ambiente (`src/env/`)

O ambiente é armazenado como uma **doubly linked list** de `t_env`:

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
free_cmd_list(mini->cmd_list)   → libera estruturas do último comando (se houver)
free_env(mini->env)             → libera toda a lista de variáveis de ambiente
clear_history()                 → libera histórico do readline
```

O `main` retorna `mini.last_exit_status` como exit code do processo.

---

## 8. Estruturas principais

```c
t_mini {
    t_env *env;             // lista de variáveis de ambiente
    t_cmd *cmd_list;        // lista de comandos atual
    int   last_exit_status; // valor de $?
    int   running;          // flag do REPL
}

t_token { type, value, *next }
t_cmd   { char **args, t_redir *redirs, pid_t pid, *next }
t_redir { type, char *file, *next }
t_env   { char *key, char *value, *prev, *next }
```

---

## 9. Diagrama de memória — ciclo de vida por linha

```
readline() → string bruta (heap, free após process_line)
    ↓
lexer()    → t_token list (heap, free_tokens após parser)
    ↓
expander() → modifica valores dos tokens in-place
    ↓
parser()   → t_cmd list (heap, free_cmd_list após execute)
    ↓
executor() → fork/execve ou builtin direto
    ↓
free_cmd_list()  → libera t_cmd + t_redir + args
free(line)       → libera string do readline
```

Nenhum desses recursos sobrevive entre iterações do REPL.
