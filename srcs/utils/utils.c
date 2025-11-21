/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:59:13 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Recursively frees the memory used by a command tree.
 *
 * @param cmd Pointer to the root of the command tree.
 */
void	free_command_tree(struct s_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
		free_exec_command((struct s_execcmd *)cmd);
	else if (cmd->type == REDIR)
		free_redir_command((struct s_redircmd *)cmd);
	else if (cmd->type == PIPE)
		free_pipe_command((struct s_pipecmd *)cmd);
	else if (cmd->type == HEREDOC)
		free_heredoc_command((struct s_heredoccmd *)cmd);
	else if (cmd->type == LIST)
		free_list_command((struct s_listcmd *)cmd);
}

/**
 * Cleans up readline state and history.
 * Should be called before exiting the shell.
 */
void	cleanup(void)
{
	rl_clear_history();
	rl_free_line_state();
}

/**
 * Frees all resources associated with the shell state.
 *
 * @param shell Pointer to the shell state structure.
 */
void	free_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	if (shell->user)
		free(shell->user);
	if (shell->home)
		free(shell->home);
	if (shell->pwd)
		free(shell->pwd);
	if (shell->oldpwd)
		free(shell->oldpwd);
	free(shell);
}

int	interrumped_flag(int option)
{
	static volatile sig_atomic_t	g_heredoc_interrupted;

	if (option == 1)
		g_heredoc_interrupted = 1;
	else if (option == 0)
		g_heredoc_interrupted = 0;
	return (g_heredoc_interrupted);
}
