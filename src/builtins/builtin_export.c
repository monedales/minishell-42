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


// TODO (Pessoa B): Implementar export
// Usa set_env_value() da Pessoa A
void	sort_env(t_env *env)
{
	t_env	*i;
	t_env	*j;
	char	*temp;

	i = env;
	while (i)
	{
		j = env;
		while (j && j->next)
		{
			if (ft_strncmp(j->key, j->next->key) > 0)
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

int	builtin_export(char **args, t_mini *mini)
{

	//int	set_env_value(t_env **env, const char *key, const char *value)
	// printa a env em ordem alfabetica
	int		i;
	t_env	*env;

	i = 0;
	env = mini->env;
	if (args[1] == NULL)
	{
		env = sort_env(env);
		print_env(env);
	}
	else 
	{
		while (args[i])
		{
			split_env_string()	// TODO
			set_env_value()		// TODO
		}
	}
	return (0);
}
