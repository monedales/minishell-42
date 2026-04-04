/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/04/04 15:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	has_quotes(char *str)
{
	while (*str)
	{
		if (*str == '\'' || *str == '"')
			return (TRUE);
		str++;
	}
	return (FALSE);
}

static int	process_heredoc_redir(t_redir *redir, t_mini *mini)
{
	int		expand;
	char	*clean;

	expand = !has_quotes(redir->file);
	clean = remove_quotes(redir->file);
	free(redir->file);
	redir->file = clean;
	redir->fd = collect_heredoc(redir->file, expand, mini);
	if (redir->fd == -1)
		return (ERROR);
	return (SUCCESS);
}

/**
 * @brief Collects all heredoc inputs for a command list in the parent process.
 *
 * Walks every command's redir list and pre-fills each TKN_REDIR_HEREDOC node
 * with a pipe read fd via collect_heredoc(). This must run before any fork so
 * that readline never executes inside a child process — preventing the
 * 'still reachable' leak that occurs when Ctrl+C kills a child mid-readline.
 *
 * If Ctrl+C is received during collection, closes already-collected fds and
 * returns ERROR so execute_cmd_list can propagate the 130 exit status.
 *
 * @param cmd_list Linked list of commands to process.
 * @param mini     Main shell structure (used to update last_exit_status).
 * @return SUCCESS if all heredocs collected, ERROR on signal or pipe failure.
 */
int	collect_all_heredocs(t_cmd *cmd_list, t_mini *mini)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TKN_REDIR_HEREDOC)
			{
				if (process_heredoc_redir(redir, mini) == ERROR)
					return (ERROR);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}
