/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:24:26 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Returns the current working directory as a newly allocated string.
 * On failure, prints an error and returns NULL.
 */
char	*get_current_dir(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: getcwd");
	return (cwd);
}

/**
 * Updates the shell's PWD and OLDPWD variables to the provided values,
 * both in the shell struct and in the environment.
 *
 * @param shell Pointer to the shell state structure.
 * @param oldpwd The previous working directory.
 * @param newpwd The new working directory.
 */
void	update_pwd(t_shell *shell, char *oldpwd, char *newpwd)
{
	if (!shell || !oldpwd || !newpwd)
		return ;
	free(shell->oldpwd);
	free(shell->pwd);
	shell->oldpwd = ft_strdup(oldpwd);
	shell->pwd = ft_strdup(newpwd);
	set_env(shell, "OLDPWD", shell->oldpwd);
	set_env(shell, "PWD", shell->pwd);
}

/**
 * @brief Ordena un array de cadenas alfabéticamente usando bubble sort
 * 
 * @param env Array de cadenas a ordenar
 * @param size Tamaño del array
 */
static void	sort_env(char **env, int size)
{
	int		i;
	int		j;
	char	*temp;
	int		swapped;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		swapped = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (swapped == 0)
			break ;
		i++;
	}
}

/**
 * @brief Create a env copy object
 * 
 * @param env 
 * @param env_size 
 * @return char** 
 */
static char	**create_env_copy(char **env, int env_size)
{
	char	**env_copy;
	int		i;

	env_copy = (char **)malloc(sizeof(char *) * (env_size + 1));
	if (!env_copy)
		exit(1);
	i = 0;
	while (i < env_size)
	{
		env_copy[i] = ft_strdup(env[i]);
		if (!env_copy[i])
		{
			while (--i >= 0)
				free(env_copy[i]);
			free(env_copy);
			exit(1);
		}
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

/**
 * @brief Imprime el entorno ordenado alfabéticamente con el formato 
 * de declare -x
 * 
 * @param shell Estructura del shell que contiene el entorno
 */
void	sort_and_print_env(t_shell *shell)
{
	char	**env_copy;
	int		env_size;
	int		i;

	env_size = 0;
	while (shell->env[env_size] != NULL)
		env_size++;
	env_copy = create_env_copy(shell->env, env_size);
	sort_env(env_copy, env_size);
	print_env_export(env_copy, env_size);
	i = 0;
	while (i < env_size)
		free(env_copy[i++]);
	free(env_copy);
}
