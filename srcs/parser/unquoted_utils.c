/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:49:00 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/08/31 21:09:33 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Handles escaped characters in unquoted strings and appends the
 * unescaped character to the buffer. Advances the input pointer and
 * returns the updated buffer index.
 *
 * @param p Pointer to the input string pointer.
 * @param buf Buffer to append the unescaped character to.
 * @param i The current buffer index.
 * @return The updated buffer index.
 */
int	handle_escape(char **p, char *buf, int i)
{
	(*p)++;
	if (**p)
		buf[i++] = *(*p)++;
	return (i);
}

/**
 * Reads and processes a single-quoted string from the input pointer.
 * Appends the content between the single quotes to the buffer.
 *
 * @param p Pointer to the input string pointer.
 * @param buf Buffer to append the single-quoted content to.
 * @param i The current buffer index.
 * @return The updated buffer index.
 */
int	handle_single_quote(char **p, char *buf, int i, t_shell *shell)
{
	char	*sq;

	sq = read_single(p, shell);
	if (!sq)
	{
		if (shell->quote_error)
			return (-1);
		return (i);
	}
	i += ft_strlcpy(&buf[i], sq, 1024 - i);
	free(sq);
	return (i);
}

/**
 * Reads and processes a double-quoted string from the input pointer in
 * unquoted context. Handles escape sequences and variable expansions.
 * Appends the processed content to the buffer.
 *
 * @param p Pointer to the input string pointer.
 * @param buf Buffer to append the double-quoted content to.
 * @param i The current buffer index.
 * @param shell Pointer to shell state/context.
 * @return The updated buffer index.
 */
int	handle_double_quote(char **p, char *buf, int i, t_shell *shell)
{
	char	*dq;

	dq = read_double(p, shell);
	if (!dq)
	{
		if (shell->quote_error)
			return (-1);
		return (i);
	}
	i += ft_strlcpy(&buf[i], dq, 1024 - i);
	free(dq);
	return (i);
}

/**
 * Creates a single redirection node (helper function)
 */
struct s_command	*handle_file_redirection_single(struct s_command *subcmd,
	char *file, char *op)
{
	struct s_redircmd	*r;

	r = ft_calloc(1, sizeof(*r));
	if (!r)
	{
		free_command_tree(subcmd);
		return (NULL);
	}
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
	return ((struct s_command *)r);
}
