/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/14 14:29:42 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Returns the error message string for a given error code.
 *
 * Uses a static array of messages indexed by the t_error enum.
 * Each message follows bash's format conventions.
 *
 * @param error The error code from the t_error enum.
 * @return The corresponding error message string, or NULL if invalid.
 */
static const char	*get_error_msg(t_error error)
{
	static const char	*messages[] = {
		NULL,
		"syntax error near unexpected token ",
		"command not found",
		"No such file or directory",
		"Permission denied",
		"memory allocation failed",
		"too many arguments",
		"numeric argument required",
		"not a valid identifier",
		"HOME not set",
		"OLDPWD not set"
	};

	if (error <= ERR_NONE || error > ERR_OLDPWD_NOT_SET)
		return (NULL);
	return (messages[error]);
}

/**
 * @brief Displays an error message following bash's format.
 *
 * Format: minishell: [cmd]: [detail]: message
 * If cmd or detail are NULL, their segment is skipped.
 *
 * Examples:
 *   handle_error(ERR_NO_FILE, "cd", "foo")
 *     -> minishell: cd: foo: No such file or directory
 *   handle_error(ERR_CMD_NOT_FOUND, NULL, "foo")
 *     -> minishell: foo: command not found
 *   handle_error(ERR_SYNTAX, NULL, "'|'")
 *     -> minishell: syntax error near unexpected token '|'
 *
 * @param error The error code from the t_error enum.
 * @param cmd The command name (e.g. "cd", "exit"), or NULL.
 * @param detail Extra context (e.g. filename, token), or NULL.
 * @return Always returns 1 to indicate an error condition.
 */
int	handle_error(t_error error, char *cmd, char *detail)
{
	const char	*msg;

	msg = get_error_msg(error);
	if (!msg)
		return (1);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (detail && error != ERR_SYNTAX)
	{
		ft_putstr_fd(detail, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	if (detail && error == ERR_SYNTAX)
		ft_putstr_fd(detail, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

/**
 * @brief Displays an error message and exits with the given code.
 *
 * Calls handle_error to print the message, then exits the process.
 * Used for fatal errors that cannot be recovered from.
 *
 * @param error The error code from the t_error enum.
 * @param cmd The command name, or NULL.
 * @param detail Extra context, or NULL.
 * @param code The exit status code.
 */
void	exit_error(t_error error, char *cmd, char *detail, int code)
{
	handle_error(error, cmd, detail);
	exit(code);
}
