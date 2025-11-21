/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:52:29 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Implements the `pwd` built-in command.
 * Prints the current working directory to standard output.
 * Sets the shell's exit status to 0 on success.
 * Sets to 1 and prints an error on failure.
 * Returns the exit status.
 */
int	builtin_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		free(cwd);
		shell->exit_status = 0;
	}
	else
	{
		perror("minishell: pwd");
		shell->exit_status = 1;
	}
	return (shell->exit_status);
}
