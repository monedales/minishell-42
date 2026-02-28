/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/02/19 22:08:53 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/**
 * @brief Checks if a command name is a builtin.
 * 
 * Uses ft_strncmp with strlen + 1 to ensure exact match,
 * preventing partial matches like "cdrom" matching "cd".
 * 
 * @param cmd Command name string
 * @return 1 if builtin, 0 otherwise
 */
int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/**
 * @brief Dispatches a builtin command to its implementation.
 * 
 * Matches args[0] against all known builtins and calls the
 * corresponding function. Returns 1 if the command is not found
 * (should not happen if is_builtin was checked first).
 * 
 * @param cmd_list The command node containing args
 * @param mini     The main shell structure
 * @return The return value of the executed builtin
 */
int	execute_builtin(t_cmd *cmd_list, t_mini *mini)
{
	char	*cmd_name;

	cmd_name = cmd_list->args[0];
	if (ft_strncmp(cmd_name, "cd", 3) == 0)
		return (builtin_cd(cmd_list->args, mini));
	if (ft_strncmp(cmd_name, "echo", 5) == 0)
		return (builtin_echo(cmd_list->args));
	if (ft_strncmp(cmd_name, "env", 4) == 0)
		return (builtin_env(mini));
	if (ft_strncmp(cmd_name, "exit", 5) == 0)
		return (builtin_exit(cmd_list->args, mini));
	if (ft_strncmp(cmd_name, "export", 7) == 0)
		return (builtin_export(cmd_list->args, mini));
	if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd_name, "unset", 6) == 0)
		return (builtin_unset(cmd_list->args, mini));
	return (1);
}
