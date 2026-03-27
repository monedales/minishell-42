# 📝 Minishell — Cheat Sheet

**Para copiar no caderno e fixar o conteúdo.**
**Referência digital rápida → PROJECT_FLOW.md**
**Conceitos detalhados → CONCEPTS.md**

---

## 🔄 Fluxo do main (copiar como diagrama)

```
main()
  │
  ├─ init_minishell()   → envp → t_env list
  ├─ setup_signals()    → SIGINT=handler / SIGQUIT=IGN
  │
  └─ repl_loop():
       readline()
         ├─ NULL + ctrl-D      → "exit\n", para
         ├─ g_signal == 130    → atualiza $?, continua
         └─ linha com texto    → process_line():
              lexer()                → tokens
              expand_tokens()        → $VAR, $?
              remove_quotes()        → limpa aspas
              parser()               → t_cmd list
              execute_cmd_list()     → roda
              free_cmd_list()        → libera
```

---

## 📦 Estruturas (copiar como caixinhas)

```
t_mini                    t_env
┌──────────────────┐      ┌─────────────┐
│ env → t_env list │      │ key: "HOME" │
│ cmd_list         │      │ value: "/…" │
│ last_exit_status │      │ prev ←      │
│ running: TRUE    │      │ next →      │
└──────────────────┘      └─────────────┘

t_cmd                     t_token
┌──────────────────┐      ┌──────────────┐
│ args: ["ls",NULL]│      │ type: WORD   │
│ redirs → list    │      │ value: "ls"  │
│ pid              │      │ next →       │
│ next →           │      └──────────────┘
└──────────────────┘
```

---

## ⚙️ Três caminhos de execução

| Situação            | Função                  | Onde roda |
|---------------------|-------------------------|-----------|
| builtin sozinho     | `exec_builtin_parent()` | **pai**   |
| externo sozinho     | `execute_simple_cmd()`  | filho     |
| pipeline (N cmds)   | `execute_pipeline()`    | N filhos  |

### Por que builtin no pai?
`cd`, `export`, `exit` mudam o **estado do shell**.
Um filho mudaria só a si mesmo e o shell não saberia.

---

## 🔐 Sinais — três modos (copiar como tabela)

| Quando            | Função                   | SIGINT          | SIGQUIT   |
|-------------------|--------------------------|-----------------|-----------|
| prompt            | `setup_signals()`        | `handle_sigint` | `SIG_IGN` |
| pai c/ filho      | `setup_exec_signals()`   | `SIG_IGN`       | `SIG_IGN` |
| filho (pós-fork)  | `setup_child_signals()`  | `SIG_DFL`       | `SIG_DFL` |

**handle_sigint:** `g_signal=130` + `write("\n")` + limpa readline + redesenha prompt

**Por que filho reseta?**
Herda `SIG_IGN` do pai → ctrl-\ seria ignorado pelo `cat` sem o reset.

**Por que g_signal = 130?**
Exit status de processo morto por sinal = 128 + N. SIGINT = 2. 128+2 = **130**.

**Por que SA_RESTART?**
Sem ele, no macOS o readline retorna NULL a cada ctrl-C causando loop infinito.

---

## 🔤 Parsing — ordem e por quê

```
string → lexer → tokens → expand → remove_quotes → parser → t_cmd
```

| Passo           | O que faz                          | Por quê nessa ordem               |
|-----------------|------------------------------------|-----------------------------------|
| lexer           | string → tokens, respeita aspas    | aspas precisam estar intactas     |
| expand_tokens   | $VAR/$? in-place nos tokens        | aspas controlam o que expande     |
| remove_quotes   | remove `'` e `"` dos valores       | parser recebe argumentos limpos   |
| parser          | tokens → t_cmd list                | já tem os valores finais          |

### Tipos de token

| Token              | Símbolo | Exemplo       |
|--------------------|---------|---------------|
| `TOKEN_WORD`       | palavra | `ls`, `-la`   |
| `TOKEN_PIPE`       | `\|`    | `\|`          |
| `TKN_REDIR_IN`     | `<`     | `< file`      |
| `TKN_REDIR_OUT`    | `>`     | `> file`      |
| `TKN_REDIR_APPEND` | `>>`    | `>> file`     |
| `TKN_REDIR_HEREDOC`| `<<`    | `<< EOF`      |

---

## 💲 Expansão — regras de aspas (copiar como tabela)

| Input          | Expande? | Output        |
|----------------|----------|---------------|
| `$USER`        | ✅        | `mona`        |
| `"$USER"`      | ✅        | `mona`        |
| `'$USER'`      | ❌        | `$USER`       |
| `$?`           | ✅        | exit status   |
| `$NAOEXISTE`   | ✅        | `""` (vazio)  |
| `"valor: $?"`  | ✅        | `valor: 0`    |

---

## 🧰 Builtins — resumo (copiar como tabela)

