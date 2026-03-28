/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/26 21:26:19 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Implements heredoc (<<).
 * 
 * Creates a pipe, reads lines from stdin until the delimiter is matched,
 * writing each line into the pipe's write end. The read end is then
 * duplicated onto stdin so the command reads from the accumulated input.
 * Does not update shell history (readline used directly without add_history).
 * 
 * @param delimiter The stop word (e.g. "EOF")
 * @return SUCCESS or ERROR
 */
int	redir_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (ERROR);
	while (1)
	{
		line = readline(">");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putstr_fd("\n", pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		close(pipefd[0]);
		return (ERROR);
	}
	close(pipefd[0]);
	return (SUCCESS);
}

/**
 * @brief Applies a single redirection node.
 *
 * For heredoc, restores STDIN to orig_stdin (the terminal) before
 * reading, so that multiple heredocs always read from the terminal
 * and not from a previous heredoc's pipe.
 *
 * @param redir       The redirection node to apply.
 * @param orig_stdin  The saved terminal fd (from dup at start of setup).
 * @return SUCCESS or ERROR.
 */
static int	apply_redir(t_redir *redir, int orig_stdin)
{
	if (redir->type == TKN_REDIR_IN)
		return (redir_in(redir->file));
	if (redir->type == TKN_REDIR_OUT)
		return (redir_out(redir->file, FALSE));
	if (redir->type == TKN_REDIR_APPEND)
		return (redir_out(redir->file, TRUE));
	if (redir->type == TKN_REDIR_HEREDOC)
	{
		dup2(orig_stdin, STDIN_FILENO);
		return (redir_heredoc(redir->file));
	}
	return (SUCCESS);
}

/**
 * @brief Applies all redirections of a command in list order.
 *
 * Saves the original STDIN before iterating so that each heredoc
 * can restore it before calling readline — preventing a heredoc
 * from reading the pipe left by a previous heredoc.
 * Only the last heredoc's pipe remains as STDIN after the loop.
 *
 * @param redirs Head of the redirection linked list.
 * @return SUCCESS if all redirections applied, ERROR otherwise.
 */
int	setup_redirections(t_redir *redirs)
{
	int	orig_stdin;

	orig_stdin = dup(STDIN_FILENO);
	if (orig_stdin == -1)
		return (ERROR);
	while (redirs)
	{
		if (apply_redir(redirs, orig_stdin) == ERROR)
		{
			close(orig_stdin);
			return (ERROR);
		}
		redirs = redirs->next;
	}
	close(orig_stdin);
	return (SUCCESS);
}
