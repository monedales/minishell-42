# Agent Skills para Projetos 42

Esta pasta contém **Agent Skills** personalizadas para projetos da 42 School, compatíveis com GitHub Copilot e outros agentes que suportam o [padrão Agent Skills](https://github.com/agentskills/agentskills).

## Skills Disponíveis

### 🔍 norminette-checker
Verifica e corrige código C de acordo com as normas da 42 School.

**Quando usar:** Antes de commits, ao criar/modificar arquivos .c/.h, ou quando mencionar "norm" ou "norminette".

### 📐 42-patterns  
Padrões, estruturas e boas práticas comuns para projetos da 42.

**Quando usar:** Iniciando projetos, criando Makefiles, estruturando código.

### 🧠 philosophers-guide
Guia específico para o projeto Dining Philosophers (philo).

**Quando usar:** Trabalhando no projeto philosophers, debugando threads, sincronização ou race conditions.

### 📝 42-readme-generator
Gera READMEs completos e bem estruturados para projetos da 42.

**Quando usar:** Criando ou melhorando documentação de projetos.

## Como Funciona

As skills são automaticamente carregadas pelo Copilot quando relevantes para sua tarefa. Cada skill contém:

- **Frontmatter YAML** com nome e descrição
- **Instruções** sobre como realizar a tarefa
- **Exemplos** e templates
- **Boas práticas** específicas do contexto

## Estrutura

```
.github/skills/
├── norminette-checker/
│   └── SKILL.md
├── 42-patterns/
│   └── SKILL.md
├── philosophers-guide/
│   └── SKILL.md
└── 42-readme-generator/
    └── SKILL.md
```

## Formato Agent Skills

Cada `SKILL.md` segue o formato padrão:

```markdown
---
name: skill-name
description: O que a skill faz e quando usar
---

# Título da Skill

Conteúdo com instruções, exemplos e guidelines...
```

## Referências

- [GitHub Copilot Agent Skills](https://docs.github.com/pt/copilot/concepts/agents/about-agent-skills)
- [Agent Skills Standard](https://github.com/agentskills/agentskills)
- [Anthropic Skills Repository](https://github.com/anthropics/skills)
- [Awesome Copilot Collection](https://github.com/github/awesome-copilot)

## Uso

As skills funcionam com:
- GitHub Copilot coding agent
- GitHub Copilot CLI
- Visual Studio Code Insiders (agent mode)
- VS Code stable (em breve)

Não é necessário configuração adicional - basta ter os arquivos na estrutura `.github/skills/` ou `.claude/skills/`.
