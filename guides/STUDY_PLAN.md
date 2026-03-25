# Minishell — Study Plan: `env/` + `error_utils` + `parsing/`

**Tempo estimado:** 3–4 dias  
**Status do projeto:** Quase pronto — foco em revisão profunda, edge cases e polimento  
**Conceitos-chave:** Linked lists, enums C, tokenization, state machines, expansão de variáveis

---

## Pré-requisitos ✅

Antes de estudar cada parte, verifique se você domina:
- [ ] Linked lists duplamente encadeadas em C (prev/next)
- [ ] `enum` em C e indexação por enum
- [ ] State machines simples (ex: estados de aspas)
- [ ] `STDERR_FILENO` e saída de erros no shell
- [ ] Como o bash processa: `echo "$VAR"` vs `echo '$VAR'`

---

## Dia 1 — `env/` (Linked List de Variáveis de Ambiente)

### Sessão 1 — Manhã (2h): Entender a estrutura completa

**Objetivo:** Dominar o fluxo completo `init → get → set → unset → to_array`

**Tarefas:**
- [ ] Ler `env_init.c` e desenhar no papel a estrutura `t_env` com prev/next (**20 min**)
- [ ] Traçar o fluxo de `create_env_node` → `add_env_node` → lista final (**15 min**)
- [ ] Ler `env_get.c` e responder: o que retorna `get_env_value` quando a key não existe? (**10 min**)
- [ ] Ler `env_set.c` e identificar os dois caminhos: update vs. create (**15 min**)
- [ ] Ler `env_unset.c` e traçar os 3 casos: nó no início / meio / fim da lista (**20 min**)
- [ ] Ler `env_utils.c`: entender `env_to_array` e por que o `execve` precisa de `char **` (**20 min**)

**Deliverable:** Conseguir explicar o ciclo de vida de uma variável de ambiente do `init` até o `execve` sem olhar o código

---

### Sessão 2 — Tarde (2h): Testar e verificar edge casos

**Tarefas:**
- [ ] Compilar e testar manualmente com `echo $HOME`, `export FOO=bar`, `unset FOO` (**30 min**)
- [ ] Verificar: o que acontece se `value` for `""` (string vazia)? `set_env_value` lida com isso? (**20 min**)
- [ ] Verificar: e se `key` tiver `=` dentro? (edge case do parser de env) (**20 min**)
- [ ] Checar leak em `env_to_array` — o array é liberado após uso no executor? (**20 min**)
- [ ] Verificar: `print_env` usa `printf` — o TODO no comentário menciona trocar por `ft_printf`. Fazer essa troca. (**20 min**)

**Checkpoint ✅ Dia 1:**
```bash
export TEST=hello && echo $TEST   # deve imprimir: hello
unset TEST && echo $TEST          # deve imprimir vazio
env | grep PATH                   # deve mostrar o PATH
```

---

## Dia 2 — `utils/error_utils.c`

### Sessão 1 — Manhã (1h30): Entender o sistema de erros

**Objetivo:** Entender como o `handle_error` replica o comportamento do bash

**Tarefas:**
- [ ] Ler `error_utils.c` inteiro (**15 min**)
- [ ] Mapear cada `t_error` do enum em `minishell.h` com sua mensagem no array `messages[]` (**15 min**)
- [ ] Traçar os 3 formatos de saída do `handle_error` (com cmd+detail / só detail / syntax) (**20 min**)
- [ ] Comparar com o bash real: `bash -c "cd /naoexiste"` — a mensagem bate? (**20 min**)
- [ ] Pensar: quando usar `exit_error` vs `handle_error`? Listar 2 casos de cada (**20 min**)

### Sessão 2 — Tarde (1h): Testar e cobrir casos faltantes

**Tarefas:**
- [ ] Testar cada tipo de erro manualmente (cd sem dir, comando inválido, sintaxe ruim) (**30 min**)
- [ ] Verificar se `ERR_SYNTAX` exibe o token corretamente: `minishell: syntax error near unexpected token '|'` (**15 min**)
- [ ] Confirmar que erros vão para `stderr` (use `2>/dev/null` para suprimir e verificar) (**15 min**)

**Checkpoint ✅ Dia 2:**
```bash
cd /pasta_inexistente        # minishell: cd: /pasta_inexistente: No such file or directory
exit abc                     # minishell: exit: abc: numeric argument required
|                            # minishell: syntax error near unexpected token '|'
```

---

## Dia 3 — `parsing/`: Lexer, Tokens e Parser

### Sessão 1 — Manhã (2h30): Lexer e Tokens

**Objetivo:** Entender como o input vira uma lista de tokens

