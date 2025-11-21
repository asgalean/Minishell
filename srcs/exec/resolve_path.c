/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:16 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Retrieves the PATH environment variable from the given environment array.
 * 
 * @param env The environment variables array (NULL-terminated).
 * @return Pointer to the string containing the PATH variable, or NULL if 
 * not found.
 */
static char	*get_path_var(char **env)
{
	size_t	i;
	char	*result;

	i = 0;
	if (!env)
		return (NULL);
	while (env && env[i])
	{
		result = ft_strnstr(env[i], "PATH=", 5);
		if (result != NULL)
			return (result + 5);
		i++;
	}
	return (NULL);
}

/**
 * @brief 
 * 
 * @param dirs 
 * @return char** 
 */
static char	**append_slash_to_dirs(char **dirs)
{
	size_t	i;
	char	*joined;

	i = 0;
	while (dirs[i])
	{
		joined = ft_strjoin(dirs[i], "/");
		if (!joined)
		{
			free_2d_arr((void **)dirs);
			return (NULL);
		}
		free(dirs[i]);
		dirs[i] = joined;
		i++;
	}
	return (dirs);
}

/**
 * Splits the PATH environment variable into an array of directories.
 * Appends a trailing slash to each directory for easier command path 
 * resolution.
 * 
 * @param env The environment variables array (NULL-terminated).
 * @return NULL-terminated array of directory strings with trailing 
 * slashes, or NULL on failure.
 */
static char	**get_path_dirs(char **env)
{
	char	*path_value;
	char	**dirs;
	size_t	i;

	path_value = get_path_var(env);
	if (!path_value)
		return (NULL);
	if (ft_strchr(path_value, ':') != NULL)
		dirs = ft_split(path_value, ':');
	else
	{
		dirs = malloc(sizeof(char *) * 2);
		dirs[0] = ft_strdup(path_value);
		dirs[1] = NULL;
	}
	if (!dirs)
		return (NULL);
	i = 0;
	return (append_slash_to_dirs(dirs));
}

/**
 * Resolves the full executable path for a command.
 * - If the command contains a slash and is executable, returns its duplicate.
 * - Otherwise, searches for the command in the directories of PATH.
 * 
 * @param cmd_name The name of the command to resolve.
 * @param env      The environment variables array.
 * @return Allocated string with the executable path, or NULL if not found.
 */
char	*resolve_cmd_path(char *cmd_name, char **env)
{
	char	**dirs;
	char	*candidate;
	size_t	i;

	if (!cmd_name || !*cmd_name)
		return (NULL);
	if (ft_strchr(cmd_name, '/') && access(cmd_name, X_OK) == 0)
		return (ft_strdup(cmd_name));
	dirs = get_path_dirs(env);
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		candidate = ft_strjoin(dirs[i], cmd_name);
		if (!candidate)
			break ;
		if (access(candidate, X_OK) == 0)
			return (free_2d_arr((void **)dirs), candidate);
		free(candidate);
		i++;
	}
	free_2d_arr((void **)dirs);
	return (NULL);
}
