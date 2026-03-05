# 🎯 Guia de Implementação - PESSOA B (Execução & Sistema)

## 📊 Ordem de Implementação Recomendada

### FASE 1: Built-ins Simples (Começar por aqui!)
Começar pelos built-ins é estratégico: você pode testar sem precisar do executor completo.

#### 1.1 Built-ins Independentes (PRIORIDADE MÁXIMA)
```c
✅ src/builtins/builtin_pwd.c
   - builtin_pwd()
   - Usar getcwd()
   - Retornar SUCCESS/ERROR

✅ src/builtins/builtin_env.c
   - builtin_env()
   - Usar print_env() da Pessoa A
   - Apenas listar variáveis

✅ src/builtins/builtin_echo.c
   - builtin_echo()
   - Verificar flag -n
   - Imprimir argumentos separados por espaço
```

**TESTE TEMPORÁRIO:**
```c
// No main, criar um teste direto:
char *args[] = {"echo", "hello", "world", NULL};
builtin_echo(args);  // hello world

char *args2[] = {"echo", "-n", "test", NULL};
builtin_echo(args2);  // test (sem \n)
```

#### 1.2 Built-ins com Ambiente
```c
✅ src/builtins/builtin_export.c
   - builtin_export()
   - Parsear "VAR=valor"
   - Chamar set_env_value() da Pessoa A
   - Se sem args, listar variáveis ordenadas

✅ src/builtins/builtin_unset.c
   - builtin_unset()
   - Chamar unset_env_value() da Pessoa A
   - Suportar múltiplos argumentos

✅ src/builtins/builtin_cd.c
   - builtin_cd()
   - Usar chdir()
   - Atualizar PWD e OLDPWD no env
   - cd sem args vai para $HOME
```

**TESTE:**
```c
builtin_export((char*[]){"export", "VAR=42", NULL}, &mini);
printf("%s\n", get_env_value(mini.env, "VAR"));  // 42

builtin_cd((char*[]){"cd", "/tmp", NULL}, &mini);
builtin_pwd();  // /tmp
```

#### 1.3 Built-in Exit
```c
✅ src/builtins/builtin_exit.c
   - builtin_exit()
   - Parsear argumento numérico (se houver)
   - Validar: apenas números
   - Setar mini->running = FALSE
   - Retornar exit code

✅ src/builtins/builtin_checker.c
   - is_builtin() - verifica se comando é builtin
   - execute_builtin() - dispatcher
```

---

### FASE 2: Executor Simples (Sem Pipes)

#### 2.1 Path Finder
```c
✅ src/execution/path_finder.c
   - find_command_path()
   - Se cmd tem '/', testar com access()
   - Senão, buscar PATH no env
   - Split PATH por ':'
   - Para cada dir, testar dir/cmd
   - Retornar primeiro válido
```

**TESTE:**
```c
char *path = find_command_path("ls", mini.env);
printf("%s\n", path);  // /bin/ls
```

#### 2.2 Executor Básico (Sem Redirecionamentos)
```c
✅ src/execution/executor_simple.c
   - execute_simple_cmd() SIMPLIFICADO:
     * Verificar se é builtin (executar no pai)
     * Buscar caminho com find_command_path()
     * fork()
     * No filho: execve()
     * No pai: waitpid() e capturar exit status
```

**TESTE (precisa da Pessoa A ter feito o parser):**
```c
// Parser fake temporário:
t_cmd cmd;
cmd.args = (char*[]){"ls", "-la", NULL};
cmd.redirs = NULL;
cmd.next = NULL;

int status = execute_simple_cmd(&cmd, &mini);
printf("Exit status: %d\n", status);
```

#### 2.3 Dispatcher Principal
```c
✅ src/execution/executor.c
   - execute_cmd_list()
   - Se único comando e builtin: executar no pai
   - Se único comando: execute_simple_cmd()
   - Se múltiplos comandos: execute_pipeline()
```

---

### FASE 3: Redirecionamentos

#### 3.1 Redirecionamentos Básicos (<, >)
```c
✅ src/execution/redirections.c
   - setup_redirections()
   - REDIR_IN (<):
     * fd = open(file, O_RDONLY)
     * dup2(fd, STDIN_FILENO)
     * close(fd)
   - REDIR_OUT (>):
     * fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)
     * dup2(fd, STDOUT_FILENO)
     * close(fd)
```

