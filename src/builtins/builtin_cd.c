/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:33:48 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Updates the PWD and OLDPWD environment variables
 *
 * Sets OLDPWD to the path stored in old_pwd, then retrieves the current
 * working directory with getcwd and stores it in PWD.
 * Called after a successful chdir.
 *
 * @param old_pwd The previous working directory path
 * @param mini    Pointer to the main shell structure (env list)
 */
void	update_pwd(char *old_pwd, t_mini *mini)
{
	char	buf[4096];

	set_env_value(&mini->env, "OLD_PWD", old_pwd);
	if (getcwd(buf, sizeof(buf)))
		set_env_value(&mini->env, "PWD", buf);
}

/**
 * @brief Saves the current directory and changes to the given path
 *
 * Stores the current working directory in old_pwd before calling chdir.
 * If getcwd fails, old_pwd is set to an empty string.
 * If chdir fails, prints an error and returns 1.
 *
 * @param old_pwd Buffer to store the current directory before changing
 * @param path    Target directory path to change into
 * @return 0 on success, 1 on failure
 */
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

/**
 * @brief Built-in implementation of the cd command
 *
 * Handles three cases:
 * - Too many arguments: prints an error and returns 1
 * - No argument: changes to the HOME directory (errors if HOME is not set)
 * - One argument: changes to the specified path
 *
 * After a successful directory change, updates PWD and OLDPWD via
 * update_pwd.
 *
 * @param args Array of arguments (args[0] = "cd", args[1] = path or NULL)
 * @param mini Pointer to the main shell structure (env list, exit status)
 * @return 0 on success, 1 on failure
 */
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
