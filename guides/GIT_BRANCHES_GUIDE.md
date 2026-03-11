# 🌿 Guia de Branches - Trabalho em Dupla

## 📖 O que são Branches?

Branches (ramificações) são como "linhas do tempo paralelas" do seu código. Permitem que vocês dois trabalhem em features diferentes sem interferir no trabalho um do outro.

```
main ──────●──────●──────●──────●────── (código estável)
            \           /
             \         /
feature/lexer ●───●───● (Pessoa A trabalhando)
```

---

## 🎯 Estratégia para o Minishell

### Branch Principal
- `main` - Código estável e testado

### Branches da Pessoa A (Parsing)
- `feature/environment` - Sistema de variáveis de ambiente
- `feature/lexer` - Tokenização
- `feature/parser` - Construção de comandos
- `feature/expander` - Expansão de variáveis

### Branches da Pessoa B (Execução)
- `feature/builtins` - Built-ins (echo, cd, pwd, etc)
- `feature/executor` - Executor de comandos
- `feature/redirections` - Redirecionamentos
- `feature/pipes` - Pipes

---

## 🚀 Comandos Básicos

### 1. Ver branches disponíveis
```bash
git branch                    # locais
git branch -a                 # todas (incluindo remotas)
```

### 2. Criar uma nova branch
```bash
# Criar e já mudar para a branch
git checkout -b feature/environment

# Ou em dois comandos
git branch feature/environment
git checkout feature/environment
```

### 3. Mudar de branch
```bash
git checkout main
git checkout feature/lexer
```

### 4. Ver em qual branch você está
```bash
git branch                    # branch atual tem um *
# ou
git status                    # primeira linha mostra a branch
```

### 5. Salvar seu trabalho na branch
```bash
git add .
git commit -m "✨ feat(env): implement init_env function"
git push origin feature/environment
```

### 6. Trazer atualizações da main
```bash
# Estando na sua branch
git checkout feature/environment
git pull origin main          # pega mudanças da main
```

### 7. Juntar sua branch na main (merge)
```bash
# Voltar para main
git checkout main

# Pegar atualizações
git pull origin main

# Juntar sua branch
git merge feature/environment

# Enviar para o remoto
git push origin main
```

### 8. Deletar branch após merge
```bash
# Local
git branch -d feature/environment

# Remota
git push origin --delete feature/environment
```

---

## 📋 Workflow Recomendado

### Setup Inicial (Uma vez só)

```bash
# 1. Clonar o repositório
git clone <url-do-repo>
cd minishell

# 2. Cada pessoa cria suas branches
# Pessoa A:
git checkout -b feature/environment

# Pessoa B:
git checkout -b feature/builtins
```

### Ciclo de Desenvolvimento Diário

#### Antes de começar a trabalhar:
```bash
# 1. Ver em qual branch você está
git branch

# 2. Se não estiver na sua branch, mude
git checkout feature/environment

# 3. Pegar atualizações da main
git pull origin main

# 4. Começar a trabalhar!
```

#### Durante o trabalho:
```bash
# Fazer commits frequentes
git add src/env/env_init.c
git commit -m "✨ feat(env): implement create_env_node"

git add src/env/env_get.c
git commit -m "✨ feat(env): implement get_env_value"

# Enviar para o repositório remoto
git push origin feature/environment
```

#### Quando terminar uma feature:
```bash
# 1. Commit final
git add .
git commit -m "✅ feat(env): complete environment management system"
git push origin feature/environment

# 2. Voltar para main
git checkout main

# 3. Atualizar main
git pull origin main

# 4. Fazer merge da sua branch
git merge feature/environment

# 5. Testar se tudo funciona
make
./minishell

# 6. Se tudo OK, enviar para remoto
git push origin main

# 7. Deletar branch local (opcional)
git branch -d feature/environment
```

---

## 🎯 Exemplo Prático: Pessoa A

### Dia 1 - Implementar Environment
```bash
# Criar branch
git checkout -b feature/environment

# Trabalhar no código
vim src/env/env_init.c
# ... implementar funções ...

# Salvar progresso
git add src/env/env_init.c
git commit -m "✨ feat(env): implement init_env and create_env_node"

git add src/env/env_get.c
git commit -m "✨ feat(env): implement get_env_value"

# Enviar para remoto
git push origin feature/environment

# Fim do dia
```

### Dia 2 - Completar Environment
```bash
# Continuar na mesma branch
git checkout feature/environment

# Pegar atualizações (caso Pessoa B tenha feito merge)
git pull origin main

# Trabalhar
vim src/env/env_set.c
# ... implementar ...

git add src/env/
git commit -m "✨ feat(env): implement set/unset env functions"
git push origin feature/environment

# Testar
make
# ... testar ...

# Fazer merge na main
git checkout main
git pull origin main
git merge feature/environment
git push origin main

# Criar próxima branch
git checkout -b feature/lexer
```

---

## 🎯 Exemplo Prático: Pessoa B

### Dia 1 - Implementar Builtins Simples
```bash
# Criar branch
git checkout -b feature/builtins

# Trabalhar
vim src/builtins/builtin_pwd.c
vim src/builtins/builtin_echo.c

# Commits
git add src/builtins/builtin_pwd.c
git commit -m "✨ feat(builtins): implement pwd"

git add src/builtins/builtin_echo.c
git commit -m "✨ feat(builtins): implement echo with -n flag"

git push origin feature/builtins
```

### Dia 2 - Completar Builtins
```bash
# Continuar
git checkout feature/builtins
git pull origin main

# Trabalhar
vim src/builtins/builtin_cd.c
vim src/builtins/builtin_export.c

git add src/builtins/
git commit -m "✨ feat(builtins): implement cd and export"
git push origin feature/builtins

# Merge
git checkout main
git pull origin main
git merge feature/builtins
git push origin main

# Próxima feature
git checkout -b feature/executor
```

