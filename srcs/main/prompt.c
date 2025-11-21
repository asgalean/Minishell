/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:04:43 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Returns the current directory as a relative path to the user's home,
 * replacing the home part with '~' if applicable.
 * Returns a newly allocated string.
 */
static char	*get_relative_path(t_shell *shell)
{
	char	*cwd;
	char	*relative;
	size_t	home_len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("~"));
	if (shell->home
		&& ft_strncmp(cwd, shell->home, ft_strlen(shell->home)) == 0)
	{
		home_len = ft_strlen(shell->home);
		if (cwd[home_len] == '\0')
			relative = ft_strdup("~");
		else
			relative = ft_strjoin("~", cwd + home_len);
		free(cwd);
		return (relative);
	}
	return (cwd);
}

/**
 * Builds the colored prefix for the prompt, showing the username and host.
 * Returns a newly allocated string.
 */
static char	*build_prefix(t_shell *shell)
{
	char	*user_color;
	char	*host_color;
	char	*temp1;
	char	*temp2;

	user_color = ft_strjoin(CLR_GREEN, shell->user);
	temp1 = ft_strjoin(user_color, CLR_RESET "@");
	free(user_color);
	host_color = ft_strjoin(CLR_RED, "minishell:");
	temp2 = ft_strjoin(temp1, host_color);
	free(temp1);
	free(host_color);
	return (temp2);
}

/**
 * Builds the full shell prompt string.
 * Format: <user>@minishell:<relative path>$ 
 * All colorized and memory managed.
 * Returns a newly allocated string for use as the prompt.
 */
char	*build_prompt(t_shell *shell)
{
	char	*prefix;
	char	*path;
	char	*temp;
	char	*final;

	prefix = build_prefix(shell);
	path = get_relative_path(shell);
	temp = ft_strjoin(prefix, path);
	free(prefix);
	free(path);
	final = ft_strjoin(temp, "$ " CLR_RESET);
	free(temp);
	return (final);
}
