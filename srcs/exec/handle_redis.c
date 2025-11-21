/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:00:35 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/08/31 21:15:07 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Creates a redirection command node
 * @param subcmd The command to wrap
 * @param file The file for redirection
 * @param op The redirection operator
 * @return Pointer to the created redirection command
 */
static struct s_redircmd	*redis_cmd(struct s_command *subcmd, char *file,
	char *op)
{
	struct s_redircmd	*r;

	r = ft_calloc(1, sizeof(*r));
	if (!r)
		return (NULL);
	r->type = REDIR;
	r->cmd = subcmd;
	r->file = ft_strdup(file);
	if (!ft_strcmp(op, ">"))
		r->mode = O_WRONLY | O_CREAT | O_TRUNC;
	else if (!ft_strcmp(op, ">>"))
		r->mode = O_WRONLY | O_CREAT | O_APPEND;
	else
		r->mode = O_RDONLY;
	if (ft_strcmp(op, "<") == 0)
		r->fd = 0;
	else
		r->fd = 1;
	return (r);
}

/**
 * Checks if a token is a redirection operator
 * @param token Token to check
 * @return 1 if is redirection operator, 0 otherwise
 */
int	is_redirection_operator(char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"));
}

/**
 * Handles file redirection parsing (>, >>, <)
 * @param subcmd Current command
 * @param tokens Pointer to tokens array
 * @param op Redirection operator
 * @param shell Shell context
 * @return Updated command or NULL on error
 */
struct s_command	*handle_file_redirection(struct s_command *subcmd,
	char ***tokens, char *op, t_shell *shell)
{
	char				*file;
	struct s_redircmd	*r;

	file = *(*tokens)++;
	if (subcmd->type == 2 && !file)
	{
		print_perror(shell, ERR_NO_DIR, NULL);
		free_command_tree(subcmd);
		return (NULL);
	}
	if (is_redirection_operator(file) || !ft_strcmp(file, "|")
		|| !ft_strcmp(file, ";") || !ft_strcmp(file, "&&")
		|| !ft_strcmp(file, "||"))
	{
		print_perror(shell, ERR_SYNTAX, NULL);
		free_command_tree(subcmd);
		return (NULL);
	}
	r = redis_cmd(subcmd, file, op);
	if (!r)
	{
		free_command_tree(subcmd);
		return (NULL);
	}
	return ((struct s_command *)r);
}
