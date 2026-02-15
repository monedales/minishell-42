#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Estruturas simplificadas para teste


// Funções auxiliares simplificadas
char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strchr(const char *s, int c)
{
	return (strchr(s, c));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	return (strncmp(s1, s2, n));
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write(fd, s, strlen(s));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	len1 = strlen(s1);
	len2 = strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		count;
	int		i;
	int		start;
	int		j;

	if (!s)
		return (NULL);
	
	// Conta palavras
	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}

	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);

	// Divide
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			result[j] = malloc(i - start + 1);
			if (!result[j])
				return (NULL);
			strncpy(result[j], s + start, i - start);
			result[j][i - start] = '\0';
			j++;
		}
	}
	result[j] = NULL;
	return (result);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// Inclui a implementação
#include "executor_simple.c"

// Funções auxiliares de teste
t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	add_env_node(t_mini *mini, char *key, char *value)
{
	t_env	*new_node;
	t_env	*temp;

	new_node = create_env_node(key, value);
	if (!new_node)
		return;

	if (!mini->env)
	{
		mini->env = new_node;
		return;
	}

	temp = mini->env;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	new_node->prev = temp;
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = temp;
	}
}

void	test_command(t_mini *mini, char **args)
{
	t_cmd	cmd;
	int		status;

	printf("\n========================================\n");
	printf("Testando comando: ");
	for (int i = 0; args[i]; i++)
		printf("%s ", args[i]);
	printf("\n========================================\n");

	cmd.args = args;
	cmd.redirs = NULL;
	cmd.next = NULL;

	status = execute_simple_cmd(&cmd, mini);
	printf("\nExit status: %d\n", status);
}

int	main(void)
{
	t_mini	mini;
	char	*path_value;

	// Inicializa
	mini.env = NULL;
	mini.cmd_list = NULL;
	mini.last_exit_status = 0;
	mini.running = 1;

	// Cria environment básico
	path_value = getenv("PATH");
	if (path_value)
		add_env_node(&mini, "PATH", path_value);
	else
		add_env_node(&mini, "PATH", "/usr/local/bin:/usr/bin:/bin");
	
	add_env_node(&mini, "USER", "testuser");
	add_env_node(&mini, "HOME", "/home/testuser");

	printf("===========================================\n");
	printf("TESTE DO EXECUTE_SIMPLE_CMD\n");
	printf("===========================================\n");

	// Teste 1: ls
	char *args1[] = {"ls", "-la", NULL};
	test_command(&mini, args1);

	// Teste 2: echo
	char *args2[] = {"echo", "Hello, World!", NULL};
	test_command(&mini, args2);

	// Teste 3: pwd
	char *args3[] = {"pwd", NULL};
	test_command(&mini, args3);

	// Teste 4: cat com arquivo que não existe (deve dar erro)
	char *args4[] = {"cat", "/arquivo/que/nao/existe.txt", NULL};
	test_command(&mini, args4);

	// Teste 5: comando que não existe
	char *args5[] = {"comando_inexistente", NULL};
	test_command(&mini, args5);

	// Teste 6: ls com caminho absoluto
	char *args6[] = {"/bin/ls", "/tmp", NULL};
	test_command(&mini, args6);

	// Limpeza
	free_env_list(mini.env);

	printf("\n===========================================\n");
	printf("TESTES CONCLUÍDOS\n");
	printf("===========================================\n");

	return (0);
}