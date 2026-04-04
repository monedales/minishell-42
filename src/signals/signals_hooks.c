/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:45:55 by mona              #+#    #+#             */
/*   Updated: 2026/04/04 15:12:14 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Readline event hook for heredoc input.
 *
 * Called periodically by readline while waiting for input during a
 * heredoc. When SIGINT has been received (g_signal == 130), sets
 * rl_done to 1 so readline returns immediately, allowing the heredoc
 * to be interrupted cleanly.
 *
 * @return Always returns 0.
 */
int	heredoc_event_hook(void)
{
	if (g_signal == 130)
		rl_done = 1;
	return (0);
}

/**
 * @brief Readline event hook for interactive prompt mode.
 *
 * Called periodically by readline while waiting for user input at the
 * prompt. When SIGINT has been received (g_signal == 130), sets
 * rl_done to 1 so readline returns immediately, allowing the prompt
 * loop to handle the interruption and redisplay a fresh prompt.
 *
 * @return Always returns 0.
 */
int	prompt_event_hook(void)
{
	if (g_signal == 130)
		rl_done = 1;
	return (0);
}

/**
 * @brief Handler for SIGINT (Ctrl+C) during heredoc input.
 *
 * Sets g_signal to 130, prints a newline, clears the current readline
 * buffer and sets rl_done to 1 so readline returns immediately.
 * This allows the heredoc loop to detect the interruption and abort
 * cleanly without leaving partial input behind.
 *
 * @param sig Signal number (unused).
 */
void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_done = 1;
}

/**
 * @brief Configures signal handlers for heredoc input mode.
 *
 * Sets up SIGINT to call handle_sigint_heredoc, which marks the
 * interruption via g_signal and forces readline to return. SIGQUIT
 * is ignored (SIG_IGN), matching bash behavior inside heredocs.
 * Also installs heredoc_event_hook so readline polls g_signal and
 * exits promptly when Ctrl+C is pressed.
 */
void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
	rl_event_hook = heredoc_event_hook;
}
