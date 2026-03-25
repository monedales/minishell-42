# Recursos de Estudo — Minishell

Explicações simples dos conceitos que você precisa dominar para entender
as áreas de `env/`, `error_utils` e `parsing/` do projeto.

---

## 1. Linked Lists duplamente encadeadas em C

### O que é?

Uma linked list é uma estrutura onde cada elemento (nó) aponta para o
próximo. Na versão **duplamente encadeada**, cada nó também aponta para
o anterior — o que facilita operações como remoção no meio da lista.

### Estrutura básica

```c
typedef struct s_env
{
    char          *key;    // "HOME"
    char          *value;  // "/Users/mona"
    struct s_env  *prev;   // nó anterior
    struct s_env  *next;   // próximo nó
}   t_env;
```

### Visualização

```
NULL ← [HOME=/Users/mona] ↔ [PATH=/usr/bin] ↔ [USER=mona] → NULL
         ^head                                      ^tail
```

### Operações essenciais

**Criar um nó:**
```c
t_env *node = malloc(sizeof(t_env));
node->key   = ft_strdup("HOME");
node->value = ft_strdup("/Users/mona");
node->prev  = NULL;
node->next  = NULL;
```

**Adicionar no final:**
```c
// Percorre até o último nó e encadeia
while (current->next != NULL)
    current = current->next;
current->next = new_node;
new_node->prev = current;
```

**Remover um nó do meio:**
```c
// Antes:  A ↔ B ↔ C
// Depois: A ↔ C  (B liberado)
node->prev->next = node->next;
node->next->prev = node->prev;
free(node->key);
free(node->value);
free(node);
```

> ⚠️ Sempre libere `key`, `value` e o nó em si — nessa ordem.
> Se liberar o nó primeiro, você perde acesso aos campos internos.

### Armadilhas comuns

| Erro | Consequência |
|---|---|
| Esquecer de ajustar `prev` ao remover | Ponteiro dangling |
| Não checar `NULL` antes de acessar `->next` | Segfault |
| Liberar o nó antes dos campos `key`/`value` | Memory leak |

---

## 2. Tokenização / Lexer — Como o shell lê sua linha

### O que é tokenização?

Quando você digita `echo "hello world" | cat -e`, o shell não vê isso
como uma string. Ele quebra em pedaços chamados **tokens**, cada um
com um tipo:

```
echo         → TOKEN_WORD
"hello world"→ TOKEN_WORD  (com aspas, tratado como uma unidade)
|            → TOKEN_PIPE
cat          → TOKEN_WORD
-e           → TOKEN_WORD
```

### Os tipos de token do minishell

| Token | Símbolo | Exemplo |
|---|---|---|
| `TOKEN_WORD` | qualquer palavra | `echo`, `ls`, `-la` |
| `TOKEN_PIPE` | `\|` | `cmd1 \| cmd2` |
| `TKN_REDIR_IN` | `<` | `< arquivo.txt` |
| `TKN_REDIR_OUT` | `>` | `> saida.txt` |
| `TKN_REDIR_APPEND` | `>>` | `>> saida.txt` |
| `TKN_REDIR_HEREDOC` | `<<` | `<< EOF` |
| `TOKEN_EOF` | fim | fim da entrada |

### Como o lexer funciona (passo a passo)

Dado o input: `ls -la > out.txt`

```
1. Pula espaços
2. Lê "ls"       → TOKEN_WORD("ls")
3. Pula espaço
4. Lê "-la"      → TOKEN_WORD("-la")
5. Pula espaço
6. Lê ">"        → TKN_REDIR_OUT
7. Pula espaço
8. Lê "out.txt"  → TOKEN_WORD("out.txt")
9. Fim           → TOKEN_EOF
```

### Aspas mudam as regras

Dentro de aspas, espaços e `|` e `>` **não** são delimitadores:

```
"hello world"  → um único TOKEN_WORD (não dois)
'ls | cat'     → um único TOKEN_WORD (o pipe é literal)
```

O lexer rastreia se está dentro de aspas simples ou duplas antes de
decidir se um caractere é delimitador ou não.

---

## 3. Expansão de Variáveis — O que o `$` faz

### Tipos de expansão no bash (e no minishell)

| Sintaxe | O que faz | Exemplo |
|---|---|---|
| `$VAR` | Substitui pelo valor da variável | `$HOME` → `/Users/mona` |
| `$?` | Substitui pelo exit status do último comando | `$?` → `0` ou `1` |
| Aspas duplas `"..."` | Permitem expansão dentro delas | `"$HOME"` expande |
| Aspas simples `'...'` | Bloqueiam toda expansão | `'$HOME'` não expande |

### Como a expansão funciona no código

O expander percorre a string caractere por caractere e constrói um
novo buffer:

```
Input:  "hello $USER!"
         h e l l o   $ U S E R !
                     ^
                     Encontrou '$'
                     → lê o nome da variável: "USER"
                     → busca o valor: "mona"
                     → insere "mona" no buffer no lugar de "$USER"

Output: "hello mona!"
```

### Estados durante a expansão

```
Caractere normal  → copia para o buffer
$                 → inicia leitura do nome da variável
  nome da var     → busca no env e substitui
  $?              → substitui pelo last_exit_status
Dentro de ' '     → nada é expandido, copia tudo literal
```

### Casos especiais para testar

```bash
echo $USER          # expande: mona
echo "$USER"        # expande: mona  (aspas duplas permitem)
echo '$USER'        # literal: $USER (aspas simples bloqueiam)
echo ""             # string vazia
echo $NAOEXISTE     # vazio (variável não definida = string vazia)
ls /x; echo $?      # 1 (ls falhou)
echo $?             # 0 (echo teve sucesso)
```