**Tarefas:**
- [ ] Ler `tokens.c`: `create_token` e `add_token` (**15 min**)
- [ ] Ler `lexer.c` inteiro e traçar: `"echo hello | cat"` → lista de tokens (**30 min**)
- [ ] Entender `extract_word`: o que acontece com `'hello world'` (aspas simples com espaço)? (**20 min**)
- [ ] Entender `identify_token_type`: testar `<<`, `>>`, `<`, `>`, `|` (**20 min**)
- [ ] Ler `quotes.c`: `update_quote_state`, `is_in_quotes`, `remove_quotes`, `validate_quotes` (**30 min**)
- [ ] Desenhar a máquina de estados de aspas: `NONE → SINGLE → NONE`, `NONE → DOUBLE → NONE` (**15 min**)

**Deliverable:** Conseguir simular no papel a tokenização de `echo "hello $NAME" | cat -e`

---

### Sessão 2 — Tarde (2h30): Parser e Validação de Sintaxe

**Tarefas:**
- [ ] Ler `parser.c` inteiro: `validate_syntax`, construção da `t_cmd` list (**40 min**)
- [ ] Traçar: `ls -la | grep src | wc -l` → 3 nós `t_cmd` com args e pipe linkados (**20 min**)
- [ ] Ler `parser_utils.c`: helpers de `add_arg_to_cmd`, `add_redir_to_cmd` (**20 min**)
- [ ] Ler `parser_free.c`: `free_redirs`, `free_cmd_list` — checar se toda memória é liberada (**20 min**)
- [ ] Identificar: o parser lida com redirects antes e depois do comando? (ex: `< in.txt cat > out.txt`) (**20 min**)

**Checkpoint ✅ Dia 3:**
```bash
echo hello | cat               # pipeline básico funciona
< Makefile cat                 # redir antes do cmd funciona
echo "test" > /tmp/out && cat /tmp/out  # redir out funciona
||                             # syntax error near unexpected token '|'
```

---

## Dia 4 — `parsing/`: Expander e Polimento Final

### Sessão 1 — Manhã (2h): Expansão de Variáveis

**Objetivo:** Entender o fluxo de expansão `$VAR` e `$?`

**Tarefas:**
- [ ] Ler `expander_utils.c`: `append_to_buffer`, `append_char_to_buffer`, `extract_var_name` (**30 min**)
- [ ] Ler `expander.c`: `expand_string`, `expand_tokens`, `expand_var_value` (**40 min**)
- [ ] Traçar o fluxo de `"hello $USER world"`: como o buffer é construído char por char (**20 min**)
- [ ] Testar edge cases de expansão:
  - [ ] `"$?"` — expande para `last_exit_status`? (**10 min**)
  - [ ] `'$HOME'` — aspas simples bloqueiam expansão? (**10 min**)
  - [ ] `"$NAOEXISTE"` — variável inexistente vira string vazia? (**10 min**)

### Sessão 2 — Tarde (2h): Integração entre as 3 partes + Norminette

**Tarefas:**
- [ ] Testar o fluxo completo: input → lexer → expander → parser → executor (**40 min**)
- [ ] Verificar que erros de parsing usam corretamente `handle_error` (**20 min**)
- [ ] Verificar que erros de env (HOME not set, etc.) chegam ao output correto (**20 min**)
- [ ] Rodar norminette em todos os arquivos das 3 áreas (**20 min**)
- [ ] Corrigir qualquer violação de norma (**20 min**)

**Checkpoint ✅ Final:**
```bash
echo $USER                      # expande corretamente
echo "$HOME is $PWD"           # múltiplas vars na mesma string
export X=42 && echo "X is $X"  # set + expand
unset X && echo "X is '$X'"    # unset + expansão vazia
echo 'no $EXPAND here'         # single quotes bloqueiam
ls /naoexiste; echo $?          # $? = 1 (last exit status)
```

---

## Recursos 📚

| Tópico | Recurso |
|---|---|
| Linked lists em C | `man 3 malloc` + `man 3 free` |
| Tokenização / Lexer | GNU Bash Reference: Shell Grammar |
| Expansão de variáveis | `man bash` → seção "EXPANSION" |
| `execve` e `char **envp` | `man 3 execve` |
| Saída de erros correta | `man 2 write` + `STDERR_FILENO` |

---

## Sessão Pomodoro (25 min) — Template Rápido

```
[ ] Definir 1 objetivo claro antes de começar
[ ] Timer: 25 min trabalho
[ ] 5 min pausa (levanta, bebe água)
[ ] Registrar o que completou
[ ] Próximo micro-objetivo
```

---

> **Nota:** A maior parte do código já está bem estruturado e documentado. O foco dessas sessões é **compreender profundamente** o que já existe, **validar edge cases**, garantir que a integração entre as 3 áreas está correta e **zero leaks** antes da avaliação.
