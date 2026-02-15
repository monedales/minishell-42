/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// TODO (Pessoa B): Implementar pwd
int	builtin_pwd(void)
{
	char	path_cwd[4096];

	if (getcwd(path_cwd, 4096) != NULL)
	{
		ft_putstr_fd(path_cwd, 1);
		write(1, "\n", 1);
	}
	else
	{
		perror("error pwd: path not found");
		return (1);
	}
	return (0);
}
