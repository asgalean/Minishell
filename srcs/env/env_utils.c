/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:41:15 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// set_env, etc.

#include "minishell.h"

/**
 * Retrieves the value of an environment variable from the shell's 
 * environment.
 * 
 * Searches shell->env for a variable matching var_name.
 * 
 * @param shell Pointer to the shell data structure.
 * @param var_name The name of the variable to retrieve.
 * @return A pointer to the value string if found, or a newly allocated 
 * empty string if not found.
 *         (Caller is responsible for freeing only if not found.)
 */
char	*get_env_value(t_shell *shell, char *var_name)
{
	size_t	i;
	size_t	var_len;

	var_len = ft_strlen(var_name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var_name, var_len) == 0 && \
			shell->env[i][var_len] == '=')
			return (shell->env[i] + var_len + 1);
		i++;
	}
	return ("");
}

/**
 * Creates a deep copy of the environment variables array (envp).
 * 
 * Allocates memory for a new array of strings and duplicates each string.
 * 
 * @param envp The original environment variables array (NULL-terminated).
 * @return A newly allocated array of duplicated strings, or NULL 
 * if allocation fails.
 */
char	**duplicate_envp(char **envp)
{
	char	**env_copy;
	int		env_n;

	env_n = 0;
	while (envp[env_n])
		env_n++;
	env_copy = malloc((env_n + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	env_n = 0;
	while (envp[env_n])
	{
		env_copy[env_n] = ft_strdup(envp[env_n]);
		if (!env_copy)
		{
			while (--env_n >= 0)
				free(env_copy[env_n]);
			free(env_copy);
			return (NULL);
		}
		env_n++;
	}
	env_copy[env_n] = NULL;
	return (env_copy);
}

/**
* @brief Verifica si un nombre de variable es válido
* 
* @param name Nombre a verificar
* @return int 1 si es válido, 0 si no
*/

int	is_valid_varname(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
