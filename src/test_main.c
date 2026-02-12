/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/12 17:48:13 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Main de teste para validar env e lexer
 * 
 * Compila com: make test_env
 */

static void	test_env_functions(t_mini *mini)
{
	char	*value;
	char	**env_arr;
	int		i;

	printf("\n========== TESTANDO ENV ==========\n\n");

	// Teste 1: Obter variavel existente
	printf("1. Testando get_env_value:\n");
	value = get_env_value(mini->env, "HOME");
	printf("   HOME = %s\n", value ? value : "(null)");
	value = get_env_value(mini->env, "USER");
	printf("   USER = %s\n", value ? value : "(null)");
	value = get_env_value(mini->env, "PATH");
	printf("   PATH = %.50s...\n", value ? value : "(null)");

	// Teste 2: Obter variavel inexistente
	printf("\n2. Testando variavel inexistente:\n");
	value = get_env_value(mini->env, "VAR_NAO_EXISTE");
	printf("   VAR_NAO_EXISTE = %s\n", value ? value : "(null - correto!)");

	// Teste 3: Criar nova variavel
	printf("\n3. Testando set_env_value (nova variavel):\n");
	set_env_value(&mini->env, "MINHA_VAR", "valor_teste");
	value = get_env_value(mini->env, "MINHA_VAR");
	printf("   MINHA_VAR = %s\n", value ? value : "(null)");

	// Teste 4: Atualizar variavel existente
	printf("\n4. Testando set_env_value (atualizar):\n");
	set_env_value(&mini->env, "MINHA_VAR", "novo_valor");
	value = get_env_value(mini->env, "MINHA_VAR");
	printf("   MINHA_VAR = %s\n", value ? value : "(null)");

	// Teste 5: Remover variavel
	printf("\n5. Testando unset_env_value:\n");
	unset_env_value(&mini->env, "MINHA_VAR");
	value = get_env_value(mini->env, "MINHA_VAR");
	printf("   MINHA_VAR = %s\n", value ? value : "(null - removida!)");

	// Teste 6: Converter para array
	printf("\n6. Testando env_to_array (primeiras 5 vars):\n");
	env_arr = env_to_array(mini->env);
	if (env_arr)
	{
		i = 0;
		while (env_arr[i] && i < 5)
		{
			printf("   [%d] %.60s...\n", i, env_arr[i]);
			i++;
		}
		// Contar quantos elementos para liberar
		while (env_arr[i])
			i++;
		ft_free_arr(env_arr, i);
	}
	printf("\n========== ENV OK ==========\n\n");
}

static void	test_lexer(void)
{
	t_token	*tokens;
	t_token	*current;
	char	*test_inputs[] = {
		"ls -la",
		"echo hello world",
		"cat < file.txt",
		"ls | grep test",
		"echo \"hello world\"",
		"echo 'hello $USER'",
		NULL
	};
	int		i;

	printf("\n========== TESTANDO LEXER ==========\n\n");
	i = 0;
	while (test_inputs[i])
	{
		printf("Input: \"%s\"\n", test_inputs[i]);
		printf("Tokens: ");
		tokens = lexer(test_inputs[i]);
		current = tokens;
		while (current)
		{
			printf("[%d:\"%s\"] ", current->type, current->value);
			current = current->next;
		}
		printf("\n\n");
		free_tokens(tokens);
		i++;
	}
	printf("========== LEXER OK ==========\n\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	printf("\n🧪 MINISHELL - Testes de ENV e LEXER 🧪\n");
	printf("==========================================\n");

	// Inicializar ambiente
	mini.last_exit_status = 0;
	mini.env = init_env(envp);
	if (!mini.env)
	{
		printf("❌ Erro ao inicializar env!\n");
		return (1);
	}
	printf("✅ Env inicializado com sucesso!\n");

	// Testar funções de env
	test_env_functions(&mini);

	// Testar lexer
	test_lexer();

	// Cleanup
	free_env(mini.env);
	printf("✅ Cleanup completo!\n");
	return (0);
}
