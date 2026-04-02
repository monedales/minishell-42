/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/31 00:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
 
#include "../../include/minishell.h"

/**
 * @brief Collects heredoc input from the terminal and returns a pipe read fd.
 *
 * Called in the PARENT process before any fork, so Ctrl+C is handled by
 * the parent's signal handler (not by readline's internal re-raise in a
 * child). Reads lines until the delimiter or EOF/Ctrl+C.
 * Returns -1 if a signal interrupted input (g_signal == 130) or on error.
 * The caller must close the returned fd when done.
 *
 * @param delimiter The stop word (e.g. "EOF")
 * @return Read end of the pipe containing the collected input, or -1 on error
 */
int collect_heredoc(char *delimiter)
{
    int     pipefd[2];
    char    *line;
 
    if (pipe(pipefd) == -1)
        return (-1);
    g_signal = 0;
    setup_heredoc_signals();
    while (1)	
    {
        line = readline("> ");
        if (g_signal == 130)
        {
            safe_free((void **)&line);
            break ; 
        }
        if (!line
            || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
        {
            free(line);
            break ;
        }
        ft_putstr_fd(line, pipefd[1]);
        ft_putstr_fd("\n", pipefd[1]);
        free(line);
    }
    close(pipefd[1]);
    rl_event_hook = NULL;
    setup_signals();
    if (g_signal == 130)
    {
        close(pipefd[0]);
        return (-1);
    }
    return (pipefd[0]);
}

/**
 * @brief Applies a single redirection node to the current process.
 *
 * For heredoc, uses the pre-collected fd stored in redir->fd by
 * collect_all_heredocs (called before any fork). Closes the fd after use.
 *
 * @param redir The redirection node to apply.
 * @return SUCCESS or ERROR.
 */
static int  apply_redir(t_redir *redir)
{
    if (redir->type == TKN_REDIR_IN)
        return (redir_in(redir->file));
    if (redir->type == TKN_REDIR_OUT)
        return (redir_out(redir->file, FALSE));
    if (redir->type == TKN_REDIR_APPEND)
        return (redir_out(redir->file, TRUE));
    if (redir->type == TKN_REDIR_HEREDOC)
    {
        if (redir->fd < 0)
            return (ERROR);
        if (dup2(redir->fd, STDIN_FILENO) == -1)
            return (ERROR);
        close(redir->fd);
        redir->fd = -1;
        return (SUCCESS);
    }
    return (SUCCESS);
}
 
/**
 * @brief Applies all redirections of a command in list order.
 *
 * Heredoc fds must already be pre-collected in the parent via
 * collect_all_heredocs before this is called in any child process.
 *
 * @param redirs Head of the redirection linked list.
 * @return SUCCESS if all redirections applied, ERROR otherwise.
 */
int setup_redirections(t_redir *redirs)
{
    while (redirs)
    {
        if (apply_redir(redirs) == ERROR)
            return (ERROR);
        redirs = redirs->next;
    }
    return (SUCCESS);
}

