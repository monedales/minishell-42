# 📚 Libft — Referência

A libft está em `libraries/libft/` e é compilada automaticamente pelo
Makefile do projeto.

---

## Compilação

```bash
# Compila libft + minishell
make

# Limpa tudo e recompila
make re
```

O Makefile do projeto detecta o sistema operacional:
- **macOS**: usa readline do Homebrew (`$(brew --prefix readline)`)
- **Linux**: usa `-lreadline` padrão

---

## Funções da libft usadas no projeto

### Strings
- `ft_strlen` — tamanho
- `ft_strdup` — duplicar
- `ft_strjoin` — concatenar dois strings
- `ft_strlcpy`, `ft_strlcat` — cópia/concatenação segura
- `ft_strncmp` — comparar
- `ft_strchr`, `ft_strrchr` — encontrar caractere
- `ft_substr` — extrair substring
- `ft_strtrim` — remover caracteres das bordas
- `ft_strnstr` — encontrar substring

### Memória
- `ft_calloc` — alocar memória zerada
- `ft_memset`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`
- `ft_memdel` — free + NULL

### Caracteres
- `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`
- `ft_isspace`
- `ft_toupper`, `ft_tolower`

### Conversão
- `ft_atoi`, `ft_atol` — string → inteiro/long
- `ft_itoa` — inteiro → string

### Output
- `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`
- `ft_printf` — printf próprio

### Outras
- `ft_split` — dividir string por delimitador
- `ft_free_arr` — liberar array de strings
- `ft_count_words` — contar palavras

---

## Troubleshooting

**libft.a não encontrado:**
```bash
cd libraries/libft && make && cd ../..
make
```

**Erro de linking (undefined reference):**
```bash
make fclean
make
```

**readline não encontrado no macOS:**
```bash
brew install readline
make re
```
