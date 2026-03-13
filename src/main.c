/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/03/12 21:14:48 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief Initializes the main minishell state.
 *
 * Builds the internal environment list from `envp` and sets
 * default runtime fields used by the REPL loop.
 *
 * @param mini Pointer to global shell state.
 * @param envp Environment array received by `main`.
 * @return `SUCCESS` on init success, `ERROR` on allocation failure.
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
 * @brief Processes one full command line.
 *
 * Runs the complete pipeline for one input line:
 * lexer -> expander -> parser -> executor, then frees temporary
 * parsing/execution structures.
 *
 * @param line Input line returned by `readline`.
 * @param mini Pointer to global shell state.
 */
static void	process_line(char *line, t_mini *mini)
{
	t_token	*tokens;

	tokens = lexer(line);
	if (!tokens)
		return ;
	expand_tokens(tokens, mini);
	mini->cmd_list = parser(tokens);
	free_tokens(tokens);
	if (!mini->cmd_list)
		return ;
	mini->last_exit_status = execute_cmd_list(mini->cmd_list, mini);
	free_cmd_list(mini->cmd_list);
	mini->cmd_list = NULL;
}

/**
 * @brief Interactive REPL loop.
 *
 * Reads user input, handles Ctrl-D/Ctrl-C behavior, stores
 * non-empty commands in history, and dispatches execution while
 * `mini->running` remains true.
 *
 * @param mini Pointer to global shell state.
 */
static void	repl_loop(t_mini *mini)
{
	char	*line;

	while (mini->running)
	{
		line = readline("minishell$ ");
		if (!line)
			return ((void)printf("exit\n"));
		if (g_signal == 130)
		{
			mini->last_exit_status = 130;
			g_signal = 0;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line(line, mini);
		free(line);
	}
}

/**
 * @brief Releases all global resources before exit.
 *
 * Frees any remaining command list, environment list, and readline
 * history allocated during shell execution.
 *
 * @param mini Pointer to global shell state.
 */
static void	cleanup_minishell(t_mini *mini)
{
	if (mini->cmd_list)
		free_cmd_list(mini->cmd_list);
	if (mini->env)
		free_env(mini->env);
	clear_history();
}

/**
 * @brief Program entry point.
 *
 * Initializes shell state, configures signal handlers, runs the REPL,
 * and performs final cleanup before returning the last shell status.
 *
 * @param argc Number of CLI arguments (unused).
 * @param argv CLI argument vector (unused).
 * @param envp Environment array.
 * @return Shell exit status (`mini.last_exit_status`).
 */
int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	if (init_minishell(&mini, envp) == ERROR)
	{
		handle_error(ERR_MALLOC, NULL, NULL);
		return (EXIT_FAILURE);
	}
	setup_signals();
	repl_loop(&mini);
	cleanup_minishell(&mini);
	return (mini.last_exit_status);
}
