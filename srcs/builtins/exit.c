/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:36:31 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/23 20:25:19 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_frees(t_shell *shell, struct s_command *e)
{
	free_command_tree(e);
	free_tokens(shell->tokens);
	free_shell(shell);
}

/**
 * Implements the `exit` built-in command.
 * - If called without arguments, exits with the last shell exit status.
 * - If a numeric argument is given, exits with that code (lowest 8 bits).
 * - If a non-numeric arg is given, prints an error and exits with code 255.
 * - If more than one arg is given, prints an error and doesn't exit (returns 1)
 * Always prints "exit" before exiting or returning.
 */
int	builtin_exit(t_shell *shell, struct s_execcmd *e)
{
	int	code;
	int	exit_status;

	if (e->argv[1])
	{
		if (!ft_isnumeric(e->argv[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(e->argv[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit(255);
		}
		if (e->argv[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		code = ft_atoi(e->argv[1]);
		handle_exit_frees(shell, (struct s_command *)e);
		exit(code & 0xFF);
	}
	exit_status = shell->exit_status;
	handle_exit_frees(shell, (struct s_command *)e);
	exit(exit_status);
}
