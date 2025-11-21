/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:25:34 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Executes a built-in shell command if the command name matches a built-in.
 * 
 * @param argv Array of strings representing the command and its arguments.
 * @param shell Pointer to the shell state struct.
 * @return The exit status of the built-in command, or 1 if not a built-in.
 */
int	exec_builtin(struct s_execcmd *e, t_shell *shell)
{
	if (!e->argv[0])
		return (1);
	if (ft_strcmp(e->argv[0], "cd") == 0)
		return (builtin_cd(shell, e->argv));
	if (ft_strcmp(e->argv[0], "exit") == 0)
		return (builtin_exit(shell, e));
	if (ft_strcmp(e->argv[0], "echo") == 0)
		return (builtin_echo(e->argv));
	if (ft_strcmp(e->argv[0], "pwd") == 0)
		return (builtin_pwd(shell));
	if (ft_strcmp(e->argv[0], "env") == 0)
		return (builtin_env(shell));
	if (ft_strcmp(e->argv[0], "export") == 0)
		return (builtin_export(shell, e->argv));
	if (ft_strcmp(e->argv[0], "unset") == 0)
		return (builtin_unset(shell, e->argv));
	return (1);
}
