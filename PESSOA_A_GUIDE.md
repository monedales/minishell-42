# 🎯 Guia de Implementação - PESSOA A (Parsing & Organização)

## 📊 Ordem de Implementação Recomendada

### FASE 1: Fundação (Ambiente)
Comece pelo sistema de variáveis de ambiente, pois ele é usado por todas as outras partes.

#### 1.1 Environment Básico (PRIORIDADE MÁXIMA)
```c
✅ src/env/env_init.c
   - create_env_node()
   - add_env_node()
   - split_env_string()
   - init_env()
   - free_env()

✅ src/env/env_get.c
   - get_env_value()

✅ src/env/env_utils.c
   - count_env_vars()
   - env_to_array()  (necessário para execve)
   - print_env()     (necessário para builtin env)
```

**TESTE INTERMEDIÁRIO:**
```c
// No main temporário, testar:
t_env *env = init_env(envp);
printf("%s\n", get_env_value(env, "HOME"));
print_env(env);
free_env(env);
```

#### 1.2 Environment Modificável
```c
✅ src/env/env_set.c
   - update_env_node()
   - set_env_value()

✅ src/env/env_unset.c
   - unset_env_value()
```

**TESTE:**
```c
set_env_value(&env, "MYVAR", "42");
printf("%s\n", get_env_value(env, "MYVAR"));  // 42
unset_env_value(&env, "MYVAR");
printf("%s\n", get_env_value(env, "MYVAR"));  // NULL
```

---

### FASE 2: Lexer (Tokenização)

#### 2.1 Lexer Básico (Palavras e Pipes)
```c
✅ src/parsing/lexer.c
   - create_token()
   - add_token()
   - Implementar lexer() SIMPLIFICADO:
     * Ignorar aspas por enquanto
     * Apenas separar por espaços e pipes
     * Identificar '|' como TOKEN_PIPE
     * Todo o resto é TOKEN_WORD
```

**TESTE:**
```c
t_token *tokens = lexer("echo hello | grep h");
// Esperado: [WORD:echo] [WORD:hello] [PIPE] [WORD:grep] [WORD:h]
```

#### 2.2 Adicionar Redirecionamentos
```c
✅ Expandir identify_token_type()
   - Detectar '<', '>', '>>', '<<'

✅ Testar:
   lexer("cat < in.txt > out.txt")
   // [WORD:cat] [REDIR_IN] [WORD:in.txt] [REDIR_OUT] [WORD:out.txt]
```

#### 2.3 Suporte a Aspas
```c
✅ src/parsing/quotes.c
   - is_in_quotes()
   - validate_quotes()

✅ Atualizar lexer.c
   - extract_word() com máquina de estados para aspas
   - "hello world" vira TOKEN_WORD com value "hello world"
```

**TESTE:**
```c
lexer("echo \"hello world\"")
// [WORD:echo] [WORD:"hello world"]  (com aspas ainda)
```

---

### FASE 3: Parser (Construção dos Comandos)

#### 3.1 Parser Básico (Sem Redirecionamentos)
```c
✅ src/parsing/parser.c
   - create_cmd_node()
   - add_arg_to_cmd()
   - validate_syntax()
   - parser() SIMPLIFICADO:
     * Ignorar redirecionamentos por enquanto
     * Focar em separar por pipes
     * Construir t_cmd com apenas args
```

**TESTE:**
```c
tokens = lexer("echo hello | grep h");
t_cmd *cmds = parser(tokens);
// cmd1: args=["echo", "hello"]
// cmd2: args=["grep", "h"]
```

#### 3.2 Adicionar Redirecionamentos
```c
✅ Expandir parser.c
   - create_redir_node()
   - add_redir_to_cmd()
   - Processar tokens de redirecionamento
```

**TESTE:**
```c
tokens = lexer("cat < in.txt > out.txt");
t_cmd *cmd = parser(tokens);
// cmd->args = ["cat"]
// cmd->redirs = [<in.txt] -> [>out.txt]
```

---

### FASE 4: Expander (Variáveis)

#### 4.1 Expansão Básica
```c
✅ src/parsing/expander.c
   - is_var_start()
   - extract_var_name()
   - expand_string() SEM aspas
   - expand_tokens()
```

**TESTE:**
```c
set_env_value(&mini->env, "USER", "mona");
tokens = lexer("echo $USER");
expand_tokens(tokens, mini);
// token[1]->value agora é "mona"
```