**TESTE:**
```bash
echo "test content" > file.txt
cat < file.txt  # deve mostrar "test content"
```

#### 3.2 Append e Heredoc
```c
✅ Expandir setup_redirections()
   - REDIR_APPEND (>>):
     * open com O_APPEND em vez de O_TRUNC
   - REDIR_HEREDOC (<<):
     * Ler linhas até encontrar delimiter
     * Criar pipe temporário
     * Escrever conteúdo no pipe
     * dup2 da leitura do pipe para stdin
```

**TESTE:**
```bash
echo "line 1" > file.txt
echo "line 2" >> file.txt
cat file.txt  # deve ter 2 linhas

cat << EOF
hello
world
EOF
```

---

### FASE 4: Pipes (A Parte Mais Difícil!)

#### 4.1 Entender a Lógica de Pipes

**Conceito:**
```
cmd1 | cmd2 | cmd3

pipe1[2]     pipe2[2]
  ↓            ↓
cmd1 → | → cmd2 → | → cmd3
     write  read   write  read
```

**Algoritmo:**
1. Para cada comando (exceto o último):
   - Criar pipe: `pipe(fd)`
   - Fork
   - No filho:
     - Se não é o primeiro: dup2(prev_read, STDIN)
     - Se não é o último: dup2(fd[1], STDOUT)
     - Fechar todos FDs
     - Execve
   - No pai:
     - Fechar fd[1] (escrita)
     - Salvar fd[0] como prev_read
2. Esperar todos os processos

#### 4.2 Pipeline Simples (2 comandos)
```c
✅ src/execution/executor_pipeline.c
   - execute_pipeline() para 2 comandos
   - Criar 1 pipe
   - Fork para cmd1: stdout -> pipe
   - Fork para cmd2: stdin <- pipe
   - Fechar todos FDs no pai
   - Waitpid para ambos
```

**TESTE:**
```bash
echo "test" | cat
ls | wc -l
```

#### 4.3 Pipeline Múltipla (N comandos)
```c
✅ Expandir execute_pipeline()
   - Loop pelos comandos
   - Gerenciar prev_read e next_write
   - CRITICAL: Fechar FDs corretamente!
```

**TESTE:**
```bash
ls | grep mini | wc -l
cat file.txt | head -5 | tail -2
```

---

### FASE 5: Sinais

#### 5.1 Setup Básico
```c
✅ src/signals/signals.c
   - setup_signals()
   - handle_sigint() - Ctrl-C
   - handle_sigquit() - Ctrl-\
```

**Comportamento esperado:**
- **Ctrl-C (SIGINT)**: 
  - Durante prompt: nova linha
  - Durante execução: terminar processo filho
- **Ctrl-D (EOF)**:
  - Já tratado pelo readline (retorna NULL)
- **Ctrl-\ (SIGQUIT)**:
  - Durante prompt: ignorar
  - Durante execução: terminar + core dump

#### 5.2 Sinais Durante Execução
```c
✅ Configurar sinais diferentes no filho
   - Restaurar handlers default antes de execve
```

---

## 🧪 Estratégia de Teste Progressivo

### Teste 1: Built-ins Básicos
```bash
./minishell
minishell$ pwd
minishell$ env
minishell$ echo hello world
minishell$ echo -n test
```

### Teste 2: Built-ins com Ambiente
```bash
minishell$ export VAR=42
minishell$ export TEST="hello world"
minishell$ env | grep VAR
minishell$ unset VAR
```

### Teste 3: CD e Paths
```bash
minishell$ pwd
minishell$ cd /tmp
minishell$ pwd
minishell$ cd
minishell$ pwd  # deve estar em $HOME
```

### Teste 4: Comandos Externos
```bash
minishell$ /bin/ls
minishell$ ls
minishell$ ls -la
```

### Teste 5: Redirecionamentos
```bash
minishell$ echo "test" > file.txt
minishell$ cat file.txt
minishell$ echo "more" >> file.txt
minishell$ cat < file.txt
```

### Teste 6: Heredoc
```bash
minishell$ cat << EOF
> line 1
> line 2
> EOF
```

