/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
			break;
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putstr_fd("\n", pipefd[1]);
		free(line);
	}
	close(pipefd[0]);
	return (SUCCESS);
}

/**
 * @brief Configura os redirecionamentos para um comando
 * 
 * TODO (Pessoa B): Implementar
 * - Percorrer lista de redirecionamentos
 * - Para cada tipo:
 *   - REDIR_IN (<): open arquivo, dup2 para stdin
 *   - REDIR_OUT (>): open/create arquivo, dup2 para stdout
 *   - REDIR_APPEND (>>): open/create em modo append, dup2 para stdout
 *   - REDIR_HEREDOC (<<): ler até delimiter, criar pipe temporário
 * - Fechar FDs originais após dup2
 * 
 * @param redirs Lista de redirecionamentos
 * @return SUCCESS ou ERROR
 */
int	setup_redirections(t_redir *redirs)
{
	while(redirs)
	{
		if (redirs->type == TKN_REDIR_IN)
		{
			if (redir_in(redirs->file) == ERROR)
				return (ERROR);
		}
		else if (redirs->type == TKN_REDIR_OUT)
		{
			if (redir_out(redirs->file, FALSE) == ERROR)
				return (ERROR);
		}
				else if (redirs->type == TKN_REDIR_APPEND)
		{
			if (redir_out(redirs->file, TRUE) == ERROR)
				return (ERROR);
		}
		else if (redirs->type == TKN_REDIR_HEREDOC)
		{
			if (redir_heredoc(redirs->file) == ERROR)
				return (ERROR);
		}
		redirs = redirs->next;
	}
	return (SUCCESS);
	}

