/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:26:59 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if the given command is a shell built-in.
 * Returns 1 if it matches any built-in command, otherwise 0.
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

/**
 * Checks if the given token is a valid built-in command name.
 * Returns 1 if the token matches a built-in exactly, 0 if it is only a prefix.
 * (E.g., "cd" returns 1, "cd.." returns 0.)
 */
int	is_valid_builtin(char *token)
{
	if (ft_strncmp(token, "cd", 2) == 0 && token[2] != '\0')
		return (0);
	if (ft_strncmp(token, "echo", 4) == 0 && token[4] != '\0')
		return (0);
	if (ft_strncmp(token, "env", 3) == 0 && token[3] != '\0')
		return (0);
	if (ft_strncmp(token, "exit", 4) == 0 && token[4] != '\0')
		return (0);
	if (ft_strncmp(token, "pwd", 3) == 0 && token[3] != '\0')
		return (0);
	if (ft_strncmp(token, "export", 6) == 0 && token[6] != '\0')
		return (0);
	if (ft_strncmp(token, "unset", 5) == 0 && token[5] != '\0')
		return (0);
	return (1);
}
