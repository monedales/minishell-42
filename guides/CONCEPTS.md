# 🧠 Minishell — Conceitos Fundamentais

**Para estudo detalhado. Consulte CHEAT_SHEET.md para referência rápida
e STUDY_PLAN.md para o plano de revisão.**

---

## 1. O que é um shell?

Um shell é um programa que lê comandos do usuário, interpreta, e
executa programas. O bash é um shell. O minishell é uma versão
simplificada do bash.

O fluxo básico de qualquer shell é o **REPL**:
- **R**ead → lê a linha do usuário
- **E**val → interpreta e executa
- **P**rint → mostra o resultado
- **L**oop → volta ao início

```
while (running):
    line = readline("minishell$ ")
    tokens = lexer(line)
    expand(tokens)
    remove_quotes(tokens)
    cmd_list = parser(tokens)
    remove_quotes_from_args(cmd_list)
    remove_quotes_from_redirs(cmd_list)
    execute(cmd_list)
```

---

## 2. Processos — fork e execve

Quando você digita `ls`, o minishell não *vira* o `ls`. Ele cria um
**processo filho** que vira o `ls`, enquanto o pai espera.

### fork()
Duplica o processo atual. O filho é uma cópia exata do pai.

```
pid_t pid = fork();
if (pid == 0)
    // sou o filho
else
    // sou o pai, pid = PID do filho
```

### execve()
Substitui o processo atual por outro programa. Se funcionar, nunca
retorna — o processo vira o novo programa.

```c
execve("/bin/ls", args, envp);
// Se chegou aqui, execve falhou
perror("ls");
exit(127);
```

### Por que fork + execve e não execve direto?
Se o minishell chamasse `execve` diretamente, **ele mesmo viraria o ls**
e nunca mais voltaria para o prompt. Com fork, o filho vira o ls e o
pai continua existindo.

### waitpid()
O pai espera o filho terminar e pega o exit status.

```c
waitpid(pid, &status, 0);
if (WIFEXITED(status))
    exit_code = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
    exit_code = 128 + WTERMSIG(status);
```

### Exit status — o que significa $?
- `0` — sucesso
- `1` — erro genérico
- `126` — arquivo encontrado mas não executável
- `127` — comando não encontrado
- `128 + N` — processo morreu pelo sinal N (ex: 130 = Ctrl+C = 128+2)

---

## 3. Pipes — conectar processos

Um pipe é um canal unidirecional entre dois processos:
um escreve, o outro lê.

```
int pipefd[2];
pipe(pipefd);
// pipefd[0] = read end (leitura)
// pipefd[1] = write end (escrita)
```

Para `ls | grep src`:
- Filho 1 (`ls`): fecha pipefd[0], conecta pipefd[1] → stdout
- Filho 2 (`grep`): fecha pipefd[1], conecta pipefd[0] → stdin
- O pai fecha os dois lados após o fork

### dup2()
Redireciona um file descriptor para outro:
```c
dup2(pipefd[1], STDOUT_FILENO); // stdout agora escreve no pipe
close(pipefd[1]);               // fecha o original
```

### Por que fechar todos os fds no pai?
Se o pai mantiver pipefd[1] aberto, o grep nunca receberá EOF — vai
ficar esperando para sempre.

---

## 4. Redirecionamentos

Redirecionamentos mudam de onde um processo lê ou para onde escreve,
antes de executar o comando.

| Operador | O que faz                          |
|----------|------------------------------------|
| `< file` | stdin vem do arquivo               |
| `> file` | stdout vai para o arquivo (trunca) |
| `>> file`| stdout vai para o arquivo (anexa)  |
| `<< DEL` | stdin vem de um "pipe virtual"     |

Todos usam `open()` + `dup2()` internamente.

### Heredoc (`<<`)
Lê linhas interativamente até encontrar o delimitador. Armazena tudo
em um pipe, depois conecta esse pipe ao stdin do comando.

```
cat << EOF
linha 1
linha 2
EOF
```

**Coleta no pai**: Todos os heredocs são coletados em `collect_all_heredocs()`
ANTES de qualquer fork. Isso evita que readline execute dentro de um
processo filho, prevenindo leaks de "still reachable".

**Expansão no heredoc**: Se o delimitador NÃO tem aspas (`<< EOF`),
variáveis como `$VAR` são expandidas dentro do heredoc. Se tem aspas
(`<< 'EOF'`), o conteúdo é literal.

**Sinais no heredoc**: `setup_heredoc_signals()` instala um handler
específico (`handle_sigint_heredoc`) e o `heredoc_event_hook` para que
Ctrl+C interrompa a leitura e retorne ao prompt.

---

## 5. Tokenização — como o shell lê a linha

Antes de executar, a linha precisa ser quebrada em pedaços (tokens).

`echo "hello world" | cat -n`

```
[WORD:"echo"] [WORD:"hello world"] [PIPE] [WORD:"cat"] [WORD:"-n"]
```

Regras importantes:
- Espaços e tabs são separadores (ignorados entre tokens)
- `|`, `<`, `>`, `<<`, `>>` são tokens próprios
- Aspas mudam as regras: dentro de `"..."` ou `'...'`, espaços e
  operadores fazem parte da palavra, não são separadores

