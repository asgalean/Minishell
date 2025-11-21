/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:26:41 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/05/05 13:01:22 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Extracts a var name (alphanumeric and underscores) from the input pointer.
 * Advances the pointer and returns a dynamically allocated str of the var name
 *
 * @param p Pointer to the input string pointer.
 * @return Newly allocated string with the variable name.
 */
static char	*extract_name(char **p)
{
	char	buf[256];
	int		i;

	i = 0;
	while (ft_isalnum(**p) || **p == '_')
		buf[i++] = *(*p)++;
	buf[i] = '\0';
	return (ft_strdup(buf));
}

/**
 * Expands a variable or special token (`$?`) into its corresponding value.
 * Supports both `${VAR}` and simple `$VAR` notations.
 * Returns a dynamically allocated string containing the expanded value.
 *
 * @param p Pointer to the input string pointer (advanced during parsing).
 * @param shell Pointer to shell state/context.
 * @return Newly allocated string with the expanded value.
 */
char	*expand_var(char **p, t_shell *shell)
{
	char	*name;
	char	*value;

	if (**p == '?')
	{
		(*p)++;
		return (ft_itoa(shell->exit_status));
	}
	if (**p == '{')
	{
		(*p)++;
		name = extract_name(p);
		if (**p == '}')
			(*p)++;
	}
	else
		name = extract_name(p);
	value = get_env_value(shell, name);
	free(name);
	if (value)
		return (ft_strdup(value));
	return ("");
}
