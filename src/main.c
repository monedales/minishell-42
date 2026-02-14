/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/14 14:29:42 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Inicializa a estrutura principal do minishell
 * 
 * @param mini Ponteiro para a estrutura principal
 * @param envp Array de strings com as variáveis de ambiente
 * @return SUCCESS ou ERROR
 */
static int	init_minishell(t_mini *mini, char **envp)
{
	mini->env = init_env(envp);
	if (!mini->env)
		return (ERROR);
	mini->cmd_list = NULL;
	mini->last_exit_status = 0;
	mini->running = TRUE;
	return (SUCCESS);
}

/**
 * @brief Processa uma linha de comando completa
 * 
 * @param line String lida do readline
 * @param mini Estrutura principal do minishell
 */
static void	process_line(char *line, t_mini *mini)
{
	t_token	*tokens;

	// 1. Lexer: Transforma a string em tokens
	tokens = lexer(line);
	if (!tokens)
		return ;
	
	// 2. Expander: Substitui variáveis ($VAR, $?)
	expand_tokens(tokens, mini);
	
	// 3. Parser: Cria a lista de comandos
	mini->cmd_list = parser(tokens);
	free_tokens(tokens);
	
	if (!mini->cmd_list)
		return ;
	
	// 4. Executor: Executa os comandos
	mini->last_exit_status = execute_cmd_list(mini->cmd_list, mini);
	
	// 5. Cleanup
	free_cmd_list(mini->cmd_list);
	mini->cmd_list = NULL;
}

/**
 * @brief Loop principal do minishell (REPL)
 * 
 * @param mini Estrutura principal do minishell
 */
static void	repl_loop(t_mini *mini)
{
	char	*line;

	while (mini->running)
	{
		line = readline("minishell$ ");
		
		// Ctrl-D (EOF)
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		
		// Linha vazia
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		
		// Adiciona ao histórico
		add_history(line);
		
		// Processa a linha
		process_line(line, mini);
		
		free(line);
	}
}

/**
 * @brief Limpa todos os recursos alocados
 * 
 * @param mini Estrutura principal do minishell
 */
static void	cleanup_minishell(t_mini *mini)
{
	if (mini->cmd_list)
		free_cmd_list(mini->cmd_list);
	if (mini->env)
		free_env(mini->env);
}

/**
 * @brief Entry point do minishell
 * 
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * @param envp Array de variáveis de ambiente
 * @return Exit status
 */
int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	
	// Setup
	if (init_minishell(&mini, envp) == ERROR)
	{
		handle_error(ERR_MALLOC, NULL, NULL);
		return (EXIT_FAILURE);
	}
	
	// Configura os sinais (Ctrl-C, Ctrl-\, Ctrl-D)
	setup_signals();
	
	// Loop principal
	repl_loop(&mini);
	
	// Cleanup
	cleanup_minishell(&mini);
	
	return (mini.last_exit_status);
}