---

## 🔄 Sincronização entre Pessoas

### Pessoa A precisa de código da Pessoa B
```bash
# Pessoa B fez merge de builtins na main
# Pessoa A quer usar na sua branch

git checkout feature/parser
git pull origin main           # pega atualizações da main
# Agora você tem o código da Pessoa B!
```

### Ambos trabalhando ao mesmo tempo
```bash
# Pessoa A na branch feature/lexer
# Pessoa B na branch feature/builtins
# Nenhum conflito! Cada um no seu quadrado

# Quando terminarem, um faz merge primeiro:
# Pessoa B:
git checkout main
git merge feature/builtins
git push origin main

# Pessoa A (depois):
git checkout main
git pull origin main           # pega o merge da Pessoa B
git merge feature/lexer        # agora junta o seu
git push origin main
```

---

## ⚠️ Problemas Comuns e Soluções

### 1. "Esqueci de criar uma branch e comitei na main!"
```bash
# Criar branch com seu código
git branch feature/minha-feature

# Voltar main para antes dos seus commits
git checkout main
git reset --hard origin/main

# Voltar para sua branch
git checkout feature/minha-feature
# Seu código está salvo aqui!
```

### 2. "Quero descartar mudanças não salvas"
```bash
# Descartar mudanças em um arquivo
git checkout -- arquivo.c

# Descartar TODAS mudanças não salvas
git reset --hard
```

### 3. "Conflito ao fazer merge!"
```bash
# Git mostra algo assim no arquivo:
# <<<<<<< HEAD
# código da main
# =======
# seu código
# >>>>>>> feature/sua-branch

# 1. Abrir o arquivo e resolver manualmente
vim arquivo.c
# Escolher qual código manter ou mesclar ambos

# 2. Marcar como resolvido
git add arquivo.c

# 3. Completar o merge
git commit -m "🔀 merge: resolve conflicts"
```

### 4. "Mudei de branch sem commitar!"
```bash
# Opção 1: Salvar temporariamente (stash)
git stash
git checkout outra-branch
# ... fazer algo ...
git checkout branch-original
git stash pop                  # recupera suas mudanças

# Opção 2: Commitar antes
git add .
git commit -m "🚧 wip: work in progress"
git checkout outra-branch
```

### 5. "Quero ver diferenças antes do merge"
```bash
# Ver diferenças entre branches
git diff main..feature/lexer

# Ver lista de commits
git log main..feature/lexer
```

---

## 📊 Visualizar Histórico

### Ver histórico bonito
```bash
git log --oneline --graph --all

# Saída exemplo:
# * 3a7d9e2 (feature/lexer) ✨ feat(lexer): implement tokenization
# * 1b2c3d4 (main) ✨ feat(env): complete environment system
# * 5e6f7g8 🎉 feat: initial project structure
```

### Ver quem mudou um arquivo
```bash
git log --oneline src/env/env_init.c
```

### Ver mudanças de um commit específico
```bash
git show 3a7d9e2
```

---

## 🎓 Boas Práticas

### ✅ DO (Fazer):
- ✅ Criar uma branch para cada feature
- ✅ Fazer commits pequenos e frequentes
- ✅ Escrever mensagens descritivas
- ✅ Testar antes de fazer merge na main
- ✅ Fazer pull da main antes de merge
- ✅ Deletar branches após merge

### ❌ DON'T (Não Fazer):
- ❌ Trabalhar direto na main
- ❌ Fazer commits gigantes com várias mudanças
- ❌ Mensagens vagas: "fix", "update", "test"
- ❌ Fazer merge sem testar
- ❌ Esquecer de fazer push
- ❌ Deixar branches velhas acumulando

---

## 🎯 Cheatsheet Rápido

```bash
# Criar e mudar para branch
git checkout -b feature/nome

# Ver branches
git branch

# Mudar de branch
git checkout nome-da-branch

# Salvar trabalho
git add .
git commit -m "mensagem"
git push origin nome-da-branch

# Atualizar com main
git checkout sua-branch
git pull origin main

# Fazer merge na main
git checkout main
git pull origin main
git merge sua-branch
git push origin main

# Deletar branch
git branch -d nome-da-branch
```

---

## 📞 Ajuda Rápida

**Estou perdido, em qual branch estou?**
```bash
git status
# ou
git branch
```

**Esqueci o nome da minha branch!**
```bash
git branch -a
```

**Quero descartar tudo e recomeçar!**
```bash
git checkout main
git reset --hard origin/main
git clean -fd
```

**Fiz besteira, como voltar atrás?**
```bash
# Ver histórico
git reflog

# Voltar para um commit anterior
git reset --hard abc1234
```

---

## 🎮 Prática Sugerida

Antes de começar o projeto, façam um teste:

```bash
# Pessoa A
git checkout -b test/pessoa-a
echo "Olá da Pessoa A" > teste-a.txt
git add teste-a.txt
git commit -m "test: add file from person A"
git push origin test/pessoa-a

# Pessoa B
git checkout -b test/pessoa-b
echo "Olá da Pessoa B" > teste-b.txt
git add teste-b.txt
git commit -m "test: add file from person B"
git push origin test/pessoa-b

# Pessoa A faz merge
git checkout main
git merge test/pessoa-a
git push origin main

# Pessoa B faz merge
git checkout main
git pull origin main
git merge test/pessoa-b
git push origin main

# Verificar se os dois arquivos estão lá
ls teste-*.txt

# Limpar
git branch -d test/pessoa-a test/pessoa-b
git push origin --delete test/pessoa-a test/pessoa-b
rm teste-*.txt
```

---

**Dúvidas?** Pratiquem juntos antes de começar! 🚀