### Teste 7: Pipes Simples
```bash
minishell$ echo "test" | cat
minishell$ ls | grep mini
minishell$ cat file.txt | wc -l
```

### Teste 8: Pipes Múltiplos
```bash
minishell$ ls | grep .c | wc -l
minishell$ cat file.txt | head -10 | tail -5
```

### Teste 9: Sinais
```bash
minishell$ cat  # pressione Ctrl-C (deve cancelar)
minishell$      # nova linha
minishell$ sleep 10  # pressione Ctrl-C (deve cancelar)
```

### Teste 10: Exit
```bash
minishell$ exit
minishell$ exit 42
minishell$ exit abc  # deve dar erro
```

---

## ⚠️ Armadilhas Comuns

### 1. File Descriptors Não Fechados
```c
// ❌ ERRADO - vaza FDs!
int fd = open("file.txt", O_RDONLY);
dup2(fd, STDIN_FILENO);
// faltou: close(fd);

// ✅ CORRETO
int fd = open("file.txt", O_RDONLY);
if (fd < 0)
    return (ERROR);
dup2(fd, STDIN_FILENO);
close(fd);  // SEMPRE fechar após dup2!
```

### 2. Fork Bombs (Pipes)
```c
// ❌ ERRADO - fork infinito!
while (cmd)
{
    fork();  // vai criar 2^n processos!
    cmd = cmd->next;
}

// ✅ CORRETO
while (cmd)
{
    pid = fork();
    if (pid == 0)  // apenas filho continua
    {
        // executar comando
        exit(0);
    }
    cmd = cmd->next;
}
```

### 3. Pipes - Fechar FDs no Pai
```c
// ❌ ERRADO - pipe fica aberto, processo fica travado!
pipe(fd);
if (fork() == 0)
{
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execve(...);
}
// faltou fechar no pai!

// ✅ CORRETO
pipe(fd);
if (fork() == 0)
{
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execve(...);
}
close(fd[1]);  // fechar escrita no pai!
```

### 4. Wait nos Processos Corretos
```c
// ✅ Sempre esperar todos os filhos
int i = 0;
while (i < num_cmds)
{
    waitpid(pids[i], &status, 0);
    i++;
}
// Último status é o que importa para $?
```

### 5. Built-ins em Pipes
```c
// ❌ ERRADO - export no filho não afeta pai!
if (fork() == 0)
{
    builtin_export(...);  // não afeta shell pai!
    exit(0);
}

// ✅ CORRETO
// Se comando é único E é builtin -> executar no pai
if (!cmd->next && is_builtin(cmd->args[0]))
{
    return (execute_builtin(cmd, mini));
}
```

### 6. Path com / vs PATH
```c
// Testar se tem barra primeiro!
if (ft_strchr(cmd, '/'))
{
    // Caminho absoluto ou relativo
    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));
}
else
{
    // Buscar no PATH
    // ...
}
```

### 7. Exit Status
```c
// Capturar corretamente o exit status
waitpid(pid, &status, 0);

if (WIFEXITED(status))
    exit_code = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
    exit_code = 128 + WTERMSIG(status);
```

---

## 🔧 Funções Auxiliares Úteis

### Verificar se String é Numérica
```c
int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
```

### Converter String para Int (atoi)
```c
int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
```

### Contar Argumentos
```c
int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}
```

### Verificar Caractere em String
```c
char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}
```

### Criar Path Completo
```c
char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}
```

---

## 📝 Checklist de Conclusão - Pessoa B

### Built-ins
- [ ] `pwd` funcionando
- [ ] `env` listando variáveis
- [ ] `echo` com e sem `-n`
- [ ] `export` adicionando variáveis
- [ ] `unset` removendo variáveis
- [ ] `cd` com paths absolutos/relativos/home
- [ ] `exit` com e sem argumento numérico

### Execução
- [ ] Comandos absolutos funcionam (/bin/ls)
- [ ] Comandos relativos funcionam (./script)
- [ ] Comandos do PATH funcionam (ls, grep, cat)
- [ ] Exit status capturado corretamente
- [ ] Erros tratados (comando não encontrado)

### Redirecionamentos
- [ ] `<` funciona
- [ ] `>` funciona
- [ ] `>>` funciona
- [ ] `<<` funciona (heredoc)
- [ ] Múltiplos redirecionamentos no mesmo comando

