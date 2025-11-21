/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:04:34 by asgalean          #+#    #+#             */
/*   Updated: 2025/09/23 20:24:41 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if a token is a command terminator (pipe, logical ops, etc.)
 */
static int	is_command_terminator(char *token)
{
	if (!token)
		return (1);
	return (!ft_strcmp(token, "|") || !ft_strcmp(token, "&&")
		|| !ft_strcmp(token, "||") || !ft_strcmp(token, ";"));
}

/**
 * Finds the underlying EXEC command in a chain of redirections.
 * 
 * @param cmd Command structure that might be wrapped in redirections.
 * @return Pointer to the EXEC command node.
 */
static struct s_command	*find_exec_cmd(struct s_command *cmd)
{
	struct s_command	*exec_cmd;

	exec_cmd = cmd;
	while (exec_cmd && exec_cmd->type == REDIR)
		exec_cmd = ((struct s_redircmd *)exec_cmd)->cmd;
	while (exec_cmd && exec_cmd->type == HEREDOC)
		exec_cmd = ((struct s_heredoccmd *)exec_cmd)->cmd;
	return (exec_cmd);
}

/**
 * Processes a single argument token and adds it to the command.
 * 
 * @param result Current command structure.
 * @param token The argument token to add.
 * @param arg_index Pointer to the current argument index.
 */
static void	arg_to_exec(struct s_command *result, char *token, int *arg_index)
{
	struct s_command	*exec_cmd;

	exec_cmd = find_exec_cmd(result);
	if (exec_cmd && exec_cmd->type == EXEC)
	{
		((struct s_execcmd *)exec_cmd)->argv[*arg_index] = ft_strdup(token);
		(*arg_index)++;
	}
}

/**
 * Parses an executable command and its arguments.
 * Now handles redirections that appear before, after, or mixed with arguments.
 *
 * @param tokens Pointer to pointer to the tokens array.
 * @param shell Pointer to shell state/context.
 * @return Pointer to the EXEC or REDIR command node.
 */
struct s_command	*parse_exec(char ***tokens, t_shell *shell)
{
	struct s_execcmd	*e;
	int					i;
	struct s_command	*result;

	e = ft_calloc(1, sizeof(*e));
	e->type = EXEC;
	i = 0;
	result = (struct s_command *)e;
	while (**tokens && !is_command_terminator(**tokens))
	{
		if (is_redirection_operator(**tokens))
		{
			result = parse_redirs(result, tokens, shell);
			if (!result)
				return (NULL);
		}
		else
		{
			arg_to_exec(result, **tokens, &i);
			(*tokens)++;
		}
	}
	if (find_exec_cmd(result) && find_exec_cmd(result)->type == EXEC)
		((struct s_execcmd *)find_exec_cmd(result))->argv[i] = NULL;
	return (result);
}
