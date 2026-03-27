# 📖 Minishell — Plano de Estudo para Avaliação

**Desenhado para quem tem dificuldade de foco e precisa de objetivos claros.**

Usa 3 arquivos de apoio:
- **CONCEPTS.md** → explicações detalhadas de cada conceito
- **CHEAT_SHEET.md** → referência rápida para levar na avaliação
- **PROJECT_FLOW.md** → fluxo completo do código

---

## 🎯 Método: Pomodoro Modificado

### Ciclo (25 minutos)
```
15-20 min → ESTUDO ATIVO (ler + anotar)
5-10 min  → PRÁTICA (testar no terminal / explicar em voz alta)
5 min     → PAUSA (levanta, água, alonga)
```

**Dicas:**
- ⏰ Timer visível no celular ou computador
- ✅ Marca cada sessão completada
- 🎵 Música sem letra se ajudar (lofi, ambient)
- 📱 Celular longe ou em modo avião

---

## 📅 Plano de 4 Dias

---

## 🗓️ DIA 1: Visão Geral + Estruturas + REPL

### ✅ Sessão 1: O problema e o fluxo (25 min)
**OBJETIVO:** Saber explicar o que o minishell faz e como funciona o loop principal

**📖 Fazer:**
1. Ler CONCEPTS.md — seção "O que é um shell?" (5 min)
2. Ler PROJECT_FLOW.md — seção "Visão geral" e "Inicialização" (10 min)
3. Abrir `src/main.c` e seguir o código de cima até baixo (5 min)
4. Explicar em voz alta: "O que acontece desde que o programa inicia até o primeiro prompt?" (5 min)

**✍️ Anotar no caderno:**
- [ ] REPL = Read → Eval → Print → Loop
- [ ] init → setup_signals → repl_loop → cleanup

**🎮 Mini-Desafio:** Fechar o arquivo e desenhar no papel o fluxo do main

---

### ✅ Sessão 2: Estruturas de dados (25 min)
**OBJETIVO:** Saber o que tem em cada struct e por que existe

**📖 Fazer:**
1. Ler CHEAT_SHEET.md — seção "Estruturas principais" (5 min)
2. Abrir `include/minishell.h` e comparar com o cheat sheet (8 min)
3. Ler CONCEPTS.md — seção "Ambiente" (por que t_env é doubly linked) (5 min)
4. Desenhar no papel: t_mini contendo t_env list e t_cmd list (7 min)

**✍️ Anotar no caderno:**
- [ ] t_mini: env, cmd_list, last_exit_status, running
- [ ] t_env: key, value, prev, next
- [ ] t_cmd: args, redirs, pid, next
- [ ] Por que doubly linked: remoção eficiente sem percorrer tudo

**🎮 Mini-Desafio:** Explica por que t_env tem `prev` e `next` em vez de só `next`

---

### ⏸️ PAUSA LONGA: 15-30 minutos

---

### ✅ Sessão 3: REPL e sinais (25 min)
**OBJETIVO:** Entender o loop e o comportamento de ctrl-C, ctrl-D, ctrl-\

**📖 Fazer:**
1. Ler PROJECT_FLOW.md — seção "Loop principal" (5 min)
2. Ler CONCEPTS.md — seção "Sinais — três modos" (10 min)
3. Abrir `src/signals/signals.c` e ler as 4 funções (5 min)
4. Testar no terminal: `./minishell` → ctrl-C, ctrl-\, ctrl-D (5 min)

**✍️ Anotar no caderno:**
- [ ] ctrl-D = readline retorna NULL = sai
- [ ] ctrl-C no prompt = g_signal=130, novo prompt
- [ ] ctrl-\ no prompt = nada (SIG_IGN)
- [ ] Três modos: setup_signals / setup_exec_signals / setup_child_signals

**🎮 Mini-Desafio:** Explica o que acontece quando ctrl-C é apertado com `cat` rodando

---

## 🗓️ DIA 2: Parsing — Lexer, Expander, Parser