### Pipes
- [ ] Pipe simples funciona (2 comandos)
- [ ] Múltiplos pipes funcionam (3+ comandos)
- [ ] Pipes com redirecionamentos
- [ ] Todos FDs fechados corretamente

### Sinais
- [ ] Ctrl-C funciona no prompt
- [ ] Ctrl-C funciona durante execução
- [ ] Ctrl-D fecha o shell
- [ ] Ctrl-\ ignorado no prompt
- [ ] Ctrl-\ funciona durante execução

### Memory & FDs
- [ ] ZERO memory leaks no valgrind
- [ ] ZERO FDs abertos após execução (lsof)
- [ ] Funções passam norminette

---

## 🤝 Pontos de Integração com Pessoa A

### Você usa da Pessoa A:
1. `t_cmd *cmd_list` - Lista de comandos parseados
2. `t_env *env` - Lista de variáveis de ambiente
3. `get_env_value(env, key)` - Buscar variável (para PATH, HOME, etc)
4. `set_env_value(&env, key, value)` - Export
5. `unset_env_value(&env, key)` - Unset
6. `env_to_array(env)` - Converter para char** (execve precisa)
7. `print_env(env)` - Listar variáveis (comando env)

### Você fornece para Pessoa A:
1. `mini->last_exit_status` - Atualizar após cada execução (para $?)
2. `execute_cmd_list()` - Executor principal chamado pelo main

### Contrato de Interface:
```c
// Pessoa A chama:
mini->last_exit_status = execute_cmd_list(mini->cmd_list, mini);

// Pessoa B usa:
char *path_var = get_env_value(mini->env, "PATH");
char **envp = env_to_array(mini->env);
execve(path, cmd->args, envp);
```

---

## 🎓 Conceitos Importantes

### Fork, Exec, Wait
```c
pid_t pid = fork();
if (pid == -1)
    return (ERROR);  // erro
else if (pid == 0)
{
    // PROCESSO FILHO
    execve(path, args, envp);
    exit(127);  // se execve falhar
}
else
{
    // PROCESSO PAI
    waitpid(pid, &status, 0);
}
```

### Pipes
```c
int fd[2];
pipe(fd);  // fd[0]=leitura, fd[1]=escrita

// Filho 1 (escreve)
if (fork() == 0)
{
    close(fd[0]);  // não vai ler
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    execve(cmd1);
}

// Filho 2 (lê)
if (fork() == 0)
{
    close(fd[1]);  // não vai escrever
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    execve(cmd2);
}

// Pai
close(fd[0]);
close(fd[1]);
wait(NULL);
wait(NULL);
```

### Signals
```c
#include <signal.h>

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
```

---

## 🐛 Debugging Tips

### Verificar FDs Abertos
```bash
# Durante execução do minishell
lsof -p $(pgrep minishell)

# Deve mostrar apenas stdin, stdout, stderr (0, 1, 2)
```

### Debugar Pipes com strace
```bash
strace -f ./minishell
# Vai mostrar todos system calls (pipe, fork, execve, etc)
```

### GDB com Fork
```bash
gdb ./minishell
(gdb) set follow-fork-mode child  # seguir processo filho
(gdb) break execute_simple_cmd
(gdb) run
```

### Verificar Exit Status
```bash
minishell$ ls
minishell$ echo $?  # deve ser 0
minishell$ ls /nonexistent
minishell$ echo $?  # deve ser 2
```

---

## 📚 Man Pages Essenciais

```bash
man fork
man execve
man wait
man waitpid
man pipe
man dup2
man access
man open
man close
man signal
man getcwd
man chdir
```

---

## 💡 Dica Final

**Implemente incrementalmente!** Não tente fazer tudo de uma vez. A ordem recomendada é:
1. Built-ins simples (pwd, echo, env)
2. Executor básico (comandos sem pipe)
3. Redirecionamentos
4. Pipes (a parte mais complexa)
5. Sinais
6. Refinamentos e edge cases

Teste cada parte antes de avançar. Use prints para debugar o fluxo dos FDs!

---

Boa sorte, Pessoa B! 🚀 Qualquer dúvida, consulte o código da Pessoa A ou peça ajuda!
