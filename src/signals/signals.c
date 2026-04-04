/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/04/04 14:59:06 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

/**
 * @brief Handler for SIGINT (Ctrl+C) at the prompt.
 *
 * Sets g_signal to 130 and prints a newline so readline knows to
 * redraw the prompt on the next iteration. Does NOT call rl_redisplay()
 * here — that would cause a duplicate prompt when a child (e.g. cat)
 * is interrupted, because readline will already print the prompt when
 * it resumes after waitpid returns.
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
}

/**
 * @brief Ignores SIGINT in the parent while a child process is running.
 *
 * Called in the parent right after fork(), before waitpid().
 * This prevents handle_sigint from firing (and printing a duplicate
 * prompt via rl_redisplay) when Ctrl+C is pressed while a child runs.
 * The child itself handles SIGINT via SIG_DFL (setup_child_signals).
 * After waitpid returns, call setup_signals() to restore normal behavior.
 */
void	setup_exec_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Configures signal handlers for interactive prompt mode.
 *
 * Uses sigaction for both signals to ensure consistent behavior:
 * - SIGINT  (Ctrl+C) : calls handle_sigint — prints newline, redraws prompt
 * - SIGQUIT (Ctrl+\) : ignored (SIG_IGN) — same behavior as bash at prompt
 *
 * Also installs prompt_event_hook so readline polls g_signal and
 * returns promptly when Ctrl+C is pressed.
 *
 * Should be called at startup and after each child process finishes,
 * to restore prompt-mode behavior.
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
	rl_event_hook = prompt_event_hook;
}

/**
 * @brief Restores default signal behavior for child processes.
 *
 * Called in the child after fork(), before execve(). The child inherits
 * the parent's SIG_IGN for SIGQUIT — if we don't reset it, the external
 * command (e.g. cat) will ignore Ctrl+\ instead of printing "^\".
 * SIGINT is also reset so the child handles it normally (exits on Ctrl+C)
 * instead of running the parent's handler which touches readline state.
 *
 * Must be called in every child process (simple cmd and pipeline).
 */
void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
