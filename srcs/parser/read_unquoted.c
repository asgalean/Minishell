/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_unquoted.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:37:27 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if a character is a metacharacter that delimits tokens
 * (e.g., space, `<`, `>`, `|`, `;`).
 *
 * @param c The character to check.
 * @return 1 if the character is a metacharacter, 0 otherwise.
 */
static int	is_meta(char c)
{
	return (c == ' ' || c == '<' || c == '>' || c == '|' || c == ';');
}

/**
 * Expands the process ID (`$$`) in unquoted context and appends it to the buf.
 * Advances the input pointer and returns the updated buffer index.
 *
 * @param p Pointer to the input string pointer.
 * @param buf Buffer to append the PID to.
 * @param i The current buffer index.
 * @param shell Pointer to shell state/context.
 * @return The updated buffer index.
 */
static int	handle_pid_expansion(char **p, char *buf, int i, t_shell *shell)
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
 * Expands a var (`$VAR`) in unquoted context and appends its value to the buf.
 * Advances the input pointer and returns the updated buffer index.
 *
 * @param p Pointer to the input string pointer.
 * @param buf Buffer to append the variable value to.
 * @param i The current buffer index.
 * @param shell Pointer to shell state/context.
 * @return The updated buffer index.
 */
static int	handle_var_expansion(char **p, char *buf, int i, t_shell *shell)
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
 * @brief 
 * 
 * @param p 
 * @param buf 
 * @param i 
 * @param shell 
 * @return int 
 */
static int	handle_quotes(char **p, char *buf, int i, t_shell *shell)
{
	int	result;

	if (**p == '\'')
	{
		result = handle_single_quote(p, buf, i, shell);
		if (result == -1)
			return (-1);
		return (result);
	}
	else if (**p == '"')
	{
		result = handle_double_quote(p, buf, i, shell);
		if (result == -1)
			return (-1);
		return (result);
	}
	return (i);
}

/**
 * Reads and processes an unquoted string from the input pointer.
 * Handles escape sequences, single-quoted strings, double-quoted strings,
 * and variable expansions.
 * Returns a dynamically allocated string containing the processed content.
 *
 * @param p Pointer to the input string pointer (will be advanced).
 * @param shell Pointer to shell state/context.
 * @return Newly allocated string with processed content.
 */
char	*read_unquoted(char **p, t_shell *shell)
{
	char	buf[1024];
	int		i;
	int		result;

	i = 0;
	while (**p && !is_meta(**p))
	{
		if (**p == '\\' && *(*p + 1))
			i = handle_escape(p, buf, i);
		else if (**p == '\'' || **p == '"')
		{
			result = handle_quotes(p, buf, i, shell);
			if (result == -1)
				return (NULL);
			i = result;
		}
		else if (**p == '$' && *(*p + 1) == '$')
			i = handle_pid_expansion(p, buf, i, shell);
		else if (**p == '$')
			i = handle_var_expansion(p, buf, i, shell);
		else
			buf[i++] = *(*p)++;
	}
	buf[i] = '\0';
	return (ft_strdup(buf));
}