#### 4.2 Exit Status ($?)
```c
✅ Expandir expand_string()
   - Detectar "$?"
   - Substituir por mini->last_exit_status
```

**TESTE:**
```c
mini->last_exit_status = 127;
tokens = lexer("echo $?");
expand_tokens(tokens, mini);
// token[1]->value agora é "127"
```

#### 4.3 Respeitar Aspas
```c
✅ Usar is_single_quoted()
   - Não expandir dentro de aspas simples
   - Expandir dentro de aspas duplas

✅ src/parsing/quotes.c
   - remove_quotes()
```

**TESTE:**
```c
tokens = lexer("echo '$USER'");
expand_tokens(tokens, mini);
// Não deve expandir: token->value = "'$USER'"

tokens = lexer("echo \"$USER\"");
expand_tokens(tokens, mini);
// Deve expandir: token->value = "\"mona\""
```

---

## 🧪 Estratégia de Teste Progressivo

### Teste 1: Apenas ENV
```bash
./minishell
minishell$ env  # Deve listar todas variáveis
```

### Teste 2: Comando Simples (precisa da Pessoa B)
```bash
minishell$ ls
minishell$ pwd
```

### Teste 3: Variáveis
```bash
minishell$ echo $HOME
minishell$ echo $USER
minishell$ export VAR=42
minishell$ echo $VAR
```

### Teste 4: Aspas
```bash
minishell$ echo "hello world"
minishell$ echo 'hello $USER'
minishell$ echo "hello $USER"
```

### Teste 5: Pipes (precisa da Pessoa B)
```bash
minishell$ echo hello | grep h
minishell$ ls | wc -l
```

### Teste 6: Redirecionamentos (precisa da Pessoa B)
```bash
minishell$ echo test > file.txt
minishell$ cat < file.txt
minishell$ echo append >> file.txt
```

---

## ⚠️ Armadilhas Comuns

### 1. Memory Leaks
```c
// SEMPRE dar free em strings duplicadas
char *value = ft_strdup("test");
// ... usar value ...
free(value);
```

### 2. NULL Pointers
```c
// SEMPRE verificar retorno de malloc
t_token *token = malloc(sizeof(t_token));
if (!token)
    return (NULL);
```

### 3. Aspas Não Fechadas
```c
// Validar ANTES de tokenizar
if (!validate_quotes(input))
{
    print_error("syntax error: unclosed quote");
    return (NULL);
}
```

### 4. Variável Não Encontrada
```c
// $VAR inexistente deve ser substituído por string vazia, não NULL
char *value = get_env_value(env, "NONEXISTENT");
if (!value)
    value = "";  // Não remover o $VAR, apenas usar string vazia
```

### 5. Lista Ligada Mal Finalizada
```c
// SEMPRE garantir que último nó tem next = NULL
node->next = NULL;
```

---

## 📝 Checklist de Conclusão - Pessoa A

- [ ] Environment funciona (get, set, unset, print)
- [ ] Lexer identifica todos tipos de token
- [ ] Lexer respeita aspas
- [ ] Parser cria estrutura t_cmd corretamente
- [ ] Parser valida sintaxe (erros de pipe, redir)
- [ ] Expander substitui variáveis
- [ ] Expander substitui $?
- [ ] Expander respeita aspas simples
- [ ] Aspas são removidas após expansão
- [ ] ZERO memory leaks no valgrind (parte parsing)
- [ ] Todas funções passam norminette

---

## 🤝 Pontos de Integração com Pessoa B

### Você fornece:
1. `t_cmd *cmd_list` - Lista de comandos prontos para executar
2. `t_env *env` - Lista de variáveis de ambiente
3. `env_to_array()` - Converte env para char** (execve precisa)
4. `get_env_value()` - Pessoa B usa para buscar PATH

### Você usa da Pessoa B:
1. `execute_cmd_list()` - Executa seus comandos parseados
2. `mini->last_exit_status` - Atualizado após cada execução

---

## 📚 Funções Auxiliares Úteis

```c
// Verificar se string é válida
int	ft_isalnum(int c)
{
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}

// Verificar caractere de variável
int	is_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

// Copiar substring
char	*ft_substr(char *s, int start, int len)
{
    char	*sub;
    int		i;

    sub = malloc(len + 1);
    if (!sub)
        return (NULL);
    i = 0;
    while (i < len)
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return (sub);
}
```

---

Boa sorte! Foque em fazer uma parte de cada vez, testando antes de avançar. 🚀
