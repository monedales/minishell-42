# 📚 Recursos — Minishell

Referências e conceitos importantes para entender e defender o projeto.

---

## Conceitos fundamentais

### Linked lists duplamente encadeadas

O ambiente (`t_env`) é armazenado como doubly linked list. Cada nó tem
ponteiros `prev` e `next`, o que permite remoção eficiente em qualquer
posição.

```c
typedef struct s_env {
    char        *key;
    char        *value;
    struct s_env *prev;
    struct s_env *next;
} t_env;
```

Ponto de atenção: sempre liberar `key` e `value` **antes** de liberar
o nó.

---

### Tokenização (Lexer)

O lexer transforma a string bruta em tokens. Aspas mudam as regras:
dentro de aspas, espaços e operadores como `|` e `>` fazem parte da
palavra, não são delimitadores.

Tipos de token: `TOKEN_WORD`, `TOKEN_PIPE`, `TKN_REDIR_IN`,
`TKN_REDIR_OUT`, `TKN_REDIR_APPEND`, `TKN_REDIR_HEREDOC`.

---

### Expansão de variáveis

O expander roda **depois** do lexer e **antes** do parser, modificando
os tokens in-place. Usa `t_exp_state` para rastrear buffer, índice e
estado de aspas enquanto percorre a string caractere a caractere.

- `$VAR` → busca em `t_env` via `get_env_value()`
- `$?` → converte `last_exit_status` via `ft_itoa()`
- Aspas simples bloqueiam toda expansão
- Aspas duplas permitem apenas expansão de variáveis

---

### Sinais — três modos

| Modo              | Função                  | SIGINT          | SIGQUIT    |
|-------------------|-------------------------|-----------------|------------|
| Prompt interativo | `setup_signals()`       | `handle_sigint` | `SIG_IGN`  |
| Pai com filho     | `setup_exec_signals()`  | `SIG_IGN`       | `SIG_IGN`  |
| Processo filho    | `setup_child_signals()` | `SIG_DFL`       | `SIG_DFL`  |

O filho precisa resetar para `SIG_DFL` porque herda `SIG_IGN` do pai —
sem isso, `ctrl-\` seria ignorado pelo `cat`.

A variável global `g_signal` (único global permitido pelo subject) armazena
apenas o número do sinal recebido.

---

### `execve` e o ambiente

`execve` é uma syscall — não conhece `t_env`. Espera `char **envp` no
formato `"KEY=VALUE"` terminado em NULL. Por isso existe `env_to_array()`.

```
fork()
  ├── filho: find_command_path() → execve(path, args, envp)
  └── pai: waitpid() → exit status
```

Códigos de saída relevantes:
- `0` — sucesso
- `1` — erro genérico
- `126` — execve falhou (permissão negada)
- `127` — comando não encontrado
- `128 + N` — processo terminado por sinal N (ex: 130 = Ctrl-C)

---

### Redirecionamentos e heredoc

`setup_redirections()` salva o stdin original antes de iterar a lista
de redirecionamentos. Isso é necessário para que heredocs consecutivos
leiam do terminal, não do pipe do heredoc anterior.

O heredoc usa `readline(">")` para ler até o delimitador, escrevendo
em um pipe. O read end do pipe vira o novo stdin.

---

### Saída de erros

Erros vão para `stderr` (fd 2), não para `stdout`. Isso permite que
o usuário redirecione a saída sem misturar com erros:

```bash
ls /naoexiste > saida.txt   # saida.txt vazio, erro no terminal
ls /naoexiste 2>/dev/null   # erro suprimido
```

Formato padrão: `minishell: [cmd]: [detalhe]: mensagem`

---

## Manpages essenciais

```bash
man readline
man execve
man fork
man waitpid
man pipe
man dup2
man sigaction
man tcsetattr
man open
```

---

## Links

- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html)

---

## Como rodar a norminette

```bash
# Norminette está em:
~/Library/Python/3.9/bin/norminette

# Rodar em todos os arquivos do projeto:
~/Library/Python/3.9/bin/norminette src/ include/

# Rodar em arquivos específicos:
~/Library/Python/3.9/bin/norminette src/signals/signals.c src/main.c
```

`GLOBAL_VAR_DETECTED` é apenas um aviso, não um erro — a variável
global `g_signal` é exigida pelo subject.

---

## Como rodar com valgrind

```bash
make re
valgrind --suppressions=include/valgrind.sup \
         --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./minishell
```

Leaks do `readline` são esperados e não são de responsabilidade do
projeto — o subject diz explicitamente que não precisam ser corrigidos.
