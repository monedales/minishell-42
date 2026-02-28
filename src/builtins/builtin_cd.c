/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_pwd(char *old_pwd, t_mini *mini)
{
	char	buf[4096];

	set_env_value(&mini->env, "OLD_PWD", old_pwd);
	if (getcwd(buf, sizeof(buf)))
		set_env_value(&mini->env, "PWD", buf);
}

int	go_to_path(char *old_pwd, char *path)
{
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
	if (chdir(path) != 0)
	{
		handle_error(ERR_NO_FILE, "cd", NULL);
		return (1);
	}
	return (0);
}

int	builtin_cd(char **args, t_mini *mini)
{
	char	old_pwd[4096];
	char	*path;
	int		ret;

	if (args[1] && args[2])
	{
		handle_error(ERR_TOO_MANY_ARGS, "cd", NULL);
		return (1);
	}
	if (!args[1])
	{
		path = get_env_value(mini->env, "HOME");
		if (!path)
		{
			handle_error(ERR_HOME_NOT_SET, "cd", NULL);
			return (1);
		}
	}
	else
		path = args[1];
	ret = go_to_path(old_pwd, path);
	update_pwd(old_pwd, mini);
	return (ret);
}
