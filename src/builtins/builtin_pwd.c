/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:43:03 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Built-in implementation of the pwd command.
 *
 * Retrieves the current working directory via getcwd and prints
 * it to stdout followed by a newline.
 * If getcwd fails (e.g. the current directory was deleted), prints
 * an error to stderr and returns 1.
 *
 * @return 0 on success, 1 if getcwd fails
 */
int	builtin_pwd(void)
{
	char	path_cwd[4096];

	if (getcwd(path_cwd, 4096) != NULL)
	{
		ft_putstr_fd(path_cwd, 1);
		write(1, "\n", 1);
		return (0);
	}
	handle_error(ERR_NO_FILE, "pwd", NULL);
	return (1);
}
