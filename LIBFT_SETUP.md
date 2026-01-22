# 📚 Setup da Libft

## Como adicionar a libft ao projeto

### Opção 1: Copiar sua libft existente

```bash
# Copiar sua libft para o projeto
cp -r /caminho/para/sua/libft ./libft

# Ou criar um link simbólico
ln -s /caminho/para/sua/libft ./libft
```

### Opção 2: Clonar do repositório

```bash
# Se sua libft está no GitHub
git clone <url-da-sua-libft> libft

# Ou adicionar como submódulo
git submodule add <url-da-sua-libft> libft
```

### Estrutura esperada

```
minishell/
├── libft/
│   ├── Makefile
│   ├── libft.h
│   ├── ft_strlen.c
│   ├── ft_strdup.c
│   ├── ft_strjoin.c
│   ├── ft_strcmp.c (ou ft_strncmp.c)
│   ├── ft_split.c
│   ├── ft_calloc.c
│   └── ... (outras funções)
└── ...
```

## Funções da libft usadas no minishell

O projeto usa as seguintes funções da libft:

### String Manipulation
- `ft_strlen()` - Tamanho da string
- `ft_strdup()` - Duplicar string
- `ft_strjoin()` - Juntar duas strings
- `ft_strcmp()` ou `ft_strncmp()` - Comparar strings
- `ft_split()` - Separar string por delimitador
- `ft_strchr()` - Encontrar caractere em string
- `ft_substr()` - Extrair substring

### Memory Management
- `ft_calloc()` - Alocar memória zerada
- `ft_memset()` - Preencher memória

### Character Checks
- `ft_isalnum()` - Verificar alfanumérico
- `ft_isdigit()` - Verificar dígito
- `ft_isalpha()` - Verificar letra

### Conversion
- `ft_atoi()` - String para inteiro
- `ft_itoa()` - Inteiro para string

## Notas importantes

### Se ft_strcmp não existir na sua libft

Você pode usar `ft_strncmp` ou adicionar `ft_strcmp`:

```c
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
```

### Se alguma função estiver faltando

Adicione as funções necessárias à sua libft ou crie no diretório `src/utils/`.

## Compilação

Depois de adicionar a libft:

```bash
# Compilar tudo
make

# Testar se está funcionando
./minishell
```

## Troubleshooting

### Erro: libft.h not found
```bash
# Verifique se a libft está no diretório correto
ls libft/libft.h

# Se não estiver, adicione conforme instruções acima
```

### Erro: undefined reference to ft_strlen
```bash
# Compile a libft primeiro
cd libft && make && cd ..

# Depois compile o minishell
make
```

### Erro de linking
```bash
# Limpe e recompile tudo
make fclean
make
```