### Máquina de estados de aspas

O lexer rastreia em qual estado de aspas está:

```
QUOTE_NONE   → estado normal
QUOTE_SINGLE → dentro de '...' (tudo literal)
QUOTE_DOUBLE → dentro de "..." ($ ainda expande)
```

Transições: `'` alterna NONE ↔ SINGLE, `"` alterna NONE ↔ DOUBLE.

---

## 6. Expansão de variáveis

Depois de tokenizar, o expander percorre cada token e substitui
variáveis pelos seus valores.

```
$USER   → busca "USER" no t_env → retorna o valor
$?      → converte last_exit_status para string
$NADA   → variável não existe → string vazia ""
```

Regras de aspas:
- `'$USER'` → aspas simples bloqueiam, sai literal `$USER`
- `"$USER"` → aspas duplas permitem, expande normalmente
- `$USER` sem aspas → expande normalmente

### Por que expandir antes do parser?
O parser constrói a lista de comandos a partir dos tokens já expandidos.
Se `$CMD` vale `ls`, o parser precisa ver `ls`, não `$CMD`.

### Por que remover aspas depois da expansão?
As aspas precisam existir durante a expansão para controlar o que
expande ou não. Só depois que a expansão acontece, as aspas são
removidas para que o comando receba os argumentos limpos.

---

## 7. Sinais

Sinais são notificações assíncronas enviadas a processos.

| Sinal   | Número | Ctrl  | Comportamento padrão      |
|---------|--------|-------|---------------------------|
| SIGINT  | 2      | Ctrl-C| Termina o processo        |
| SIGQUIT | 3      | Ctrl-\| Termina + core dump       |

### Quatro modos no minishell

**Modo prompt (pai sem filho):**
- SIGINT → `handle_sigint`: `g_signal=130` + nova linha + limpa readline
- SIGQUIT → ignorado (SIG_IGN)
- `prompt_event_hook` → verifica `g_signal`, seta `rl_done=1`

**Modo heredoc (lendo input de heredoc):**
- SIGINT → `handle_sigint_heredoc`: `g_signal=130` + `rl_done=1`
- SIGQUIT → ignorado (SIG_IGN)
- `heredoc_event_hook` → verifica `g_signal`, seta `rl_done=1`

**Modo execução (pai com filho rodando):**
- SIGINT → ignorado (SIG_IGN) — o filho trata
- SIGQUIT → ignorado (SIG_IGN) — o filho trata

**Modo filho (após fork, antes de execve):**
- SIGINT → padrão (SIG_DFL) — filho morre normalmente
- SIGQUIT → padrão (SIG_DFL) — filho morre com core dump

### Por que o filho precisa resetar sinais?
O filho herda os handlers do pai. Se o pai tem SIGQUIT = SIG_IGN,
o filho também ignora — e `ctrl-\` não funcionaria no `cat`.
`setup_child_signals()` reseta para SIG_DFL logo após o fork.

### Event hooks do readline
Em vez de usar `SA_RESTART`, o minishell usa **event hooks** (`rl_event_hook`)
que o readline chama periodicamente. O hook verifica se `g_signal == 130`
e seta `rl_done = 1` para forçar readline a retornar. Isso permite uma
interrupção limpa tanto no prompt quanto no heredoc.

---

## 8. Ambiente (variáveis de ambiente)

O ambiente é uma lista ligada de pares `KEY=VALUE`.

```c
t_env {
    char *key;    // "HOME"
    char *value;  // "/Users/mona"
    t_env *prev;
    t_env *next;
}
```

É doubly linked (tem `prev`) para permitir remoção eficiente em
qualquer posição sem percorrer a lista toda.

### Por que t_env e não char**?
`char **envp` é o formato do sistema — difícil de modificar (adicionar,
remover, atualizar). A `t_env` permite operações O(n) simples.

### env_to_array()
Converte `t_env` para `char **` no momento do execve, porque o
kernel não conhece nossa estrutura.

---

## 9. Builtins — por que rodam no pai?

Builtins como `cd`, `export` e `exit` precisam modificar o estado
do próprio shell. Se rodassem em um processo filho:
- `cd /tmp` mudaria o diretório do filho, não do shell
- `export X=1` adicionaria a variável no filho, não no shell
- `exit` encerraria o filho, não o shell

Por isso `execute_cmd_list()` checa `is_builtin()` antes de fazer fork.
Se for builtin, roda diretamente no pai com `exec_builtin_parent()`.

Redirecionamentos ainda funcionam para builtins: o pai salva stdin/stdout,
aplica os redirecionamentos, executa o builtin, e restaura.

---

## 10. Memória — ciclo de vida por linha

Cada linha executada cria e destrói estas estruturas:

```
readline()  → string heap       → free(line) no final do loop
lexer()     → t_token list      → free_tokens() após o parser
expander()  → modifica in-place  → sem malloc novo
parser()    → t_cmd + t_redir   → free_cmd_list() após executor
heredocs    → pipe fds          → fechados durante free_cmd_list
executor()  → fork/execve       → processos filhos, não heap do pai
```

Nada sobrevive entre iterações. `readline()` tem leaks internos
que o subject permite ignorar — mas todo o nosso código deve ser
livre de leaks.