---

## 4. `execve` — Como o shell executa programas

### Por que o shell precisa do `execve`?

Quando você digita `ls`, o minishell não sabe "ls" — ele precisa
encontrar o binário `/bin/ls` e executá-lo. O `execve` substitui o
processo atual pelo programa especificado.

### Assinatura

```c
int execve(const char *path, char *const argv[], char *const envp[]);
```

| Parâmetro | O que é | Exemplo |
|---|---|---|
| `path` | Caminho absoluto do executável | `/bin/ls` |
| `argv` | Array de argumentos (NULL-terminado) | `["ls", "-la", NULL]` |
| `envp` | Variáveis de ambiente (NULL-terminado) | `["HOME=/...", "PATH=/...", NULL]` |

### Por que `char **envp` e não `t_env *`?

O `execve` é uma syscall do sistema operacional. Ele não conhece sua
estrutura `t_env`. Ele espera um formato padrão: array de strings
`"CHAVE=VALOR"` terminado em `NULL`.

Por isso existe a função `env_to_array`:

```c
// Converte: t_env* → char**
// [HOME] → [/Users/mona]   →   "HOME=/Users/mona"
// [PATH] → [/usr/bin]      →   "PATH=/usr/bin"
//                          →   NULL

char **envp = env_to_array(mini->env);
execve(path, cmd->args, envp);
// Após o exec (se falhar), liberar envp
free_split(envp);
```

### O que acontece após o `execve`?

- **Se suceder:** o processo atual é substituído — as linhas após o
  `execve` **nunca são executadas**.
- **Se falhar:** retorna `-1` e `errno` é definido com o motivo.

```c
execve(path, args, envp);
// Se chegou aqui, execve falhou
perror("execve");
exit(127);
```

### Fluxo completo no minishell

```
fork()
  ├── processo filho:
  │     find_command_path("ls") → "/bin/ls"
  │     env_to_array(env)       → char **envp
  │     execve("/bin/ls", args, envp)
  │     // nunca retorna se tiver sucesso
  │
  └── processo pai:
        waitpid(child_pid, &status, 0)
        // atualiza last_exit_status
```

---

## 5. Saída de Erros — `stderr` vs `stdout`

### Por que dois canais de saída?

Todo processo Unix tem três descritores de arquivo padrão:

| FD | Nome | Para que serve | Macro em C |
|---|---|---|---|
| `0` | `stdin` | Entrada (teclado) | `STDIN_FILENO` |
| `1` | `stdout` | Saída normal | `STDOUT_FILENO` |
| `2` | `stderr` | Erros e avisos | `STDERR_FILENO` |

Erros **devem** ir para `stderr`, não para `stdout`. Isso permite que
o usuário redirecione a saída sem misturar erros:

```bash
ls /naoexiste > saida.txt
# "saida.txt" fica vazio
# O erro aparece no terminal (stderr não foi redirecionado)

ls /naoexiste > saida.txt 2>/dev/null
# Nada aparece (stderr também redirecionado)
```

### Como escrever em `stderr` em C

```c
// Jeito 1: ft_putstr_fd (usado no minishell)
ft_putstr_fd("minishell: comando: mensagem\n", STDERR_FILENO);

// Jeito 2: write diretamente
write(STDERR_FILENO, "erro\n", 5);

// Jeito 3: fprintf (evite — usa printf da libc)
fprintf(stderr, "erro: %s\n", mensagem);
```

### Formato de erro do bash (e do minishell)

O bash usa um padrão consistente para mensagens de erro:

```
minishell: [comando]: [detalhe]: mensagem de erro
```

Exemplos reais:
```
minishell: cd: /tmp/x: No such file or directory
minishell: exit: abc: numeric argument required
minishell: syntax error near unexpected token '|'
minishell: ls: command not found
```

### Como o `handle_error` monta isso

```c
// handle_error(ERR_NO_FILE, "cd", "/tmp/x")
// Produz: minishell: cd: /tmp/x: No such file or directory

ft_putstr_fd("minishell: ", STDERR_FILENO);  // prefixo fixo
ft_putstr_fd("cd", STDERR_FILENO);           // cmd (se não NULL)
ft_putstr_fd(": ", STDERR_FILENO);
ft_putstr_fd("/tmp/x", STDERR_FILENO);       // detail (se não NULL)
ft_putstr_fd(": ", STDERR_FILENO);
ft_putstr_fd("No such file or directory", STDERR_FILENO); // msg do enum
ft_putstr_fd("\n", STDERR_FILENO);
```

### Exit status de erros comuns

| Situação | Exit status |
|---|---|
| Comando executado com sucesso | `0` |
| Erro genérico / sintaxe | `1` |
| Uso incorreto de builtin | `1` |
| Comando não encontrado | `127` |
| Permissão negada | `126` |
| Sinal (ex: Ctrl+C) | `128 + número do sinal` |

---

## Resumo Visual — Como tudo se conecta

```
Input do usuário: echo "$HOME" | cat
         │
         ▼
    ┌─────────┐
    │  LEXER  │  Quebra em tokens: [echo]["$HOME"][|][cat]
    └────┬────┘
         │
         ▼
    ┌──────────┐
    │ EXPANDER │  Expande variáveis: "$HOME" → "/Users/mona"
    └────┬─────┘
         │
         ▼
    ┌─────────┐
    │ PARSER  │  Monta t_cmd: {args=["echo","/Users/mona"]} → {args=["cat"]}
    └────┬────┘
         │
         ▼
    ┌──────────────┐
    │   EXECUTOR   │  fork() + execve() para cada cmd
    └────┬─────────┘
         │
    erros? → handle_error() → stderr
    sucesso? → atualiza $? → volta pro prompt
```
