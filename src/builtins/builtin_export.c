/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_indentifier(char *str)
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

int	export_one_arg(char *arg, t_mini *mini)
{
	char	*equal;
	char	*key;
	int		ret;

	if (!is_valid_indentifier(arg))
	{
		handle_error(ERR_NOT_VALID_ID, "export", arg);
		return (1);
	}
	equal = ft_strchr(arg, '=');
	if (!eq)
	{
		if (!get_env_value(mini->env, arg))
			set_env_value(&mini->env, arg, NULL);
		return (SUCCESS);
	}
	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return (ERROR);
	ret = set_env_value(&mini->env, key, eq + 1);
	free(key);
	return (ret);
}


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
				tmp = j->next->key;
				j->key = j->next->key;
				j->next->key = tmp;
			
				tmp = j->next->value;
				j->value = j->next->value;
				j->next->value = tmp;
			}
			j = j->next;
		}
		i = i->next;
	}
}

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
