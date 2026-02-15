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

char	*ft_strdup(const char *str)
{
	char	*new_str;
	size_t	count;
	size_t	len;

	len = ft_strlen(str) + 1;
	new_str = malloc(len * sizeof(char));
	if (!new_str)
		return (NULL);
	count = 0;
	while (str[count])
	{
		new_str[count] = str[count];
		count++;
	}
	new_str[count] = '\0';
	return (new_str);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}


void	ft_putstr_fd(char *str, int fd)
{
	size_t	len;

	len = ft_strlen(str);
	write(fd, str, len);
}

int	ft_strncmp(const char *s1, const char *s2, size_t num)
{
	unsigned char	*ps1;
	unsigned char	*ps2;
	size_t			count;

	count = 0;
	ps1 = (unsigned char *)s1;
	ps2 = (unsigned char *)s2;
	while (count < num && (ps1[count] || ps2[count]))
	{
		if (ps1[count] != ps2[count])
			return (ps1[count] - ps2[count]);
		count++;
	}
	return (0);
}



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
