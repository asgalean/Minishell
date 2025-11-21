/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:46:39 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Recursively traverses and executes the abstract syntax tree (AST) of shell 
 * commands.
 *
 * This function dispatches execution logic based on the type of command node:
 * - REDIR: Executes a redirection command.
 * - EXEC: Executes a simple command (external or built-in).
 * - PIPE: Executes a pipeline of commands.
 * - AND / OR: Implements conditional execution ("&&" / "||") by evaluating 
 * the exit status of the left command.
 * - LIST: Executes left and right commands sequentially (";").
 *
 * For AND, OR, and LIST, the function recursively evaluates the left subtree
 *  and, based on type and exit status,
 * possibly evaluates the right subtree.
 *
 * @param cmd   Pointer to the root of the command AST.
 * @param shell Pointer to the shell state/context.
 * @return      The exit status of the last executed command.
 */
int	exec_tree(struct s_command *cmd, t_shell *shell)
{
	int					st;
	struct s_listcmd	*lcmd;

	if (!cmd)
		return (shell->exit_status);
	if (cmd->type == REDIR)
		return (exec_redir((struct s_redircmd *)cmd, shell));
	if (cmd->type == EXEC)
		return (exec_simple((struct s_execcmd *)cmd, shell));
	if (cmd->type == PIPE)
		return (exec_pipe((struct s_pipecmd *)cmd, shell));
	if (cmd->type == HEREDOC)
		return (exec_heredoc((struct s_heredoccmd *)cmd, shell));
	if (cmd->type == AND || cmd->type == OR || cmd->type == LIST)
	{
		lcmd = (struct s_listcmd *)cmd;
		st = exec_tree(lcmd->left, shell);
		if ((cmd->type == AND && st == 0) || (cmd->type == OR && st != 0))
			st = exec_tree(lcmd->right, shell);
		else if (cmd->type == LIST)
			st = exec_tree(lcmd->right, shell);
		return (st);
	}
	return (shell->exit_status);
}
