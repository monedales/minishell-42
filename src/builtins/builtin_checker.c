/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strncmp(command, "echo", 5) != 0)
		return (0);	
	if (ft_strncmp(command, "cd", 3) != 0)
		return (0);	
	if (ft_strncmp(command, "pwd", 4) != 0)
		return (0);	
	if (ft_strncmp(command, "export", 7) != 0)
		return (0);	
	if (ft_strncmp(command, "unset", 6) != 0)
		return (0);	
	if (ft_strncmp(command, "env", 4) != 0)
		return (0);	
	if (ft_strncmp(command, "exit", 6) != 0)
		return (0);	
	return (1);
}

int	execute_builtin(t_cmd *cmd, t_mini *mini)
{
	if (ft_strncmp(command, "cd", 3) != 0)
		builtin_cd(cmd_list->args, mini);	
	if (ft_strncmp(command, "echo", 5) != 0)
		builtin_echo(cmd_list->args);
	if (ft_strncmp(command, "env", 4) != 0)
		builtin_env(mini);		
	if (ft_strncmp(command, "exit", 6) != 0)
		builtin_exit(cmd_list->args, mini);	
	if (ft_strncmp(command, "export", 7) != 0)
		builtin_export(cmd_list->args, mini);		
	if (ft_strncmp(command, "pwd", 4) != 0)
		builtin_pwd();		
	if (ft_strncmp(command, "unset", 6) != 0)
		builtin_unset(cmd_list->args, mini);		
	return (0);
}
