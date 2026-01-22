# 🐚 Minishell - 42 Project

## 📋 Descrição

Implementação de um shell minimalista (como bash) que suporta execução de comandos, pipes, redirecionamentos, variáveis de ambiente e built-ins.

## 🏗️ Estrutura do Projeto

```
minishell/
├── Makefile                  # Sistema de build
├── README.md                 # Documentação
├── include/
│   └── minishell.h          # Header principal com structs e protótipos
│
├── src/
│   ├── main.c               # Entry point + REPL loop
│   │
│   ├── parsing/             # 🔵 PESSOA A - Interpretação
│   │   ├── lexer.c          # Tokenização
│   │   ├── expander.c       # Expansão de variáveis ($VAR, $?)
│   │   ├── parser.c         # Construção da árvore de comandos
│   │   └── quotes.c         # Manipulação de aspas
│   │
│   ├── env/                 # 🔵 PESSOA A - Ambiente
│   │   ├── env_init.c       # Inicialização do ambiente
│   │   ├── env_get.c        # Buscar variável
│   │   ├── env_set.c        # Adicionar/modificar variável
│   │   ├── env_unset.c      # Remover variável
│   │   └── env_utils.c      # Conversão para array, free, etc
│   │
│   ├── execution/           # 🟢 PESSOA B - Execução
│   │   ├── executor.c       # Dispatcher principal
│   │   ├── executor_simple.c # Comando simples
│   │   ├── executor_pipeline.c # Pipeline (pipes)
│   │   ├── path_finder.c    # Busca no PATH
│   │   └── redirections.c   # Setup de redirecionamentos
│   │
│   ├── builtins/            # 🟢 PESSOA B - Built-ins
│   │   ├── builtin_checker.c # Verifica se é builtin
│   │   ├── builtin_echo.c
│   │   ├── builtin_cd.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_export.c
│   │   ├── builtin_unset.c
│   │   ├── builtin_env.c
│   │   └── builtin_exit.c
│   │
│   ├── signals/             # 🟢 PESSOA B - Sinais
│   │   └── signals.c        # Ctrl-C, Ctrl-\, Ctrl-D
│   │
│   └── utils/               # 🟡 AMBOS - Utilitários
│       ├── ft_strlen.c
│       ├── ft_strdup.c
│       ├── ft_strjoin.c
│       ├── ft_strcmp.c
│       ├── ft_split.c
│       ├── ft_calloc.c
│       ├── error_utils.c
│       └── free_utils.c
│
└── obj/                     # Arquivos objeto (gitignored)
```

## 🎯 Divisão de Responsabilidades

### 🔵 PESSOA A - Parsing & Organização
- **Ambiente**: Converter `envp` em lista ligada, buscar/adicionar/remover variáveis
- **Lexer**: Tokenizar a string respeitando aspas
- **Expander**: Substituir `$VAR` e `$?` pelos valores reais
- **Parser**: Construir a lista de comandos (`t_cmd`) a partir dos tokens
- **Quotes**: Remover aspas e identificar quando um caractere está quotado

### 🟢 PESSOA B - Execução & Sistema
- **Executor**: Fork, execve e busca no PATH
- **Pipes**: Criar pipes e conectar stdout → stdin entre processos
- **Redirecionamentos**: `<`, `>`, `>>`, `<<` com dup2
- **Built-ins**: Implementar echo, cd, pwd, export, unset, env, exit
- **Sinais**: Configurar handlers para Ctrl-C, Ctrl-\ e Ctrl-D

## 📦 Estruturas de Dados

### Principais structs:

```c
t_mini    // Estrutura principal (env, cmd_list, last_exit_status)
t_env     // Lista ligada de variáveis (key, value, next)
t_token   // Token do lexer (type, value, next)
t_cmd     // Nó de comando (args, redirs, next)
t_redir   // Redirecionamento (type, file, next)
```


## ✅ Checklist de Features

### Obrigatórias
- [ ] Prompt interativo (readline)
- [ ] Histórico de comandos
- [ ] Buscar e executar binários (PATH)
- [ ] Caminhos relativos e absolutos
- [ ] Pipes (`|`)
- [ ] Redirecionamentos (`<`, `>`, `>>`)
- [ ] Heredoc (`<<`)
- [ ] Expansão de variáveis (`$VAR`)
- [ ] Status de saída (`$?`)
- [ ] Aspas simples (sem expansão)
- [ ] Aspas duplas (com expansão)
- [ ] Sinais (Ctrl-C, Ctrl-D, Ctrl-\)

### Built-ins
- [ ] `echo` (com opção `-n`)
- [ ] `cd` (com caminho relativo/absoluto)
- [ ] `pwd`
- [ ] `export`
- [ ] `unset`
- [ ] `env`
- [ ] `exit`

## 🧪 Testes Sugeridos

```bash
# Comandos simples
ls -la
echo "Hello World"
pwd

# Pipes
ls -la | grep minishell | wc -l
cat file.txt | head -5 | tail -2

# Redirecionamentos
echo "test" > file.txt
cat < file.txt
echo "append" >> file.txt

# Variáveis
echo $USER
export VAR=42
echo $VAR
unset VAR

# Aspas
echo 'single $USER quotes'
echo "double $USER quotes"

# Exit status
ls nonexistent
echo $?
```

## 📚 Recursos Úteis

- `man readline`, `man execve`, `man fork`, `man pipe`, `man dup2`
- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)

## ⚠️ Cuidados Importantes

- **Memory leaks**: Use `valgrind` constantemente
- **File descriptors**: Sempre feche os FDs que abrir
- **Sinais**: Configure corretamente para o comportamento esperado
- **Norminette**: Verifique antes de cada commit
- **Edge cases**: Teste inputs vazios, muito longos, com caracteres especiais

## 🤝 Autores

- **Pessoa A**: [Nome] - Parsing & Organização
- **Pessoa B**: [Nome] - Execução & Sistema

---

**Boa sorte!** 🚀
