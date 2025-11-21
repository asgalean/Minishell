/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:17:26 by asgalean          #+#    #+#             */
/*   Updated: 2025/09/17 18:56:25 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Signal handler for SIGINT (Ctrl-C) in interactive mode.
 * Clears the current input line and redisplays the prompt.
 *
 * @param sig The signal number (unused).
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * Restores default signal behavior for child processes before command execution.
 */
void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * Ignores SIGINT and SIGQUIT signals in the shell process while idle or waiting.
 */
void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * Sets up signal handlers for the interactive shell prompt.
 * Installs a custom handler for SIGINT and ignores SIGQUIT.
 */
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief handles SIGINT SIGQUIT in heredoc
 * 
 * @param sig 
 */
void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		interrumped_flag(1);
		rl_done = 1;
	}
}
