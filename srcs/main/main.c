/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:59:40 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Entry point for the minishell program.
 * Initializes the shell environment, sets up signal handling,
 * runs the interactive shell loop, and performs cleanup on exit.
 *
 * @param argc Argument count (expected: 1).
 * @param argv Argument vector (unused).
 * @param envp Environment variables (NULL-terminated array).
 * @return 0 on success, 1 on error.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	if (argc != 1)
	{
		printf("minishell: no arguments expected\n");
		return (1);
	}
	setup_signals();
	shell = setup_shell(envp);
	if (!shell)
		return (1);
	set_shell_level(shell);
	run_shell_loop(shell);
	free_shell(shell);
	cleanup();
	return (0);
}
