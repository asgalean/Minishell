/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:55:42 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Finds the index of an environment variable in the env array.
 * 
 * @param env The environment variables array (NULL-terminated).
 * @param key The key/name to search for (without '=').
 * @return The index of the variable if found, -1 otherwise.
 */
static int	find_env_index(char **env, char *key)
{
	int		i;
	size_t	key_len;

	if (!env || !key)
		return (-1);
	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * Appends a new environment variable string to the env array.
 * 
 * Allocates a new array with space for the new variable and a NULL terminator,
 * copies the current env, adds the new variable, and frees the old array.
 * 
 * @param env The current environment variables array (NULL-terminated).
 * @param new_var The new variable string to add (ownership is taken).
 * @return The new environment array (must be freed by caller).
 */
static char	**append_env_var(char **env, char *new_var)
{
	int		len;
	char	**new_env;

	len = 0;
	while (env[len])
		len++;
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (env);
	len = -1;
	while (env[++len])
		new_env[len] = env[len];
	new_env[len] = ft_strdup(new_var);
	free(new_var);
	new_env[len + 1] = NULL;
	free(env);
	return (new_env);
}

/**
 * Sets or updates an environment variable in the shell's environment.
 * 
 * If the variable exists, its value is replaced.
 * If it does not exist, it is appended to the environment.
 * 
 * @param shell Pointer to the shell data structure.
 * @param key The variable name (without '=').
 * @param value The value to set for the variable.
 */
void	set_env(t_shell *shell, char *key, const char *value)
{
	char	*new_var;
	int		index;

	if (!shell || !key || !value)
		return ;
	new_var = ft_strjoin_3(key, "=", value);
	if (!new_var)
		return ;
	index = find_env_index(shell->env, key);
	if (index >= 0)
	{
		free(shell->env[index]);
		shell->env[index] = new_var;
	}
	else
		shell->env = append_env_var(shell->env, new_var);
}
