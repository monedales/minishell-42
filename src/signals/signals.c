/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 22:16:13 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

/**
 * @brief Handler for SIGINT (Ctrl+C) at the prompt.
 *
 * Sets g_signal to 130 (128 + SIGINT=2) so the main loop can update
 * last_exit_status after readline returns. Prints a newline, clears
 * the current readline input and redraws the prompt.
 * Uses only async-signal-safe functions (write instead of printf).
 *
 * @param sig Signal number (unused)
 */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * @brief Configures signal handlers for interactive prompt mode.
 *
 * Uses sigaction for both signals to ensure consistent behavior:
 * - SIGINT  (Ctrl+C) : calls handle_sigint — prints newline, redraws prompt
 * - SIGQUIT (Ctrl+\) : ignored (SIG_IGN) — same behavior as bash at prompt
 *
 * SA_RESTART prevents syscalls like read (used by readline) from returning
 * EINTR when a signal is caught, avoiding spurious readline errors.
 *
 * Should be called at startup and after each child process finishes,
 * to restore prompt-mode behavior.
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
