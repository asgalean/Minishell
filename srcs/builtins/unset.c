/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:38:04 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/07/09 20:07:56 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Busca el índice de una variable en el entorno
 * 
 * @param shell Estructura del shell
 * @param name Nombre de la variable a buscar
 * @return int Índice de la variable o -1 si no existe
 */
static int	find_env_index(t_shell *shell, char *name)
{
	int		i;
	size_t	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, name_len) == 0
			&& (shell->env[i][name_len] == '='
			|| shell->env[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Elimina una variable del entorno
 * 
 * @param shell Estructura del shell
 * @param name Nombre de la variable a eliminar
 * @return int 0 si se eliminó correctamente, 1 si hubo un error
 */
static int	unset_env(t_shell *shell, char *name)
{
	int	index;

	index = find_env_index(shell, name);
	if (index == -1)
		return (0);
	free(shell->env[index]);
	while (shell->env[index + 1])
	{
		shell->env[index] = shell->env[index + 1];
		index++;
	}
	shell->env[index] = NULL;
	return (0);
}

/**
 * @brief Implementa el comando builtin 'unset' para eliminar variables 
 * de entorno
 * 
 * @param shell Estructura principal del shell
 * @param argv Argumentos del comando (argv[0] es "unset")
 * @return int Estado de salida (0 para éxito, 1 si alguna variable 
 * no pudo eliminarse)
 */
int	builtin_unset(t_shell *shell, char **argv)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_varname(argv[i]))
		{
			ft_putstr_fd("unset: ", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd(": invalid parameter name\n", STDERR_FILENO);
			exit_status = 1;
		}
		else
			unset_env(shell, argv[i]);
		i++;
	}
	return (exit_status);
}
