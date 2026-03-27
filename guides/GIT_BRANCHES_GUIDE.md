# 🌿 Guia de Git — Minishell

## Branches

O projeto usa a branch `main` como branch principal.
Branches de feature seguem o padrão `feature/nome-da-feature`.

---

## Fluxo de trabalho

### Antes de começar a trabalhar
```bash
git checkout main
git pull origin main
git checkout -b feature/nome-da-feature
```

### Durante o desenvolvimento
```bash
# Commits frequentes e descritivos
git add src/arquivo.c
git commit -m "✨ feat: implement feature X"
git push origin feature/nome-da-feature
```

### Quando a feature estiver pronta
```bash
# 1. Garantir que norminette passa
~/Library/Python/3.9/bin/norminette src/ include/

# 2. Garantir que compila limpo
make re

# 3. Fazer merge na main
git checkout main
git pull origin main
git merge feature/nome-da-feature
git push origin main

# 4. Deletar branch local
git branch -d feature/nome-da-feature
```

---

## Comandos úteis

```bash
# Ver em qual branch está
git status
git branch

# Ver histórico visual
git log --oneline --graph --all

# Salvar mudanças temporariamente
git stash
git stash pop

# Ver diferenças entre branches
git diff main..feature/nome

# Descartar mudanças não commitadas
git restore arquivo.c       # um arquivo
git restore .               # tudo
```

---

## Mensagens de commit

Seguimos o padrão Conventional Commits com emojis:

| Tipo     | Emoji | Quando usar                        |
|----------|-------|------------------------------------|
| feat     | ✨    | Nova funcionalidade                |
| fix      | 🐛    | Correção de bug                    |
| refactor | ♻️    | Refatoração sem mudar comportamento|
| docs     | 📝    | Documentação                       |
| style    | 🎨    | Norminette / formatação            |
| test     | 🧪    | Testes                             |
| chore    | 🔧    | Makefile, config, etc              |

Exemplos:
```bash
git commit -m "✨ feat: implement heredoc redirection"
git commit -m "🐛 fix: handle SIGINT correctly in pipeline"
git commit -m "♻️ refactor: split redirections into utils file"
git commit -m "🎨 style: fix norminette errors in expander"
```

---

## Resolver conflitos

```bash
# Durante o merge, se houver conflito:
# O arquivo terá marcadores:
# <<<<<<< HEAD
# seu código
# =======
# código da outra branch
# >>>>>>> feature/nome

# 1. Editar o arquivo e resolver manualmente
# 2. Marcar como resolvido
git add arquivo.c
git commit -m "🔀 merge: resolve conflicts in arquivo.c"
```

---

## Reverter erros

```bash
# Ver histórico com hashes
git reflog

# Voltar para um commit anterior (CUIDADO: descarta commits)
git reset --hard abc1234

# Desfazer último commit mas manter as mudanças
git reset --soft HEAD~1
```