### ✅ Sessão 4: Tokenização (25 min)
**OBJETIVO:** Entender como a string vira tokens

**📖 Fazer:**
1. Ler CONCEPTS.md — seção "Tokenização" (8 min)
2. Ver CHEAT_SHEET.md — tabela de tipos de token (3 min)
3. Abrir `src/parsing/lexer.c` e traçar `echo "hello" | cat` (10 min)
4. Explicar em voz alta o que acontece com `'hello world'` (tem espaço dentro de aspas) (4 min)

**✍️ Anotar no caderno:**
- [ ] Separadores: espaço, tab, |, <, >
- [ ] Dentro de aspas: separadores são parte da palavra
- [ ] Estados: QUOTE_NONE, QUOTE_SINGLE, QUOTE_DOUBLE

**🎮 Mini-Desafio:** No papel, tokeniza `ls -la | grep .c > /tmp/out.txt`

---

### ✅ Sessão 5: Expansão de variáveis (25 min)
**OBJETIVO:** Entender o expander e as regras de aspas

**📖 Fazer:**
1. Ler CONCEPTS.md — seção "Expansão de variáveis" (8 min)
2. Ver CHEAT_SHEET.md — tabela "Aspas e expansão" (3 min)
3. Abrir `src/parsing/expander.c` e ver `process_char` e `expand_string` (7 min)
4. Testar no minishell: `echo $USER`, `echo '$USER'`, `echo "$USER"`, `echo $?` (7 min)

**✍️ Anotar no caderno:**
- [ ] $VAR → get_env_value() → valor
- [ ] $? → ft_itoa(last_exit_status)
- [ ] '' bloqueia tudo; "" permite $
- [ ] Expander roda ANTES de remover aspas (as aspas controlam o que expande)

**🎮 Mini-Desafio:** O que sai de `echo "$HOME is '$USER'"`?

---

### ⏸️ PAUSA LONGA: 15-30 minutos

---

### ✅ Sessão 6: Parser e sintaxe (25 min)
**OBJETIVO:** Entender como tokens viram comandos

**📖 Fazer:**
1. Ler PROJECT_FLOW.md — seção "Parser" (8 min)
2. Abrir `src/parsing/parser.c` e focar em `validate_syntax` e `parse_tokens` (10 min)
3. Traçar no papel: `ls -la | grep src | wc -l` → 3 nós t_cmd ligados (7 min)

**✍️ Anotar no caderno:**
- [ ] validate_syntax: pipe no início/fim, redirecionamento sem arquivo
- [ ] TOKEN_WORD → add_arg_to_cmd
- [ ] TKN_REDIR_* → create_redir_node → add_redir_to_cmd
- [ ] TOKEN_PIPE → current_cmd = NULL (próximo word cria novo cmd)

**🎮 Mini-Desafio:** O que o parser produz para `< in.txt cat > out.txt`?

---

## 🗓️ DIA 3: Execução — Processos, Pipes, Redirecionamentos

### ✅ Sessão 7: fork, execve, wait (25 min)
**OBJETIVO:** Entender o fluxo de execução de um comando externo

**📖 Fazer:**
1. Ler CONCEPTS.md — seção "Processos — fork e execve" (10 min)
2. Abrir `src/execution/executor_simple.c` e seguir `execute_simple_cmd` e `wait_child` (10 min)
3. Explicar em voz alta: "Por que fork antes do execve?" (5 min)

**✍️ Anotar no caderno:**
- [ ] fork() = duplica processo
- [ ] execve() = substitui processo (nunca retorna se OK)
- [ ] filho: setup_child_signals → redirections → find_path → execve
- [ ] pai: setup_exec_signals → waitpid → setup_signals
- [ ] WIFEXITED / WIFSIGNALED → exit status

**🎮 Mini-Desafio:** O que acontece se execve falha? Qual exit code?

---

### ✅ Sessão 8: Builtins (25 min)
**OBJETIVO:** Entender por que builtins rodam no pai e saber cada um

