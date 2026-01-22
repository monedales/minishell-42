# 🐚 Minishell - Projeto 42

> **Implementação de um shell Unix minimalista em C**

## 📌 Sobre o Projeto

O minishell é um dos projetos mais desafiadores da 42 School. Você vai criar seu próprio interpretador de comandos (como bash), implementando parsing de entrada, execução de processos, pipes, redirecionamentos e built-ins.

## 🏗️ Estrutura do Projeto

```
minishell/
├── 📄 Makefile                   # Sistema de build
├── 📄 README.md                  # Este arquivo
├── 📄 PROJECT_STRUCTURE.md       # Documentação detalhada da estrutura
├── 📄 PESSOA_A_GUIDE.md          # Guia específico para Pessoa A
│
├── 📂 include/
│   └── minishell.h               # Header principal com todas structs e protótipos
│
├── 📂 src/
│   ├── main.c                    # Entry point + REPL loop
│   │
│   ├── 📂 parsing/               # 🔵 PESSOA A
│   │   ├── lexer.c               # Tokenização
│   │   ├── expander.c            # Expansão de variáveis ($VAR, $?)
│   │   ├── parser.c              # Construção da árvore de comandos
│   │   └── quotes.c              # Manipulação de aspas
│   │
│   ├── 📂 env/                   # 🔵 PESSOA A
│   │   ├── env_init.c            # Inicialização do ambiente
│   │   ├── env_get.c             # Buscar variável
│   │   ├── env_set.c             # Adicionar/modificar variável
│   │   ├── env_unset.c           # Remover variável
│   │   └── env_utils.c           # Conversão para array, print, etc
│   │
│   ├── 📂 execution/             # 🟢 PESSOA B
│   │   ├── executor.c            # Dispatcher principal
│   │   ├── executor_simple.c     # Comando simples
│   │   ├── executor_pipeline.c   # Pipeline (pipes)
│   │   ├── path_finder.c         # Busca no PATH
│   │   └── redirections.c        # Setup de redirecionamentos
│   │
│   ├── 📂 builtins/              # 🟢 PESSOA B
│   │   ├── builtin_checker.c     # Verifica se é builtin
│   │   ├── builtin_echo.c
│   │   ├── builtin_cd.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_export.c
│   │   ├── builtin_unset.c
│   │   ├── builtin_env.c
│   │   └── builtin_exit.c
│   │
│   ├── 📂 signals/               # 🟢 PESSOA B
│   │   └── signals.c             # Ctrl-C, Ctrl-\, Ctrl-D
│   │
│   └── 📂 utils/                 # 🟡 AMBOS
│       ├── ft_strlen.c
│       ├── ft_strdup.c
│       ├── ft_strjoin.c
│       ├── ft_strcmp.c
│       ├── ft_split.c
│       ├── ft_calloc.c
│       ├── error_utils.c
│       └── free_utils.c
│
└── 📂 obj/                       # Arquivos objeto (gitignored)
```

## 🚀 Quick Start

```bash
# Clonar o repositório
git clone <repo-url>
cd minishell

# IMPORTANTE: Adicionar sua libft ao projeto
# Veja LIBFT_SETUP.md para instruções detalhadas
cp -r /caminho/para/sua/libft ./libft
# OU
ln -s /caminho/para/sua/libft ./libft

# Compilar
make

# Executar
./minishell

# Testar comandos básicos
minishell$ echo "Hello World"
minishell$ ls -la | grep mini
minishell$ export VAR=42
minishell$ echo $VAR

# Sair
minishell$ exit
```

## 🎯 Features Implementadas

### ✅ Comandos e Execução
- [x] Prompt interativo com readline
- [x] Histórico de comandos
- [x] Busca e execução de binários via PATH
- [x] Caminhos relativos e absolutos

### ✅ Redirecionamentos
- [x] `<` - Redirecionamento de entrada
- [x] `>` - Redirecionamento de saída
- [x] `>>` - Append
- [x] `<<` - Heredoc

### ✅ Pipes
- [x] Suporte a múltiplos pipes: `cmd1 | cmd2 | cmd3`

### ✅ Variáveis
- [x] Expansão de variáveis: `$VAR`
- [x] Exit status: `$?`
- [x] Aspas simples (sem expansão)
- [x] Aspas duplas (com expansão)

### ✅ Built-ins
- [x] `echo` (com opção `-n`)
- [x] `cd` (caminho relativo/absoluto/home)
- [x] `pwd`
- [x] `export`
- [x] `unset`
- [x] `env`
- [x] `exit`

### ✅ Sinais
- [x] Ctrl-C: Nova linha
- [x] Ctrl-D: Sair (EOF)
- [x] Ctrl-\: Nada

## 🧪 Testes

```bash
# Verificar norminette
make norm

# Testar com valgrind
make valgrind

# Executar testes manuais
./minishell
```

### Casos de Teste Importantes

```bash
# 1. Comandos simples
ls -la
pwd
echo test

# 2. Pipes
ls | grep mini
cat file.txt | head -5 | tail -2

# 3. Redirecionamentos
echo "test" > file.txt
cat < file.txt
echo "append" >> file.txt

# 4. Variáveis
echo $HOME
export TEST=value
echo $TEST
unset TEST

# 5. Aspas
echo 'single $USER quotes'
echo "double $USER quotes"

# 6. Exit status
ls nonexistent
echo $?

# 7. Heredoc
cat << EOF
line 1
line 2
EOF
```

## 📚 Documentação

- [LIBFT_SETUP.md](LIBFT_SETUP.md) - ⚙️ Como adicionar a libft ao projeto
- [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) - Estrutura detalhada e divisão de tarefas
- [PESSOA_A_GUIDE.md](PESSOA_A_GUIDE.md) - 🔵 Guia completo para Pessoa A (Parsing & Ambiente)
- [PESSOA_B_GUIDE.md](PESSOA_B_GUIDE.md) - 🟢 Guia completo para Pessoa B (Execução & Sistema)
- [include/minishell.h](include/minishell.h) - Todas as structs e protótipos

## 🤝 Divisão de Trabalho

### 🔵 Pessoa A - Parsing & Organização
- Gerenciamento de ambiente (env)
- Lexer (tokenização)
- Expander (expansão de variáveis)
- Parser (construção de comandos)
- Manipulação de aspas

### 🟢 Pessoa B - Execução & Sistema
- Executor (fork, execve, wait)
- Pipes
- Redirecionamentos
- Built-ins
- Sinais

## ⚠️ Notas Importantes

### Memory Leaks
```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

### File Descriptors
Sempre feche FDs abertos com `close()`. Use `lsof -p <pid>` para debugar.

### Norminette
```bash
norminette include/ src/
```

## 📖 Recursos Úteis

- `man bash`
- `man readline`
- `man execve`
- `man fork`
- `man pipe`
- `man dup2`
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)

## 🐛 Debugging Tips

```bash
# Verificar processos
ps aux | grep minishell

# Verificar FDs abertos
lsof -p $(pgrep minishell)

# Debugar com gdb
gdb ./minishell

# Verificar memory leaks
valgrind --leak-check=full ./minishell
```

## 👥 Autores

- **Pessoa A**: [Seu nome] - Parsing & Organização
- **Pessoa B**: [Nome do parceiro] - Execução & Sistema

## 📄 Licença

Este projeto faz parte do currículo da 42 School.

---

**Boa sorte e bom código!** 🚀