| Builtin  | Detalhe importante                                    |
|----------|-------------------------------------------------------|
| `echo`   | `-n` suprime newline; múltiplos `-n` aceitos          |
| `cd`     | sem arg → `$HOME`; atualiza `PWD` e `OLD_PWD`         |
| `pwd`    | `getcwd()` diretamente                               |
| `export` | sem arg → `declare -x KEY="VALUE"` para cada var      |
| `unset`  | var inexistente → silencioso (não é erro)             |
| `env`    | imprime `KEY=VALUE` para cada nó do `t_env`           |
| `exit`   | `mini->running = FALSE`, não chama `exit()` direto    |

---

## 📊 Exit status — decorar

| Valor     | Significa                              |
|-----------|----------------------------------------|
| `0`       | sucesso                                |
| `1`       | erro genérico / uso errado de builtin  |
| `2`       | uso errado de builtin (ex: exit abc)   |
| `126`     | arquivo existe mas não é executável    |
| `127`     | comando não encontrado                 |
| `128 + N` | processo morto pelo sinal N            |
| `130`     | Ctrl+C (128 + SIGINT=2)                |
| `131`     | Ctrl+\ (128 + SIGQUIT=3)               |

---

## 🔁 Pipeline — como os FDs se conectam

Para `ls | grep src | wc -l`:

```
Processo ls:    stdout → pipe1[1]
Processo grep:  stdin  ← pipe1[0]  /  stdout → pipe2[1]
Processo wc:    stdin  ← pipe2[0]

Pai fecha todos os fds após cada fork.
Se pai não fechar pipe1[1], grep nunca recebe EOF.
```

---

## 🗂️ Redirecionamentos — como funcionam

| Operador | Função usada  | O que faz internamente                      |
|----------|---------------|---------------------------------------------|
| `<`      | `redir_in()`  | `open(O_RDONLY)` + `dup2(fd, STDIN)`        |
| `>`      | `redir_out()` | `open(O_WRONLY\|O_CREAT\|O_TRUNC)` + dup2  |
| `>>`     | `redir_out()` | `open(O_WRONLY\|O_CREAT\|O_APPEND)` + dup2 |
| `<<`     | `redir_heredoc()` | `pipe()` + readline até delimitador    |

**Heredoc salva orig_stdin porque:**
múltiplos heredocs consecutivos precisam ler do terminal,
não do pipe deixado pelo heredoc anterior.

---

## ❓ Flashcards de revisão

Cobre as respostas e testa uma por vez:

**Por que fork antes do execve?**
→ execve substitui o processo. Sem fork, o minishell viraria o ls e nunca voltaria ao prompt.

**Por que t_env é doubly linked?**
→ Para remover um nó do meio sem percorrer a lista toda (usa `prev->next` e `next->prev`).

**Por que expandir antes de remover aspas?**
→ As aspas controlam o que expande. Se remover antes, `'$USER'` expandiria erroneamente.

**O que env_to_array() faz e por quê existe?**
→ Converte `t_env` para `char**` no formato `"KEY=VALUE"`. O execve não conhece nossa struct.

**Por que setup_redirections salva orig_stdin?**
→ Para heredocs consecutivos lerem do terminal, não do pipe do heredoc anterior.

**O que acontece com ctrl-C com cat rodando?**
→ Pai tem SIG_IGN (setup_exec_signals). Filho tem SIG_DFL → morre. Pai detecta WIFSIGNALED → escreve `\n` → chama setup_signals().

**O que acontece com ctrl-\ no prompt vazio?**
→ Nada. O pai tem SIGQUIT = SIG_IGN no modo prompt.

**Por que exit não chama exit() diretamente?**
→ Para que cleanup_minishell() rode e libere toda a memória. Seta mini->running = FALSE e o loop encerra limpo.

**O que validate_syntax checa?**
→ Pipe no início, pipe no final, dois pipes seguidos, redirecionamento sem arquivo depois.

**O que faz `echo $?; echo $?`?**
→ Primeiro `echo $?` imprime o exit status do comando anterior. Segundo imprime `0` (o echo teve sucesso).

---

## ✅ Checklist final de avaliação

```
Funcionalidade:
[ ] Prompt + readline funciona
[ ] Histórico (seta ↑)
[ ] Executa binários via PATH
[ ] Caminhos absolutos e relativos
[ ] Pipes simples e múltiplos
[ ] < > >> funcionam
[ ] Heredoc << funciona
[ ] $VAR expande
[ ] $? atualiza corretamente
[ ] '' bloqueia expansão
[ ] "" permite expansão de $

Sinais:
[ ] ctrl-C no prompt → nova linha (1 prompt só)
[ ] ctrl-C com filho → mata filho, volta prompt
[ ] ctrl-\ no prompt → não faz nada
[ ] ctrl-\ com filho → Quit (core dumped)
[ ] ctrl-D → sai com "exit"

Builtins:
[ ] echo, echo -n
[ ] cd, cd sem arg, cd caminho inválido
[ ] pwd
[ ] export com e sem arg
[ ] unset
[ ] env
[ ] exit com código, exit abc, exit 1 2

Erros:
[ ] Mensagens no formato correto → stderr
[ ] exit status correto (127, 126, etc.)
[ ] Sintaxe inválida exibe erro

Build:
[ ] make re sem warnings
[ ] valgrind sem leaks (exceto readline)
[ ] norminette OK
```
