/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:41:17 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks if a string is a valid environment variable identifier.
 *
 * A valid identifier must start with a letter or underscore, followed
 * by letters, digits or underscores. An optional '=' and its value
 * after it are ignored during validation.
 *
 * Examples: "HOME", "MY_VAR", "_x1" are valid.
 *           "1VAR", "-x", "" are not.
 *
 * @param str The argument string to validate (format: "KEY" or "KEY=VALUE")
 * @return TRUE if valid identifier, FALSE otherwise
 */
static int	is_valid_indentifier_export(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (FALSE);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (FALSE);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * @brief Exports a single argument into the environment.
 *
 * Validates the identifier, then handles two cases:
 * - "KEY"       → adds KEY to the env with a NULL value (if not already set)
 * - "KEY=VALUE" → sets or updates KEY with the given value
 *
 * Prints an error and returns 1 if the identifier is invalid.
 *
 * @param arg  The export argument string ("KEY" or "KEY=VALUE")
 * @param mini Pointer to the main shell structure (env list)
 * @return 0 on success, 1 on failure
 */
int	export_one_arg(char *arg, t_mini *mini)
{
	char	*equal;
	char	*key;
	int		ret;

	if (!is_valid_indentifier_export(arg))
	{
		handle_error(ERR_NOT_VALID_ID, "export", arg);
		return (1);
	}
	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		if (!get_env_value(mini->env, arg))
			set_env_value(&mini->env, arg, NULL);
		return (SUCCESS);
	}
	key = ft_substr(arg, 0, equal - arg);
	if (!key)
		return (ERROR);
	ret = set_env_value(&mini->env, key, equal + 1);
	free(key);
	return (ret);
}

/**
 * @brief Sorts the environment list alphabetically by key (bubble sort).
 *
 * Swaps the key and value pointers between adjacent nodes when they are
 * out of order. The nodes themselves are not moved — only their contents.
 * Called before printing the environment with no arguments.
 *
 * @param env Head of the environment linked list
 */
void	sort_env(t_env *env)
{
	t_env	*i;
	t_env	*j;
	char	*tmp;

	i = env;
	while (i)
	{
		j = env;
		while (j && j->next)
		{
			if (ft_strncmp(j->key, j->next->key, ft_strlen(j->key) + 1) > 0)
			{
				tmp = j->key;
				j->key = j->next->key;
				j->next->key = tmp;
				tmp = j->value;
				j->value = j->next->value;
				j->next->value = tmp;
			}
			j = j->next;
		}
		i = i->next;
	}
}

/**
 * @brief Prints all environment variables in export format.
 *
 * Prints each variable as "declare -x KEY=\"VALUE\"" or "declare -x KEY"
 * (if value is NULL) to stdout, matching bash's export output format.
 *
 * @param env Head of the environment linked list
 */
void	print_export_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			ft_printf("declare -x %s\n", env->key);
		env = env->next;
	}
}

/**
 * @brief Built-in implementation of the export command.
 *
 * Handles two cases:
 * - No arguments: sorts and prints all env variables in declare format
 * - With arguments: calls export_one_arg for each, accumulating errors
 *
 * Returns the last non-zero exit code if any argument failed,
 * or 0 if all succeeded.
 *
 * @param args NULL-terminated argument array (args[0] = "export")
 * @param mini Pointer to the main shell structure (env list)
 * @return 0 if all exports succeeded, 1 if any argument was invalid
 */
int	builtin_export(char **args, t_mini *mini)
{
	int		i;
	int		ret;

	ret = 0;
	if (!args[1])
	{
		sort_env(mini->env);
		print_export_env(mini->env);
		return (ret);
	}
	i = 1;
	while (args[i])
	{
		if (export_one_arg(args[i], mini) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
