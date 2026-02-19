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




#include "../../include/minishell.h"



int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (1);	
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (1);	
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (1);	
	if (ft_strncmp(cmd, "export", 6) == 0)
		return (1);	
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (1);	
	if (ft_strncmp(cmd, "env", 3) == 0)
		return (1);	
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);	
	return (0);
}

int	execute_builtin(t_cmd *cmd_list, t_mini *mini)
{
    char *cmd_name;

    cmd_name = cmd_list->args[0];
	if (ft_strncmp(cmd_name, "cd", 2) == 0)
		return (builtin_cd(cmd_list->args, mini));	
	if (ft_strncmp(cmd_name, "echo", 4) == 0)
		return (builtin_echo(cmd_list->args));
	if (ft_strncmp(cmd_name, "env", 3) == 0)
		return (builtin_env(mini));		
	if (ft_strncmp(cmd_name, "exit", 5) == 0)
		return (builtin_exit(cmd_list->args, mini));	
	if (ft_strncmp(cmd_name, "export", 6) == 0)
		return (builtin_export(cmd_list->args, mini));		
	if (ft_strncmp(cmd_name, "pwd", 3) == 0)
		return (builtin_pwd());		
	if (ft_strncmp(cmd_name, "unset", 5) == 0)
		return (builtin_unset(cmd_list->args, mini));		
	return (0);
}
