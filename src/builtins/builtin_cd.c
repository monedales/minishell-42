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

// TODO (Pessoa B): Implementar cd
int	builtin_cd(char **args, t_mini *mini)
{
	char	*path;

	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		//handle_error();
		return (1);
	}
	return (0);
}