**📖 Fazer:**
1. Ler CONCEPTS.md — seção "Builtins" (5 min)
2. Ver CHEAT_SHEET.md — tabela de builtins (5 min)
3. Abrir `src/builtins/builtin_cd.c` e `builtin_exit.c` (os mais interessantes) (8 min)
4. Testar: `cd`, `cd /tmp`, `pwd`, `export X=1`, `echo $X`, `unset X`, `exit 42` (7 min)

**✍️ Anotar no caderno:**
- [ ] Por que no pai: cd/export/exit mudam estado do shell
- [ ] exit não chama exit() → mini->running = FALSE
- [ ] cd sem arg → $HOME via get_env_value
- [ ] export sem arg → imprime declare -x

**🎮 Mini-Desafio:** Por que `export X=1 | cat` não define X no shell atual?

---

### ⏸️ PAUSA LONGA: 15-30 minutos

---

### ✅ Sessão 9: Pipes e redirecionamentos (25 min)
**OBJETIVO:** Entender como pipe conecta processos e como redir muda stdin/stdout

**📖 Fazer:**
1. Ler CONCEPTS.md — seções "Pipes" e "Redirecionamentos" (10 min)
2. Abrir `src/execution/executor_pipeline.c` e ver `child_process` e `wait_all` (8 min)
3. Ver `src/execution/redirections.c` e `redirections_utils.c` (7 min)

**✍️ Anotar no caderno:**
- [ ] pipe(): pipefd[0]=leitura, pipefd[1]=escrita
- [ ] dup2(pipefd[1], STDOUT_FILENO) → stdout vai pro pipe
- [ ] pai fecha os dois lados após fork → filho recebe EOF
- [ ] setup_redirections salva orig_stdin (por causa de heredocs múltiplos)

**🎮 Mini-Desafio:** Explica o que acontece com os FDs em `ls | grep src | wc -l`

---

## 🗓️ DIA 4: Revisão + Prática + Simulação

### ✅ Sessão 10: Ambiente e erros (25 min)
**OBJETIVO:** Consolidar env e mensagens de erro

**📖 Fazer:**
1. Abrir `src/env/env_init.c`, `env_set.c`, `env_unset.c` e ler (10 min)
2. Abrir `src/utils/error_utils.c` e ver o formato das mensagens (5 min)
3. Testar: `export A=1`, `env | grep A`, `unset A`, `env | grep A` (5 min)
4. Testar erros: `cd /x`, `notexist`, `exit abc`, `|` (5 min)

**✍️ Anotar no caderno:**
- [ ] init_env: envp (char**) → t_env list
- [ ] env_to_array: t_env → char** (para execve)
- [ ] Formato: `minishell: [cmd]: [detalhe]: mensagem`
- [ ] Erros vão para stderr (fd 2)

---

### ✅ Sessão 11: Casos de teste (25 min)
**OBJETIVO:** Rodar todos os casos do CHEAT_SHEET e entender cada saída

**📖 Fazer:**
1. Compilar: `make re`
2. Rodar cada bloco de testes do CHEAT_SHEET.md (20 min)
3. Para qualquer resultado inesperado: achar no código onde está (5 min)

**✍️ Gravar observações:** O que funcionou? O que surpreendeu?

---

### ✅ Sessão 12: Simulação de avaliação (25 min)
**OBJETIVO:** Praticar explicar como se estivesse na defesa

**📖 Fazer:**
1. Timer 4 min: explica o problema — o que o minishell faz
2. Timer 4 min: explica o fluxo do main ao executor
3. Timer 4 min: explica parsing (lexer → expander → parser)
4. Timer 4 min: explica sinais (por que 3 funções?)
5. Timer 4 min: explica fork + execve + por que builtins no pai
6. Timer 5 min: responde as perguntas do CHEAT_SHEET sem olhar

**Dica:** Gravar em áudio ou explicar para um bichinho de pelúcia funciona.

---

## 🎯 Checklist de Domínio

Marca quando conseguires explicar sem olhar:

### Conceitos
- [ ] O que é o REPL?
- [ ] Por que fork antes do execve?
- [ ] O que é um pipe e como conecta processos?
- [ ] Diferença entre `>` e `>>`
- [ ] O que faz o heredoc `<<`?
- [ ] Por que `$USER` expande mas `'$USER'` não?
- [ ] Por que remover aspas depois de expandir?
- [ ] O que é $? e como é calculado?

### Sinais
- [ ] Por que três funções de setup de sinais?
- [ ] O que cada uma configura?
- [ ] Por que o filho reseta para SIG_DFL?
- [ ] O que SA_RESTART faz?
- [ ] Por que g_signal = 130 e não 2?

### Execução
- [ ] Como o executor decide o caminho (builtin/simples/pipeline)?
- [ ] Por que builtins rodam no pai?
- [ ] O que exec_builtin_parent faz com os fds?
- [ ] Como o pipeline conecta stdout → stdin entre processos?
- [ ] O que acontece com os FDs que o pai não usa?

### Builtins
- [ ] Por que `exit` não chama `exit()` diretamente?
- [ ] O que `cd` sem argumento faz?
- [ ] O que `export` sem argumento faz?
- [ ] O que `unset` faz com variável inexistente?

### Código
- [ ] Onde está cada funcionalidade no código?
- [ ] Consigo abrir qualquer arquivo e explicar o que cada função faz
- [ ] Consigo traçar o fluxo de `echo $USER | cat` do início ao fim

---

## 💡 Dicas para foco

### ✅ FAÇA:
- **Blocos curtos** (15-25 min máximo sem pausa)
- **Explique em voz alta** — ativa mais áreas do cérebro
- **Desenhe** as estruturas no papel
- **Teste no terminal** sempre que possível
- **Mude de ambiente** entre sessões (quarto → sala → cozinha)
- **Recompensas** a cada 3 sessões completadas

### ❌ EVITE:
- Sessões longas sem pausa
- Ler passivamente sem anotar ou testar
- "Preciso entender tudo hoje" (pressão piora o foco)
- Estudar com fome ou cansaço

### 🆘 Se perder o foco:
1. Para. Não se culpe.
2. Pausa de 5 min: levanta, bebe água
3. Volta com objetivo ainda menor: 10 min em vez de 25
4. Troca o formato: em vez de ler, testa no terminal

---

## 📊 Registro de Progresso

```
DIA 1:
Sessão 1: [ ] Horário: _____ Foco: ☆☆☆☆☆
Sessão 2: [ ] Horário: _____ Foco: ☆☆☆☆☆
Sessão 3: [ ] Horário: _____ Foco: ☆☆☆☆☆

DIA 2:
Sessão 4: [ ] Horário: _____ Foco: ☆☆☆☆☆
Sessão 5: [ ] Horário: _____ Foco: ☆☆☆☆☆
Sessão 6: [ ] Horário: _____ Foco: ☆☆☆☆☆

DIA 3:
Sessão 7: [ ] Horário: _____ Foco: ☆☆☆☆☆
Sessão 8: [ ] Horário: _____ Foco: ☆☆☆☆☆
Sessão 9: [ ] Horário: _____ Foco: ☆☆☆☆☆

DIA 4:
Sessão 10: [ ] Horário: ____ Foco: ☆☆☆☆☆
Sessão 11: [ ] Horário: ____ Foco: ☆☆☆☆☆
Sessão 12: [ ] Horário: ____ Foco: ☆☆☆☆☆

TOTAL: ___ / 12 sessões
```

---

## 🎉 Recompensas por Milestone

- **3 sessões:** Lanche favorito / 30 min de algo que gostas
- **6 sessões:** Episódio de série / jogo que gostas
- **9 sessões:** Sabes 80% do conteúdo — quase lá!
- **12 sessões:** PRONTO PARA A AVALIAÇÃO 🎊

---

**Progresso > Perfeição. Cada sessão completada é uma vitória. Boa sorte! 💪**
