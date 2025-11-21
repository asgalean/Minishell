/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_double.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:43:19 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Handles escaped characters within double quotes.
 * Appends the unescaped character to the buffer, advances the input pointer,
 * and returns the updated buffer index.
 *
 * @param p Pointer to the input string pointer.
 * @param buf The buffer to append the unescaped character to.
 * @param i The current buffer index.
 * @return The updated buffer index.
 */
static int	handle_dquote_escape(char **p, char *buf, int i)
{
	(*p)++;
	if (**p)
		buf[i++] = *(*p)++;
	return (i);
}

/**
 * Expands the shell's process ID (`$$`) within double quotes 
 * and appends it to the buffer.
 * Advances the input pointer and returns the updated buffer index.
 *
 * @param p Pointer to the input string pointer.
 * @param buf The buffer to append the PID to.
 * @param i The current buffer index.
 * @param shell Pointer to shell state/context.
 * @return The updated buffer index.
 */
static int	handle_dquote_pid(char **p, char *buf, int i, t_shell *shell)
{
	char	*exp;

	exp = ft_itoa(shell->pid);
	if (exp)
	{
		i += ft_strlcpy(&buf[i], exp, 1024 - i);
		free(exp);
	}
	(*p) += 2;
	return (i);
}

/**
 * Expands a var (`$VAR`) within double quotes and appends its value to the buf.
 * Advances the input pointer and returns the updated buffer index.
 *
 * @param p Pointer to the input string pointer.
 * @param buf The buffer to append the variable value to.
 * @param i The current buffer index.
 * @param shell Pointer to shell state/context.
 * @return The updated buffer index.
 */
static int	handle_dquote_var(char **p, char *buf, int i, t_shell *shell)
{
	char	*exp;

	(*p)++;
	exp = expand_var(p, shell);
	if (exp)
	{
		i += ft_strlcpy(&buf[i], exp, 1024 - i);
		free(exp);
	}
	return (i);
}

/**
 * Reads and processes a double-quoted string from the input pointer.
 * Handles escape sequences, variable expansions, and process ID expansions 
 * within the double quotes.
 * Returns a dynamically allocated string containing the processed content.
 *
 * @param p Pointer to the input string pointer (will be advanced).
 * @param shell Pointer to shell state/context.
 * @return Newly allocated string with the expanded content.
 */
char	*read_double(char **p, t_shell *shell)
{
	char	buf[1024];
	int		i;

	i = 0;
	(*p)++;
	while (**p && **p != '"')
	{
		if (**p == '\\' && *(*p + 1))
			i = handle_dquote_escape(p, buf, i);
		else if (**p == '$' && *(*p + 1) == '$')
			i = handle_dquote_pid(p, buf, i, shell);
		else if (**p == '$')
			i = handle_dquote_var(p, buf, i, shell);
		else
			buf[i++] = *(*p)++;
	}
	if (**p != '"')
	{
		print_perror(shell, ERR_SYNTAX, "quote");
		shell->quote_error = 1;
		return (NULL);
	}
	(*p)++;
	buf[i] = '\0';
	return (ft_strdup(buf));
}
