/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/03/26 00:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Restores stdin and stdout from saved file descriptors.
 *
 * Called after executing a builtin in the parent process to undo
 * any redirections applied by setup_redirections.
 *
 * @param in  Saved STDIN_FILENO fd
 * @param out Saved STDOUT_FILENO fd
 */
void	restore_fds(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

/**
 * @brief Redirects stdin from a file (<).
 *
 * Opens the file for reading and duplicates it onto stdin.
 *
 * @param file Path to the input file
 * @return SUCCESS or ERROR
 */
int	redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		handle_error(ERR_NO_FILE, file, NULL);
		return (ERROR);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

/**
 * @brief Redirects stdout to a file (> or >>).
 *
 * Opens the file in truncate or append mode depending on the flag,
 * then duplicates it onto stdout.
 *
 * @param file   Path to the output file
 * @param append If TRUE, opens in append mode; otherwise truncates
 * @return SUCCESS or ERROR
 */
int	redir_out(char *file, int append)
{
	int	fd;
	int	flags;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		handle_error(ERR_NO_FILE, file, NULL);
		return (ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}
