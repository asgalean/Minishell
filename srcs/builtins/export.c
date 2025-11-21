/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:41:25 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/07/27 20:08:59 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Procesa un argumento de tipo variable=valor para export
 * 
 * @param shell Estructura del shell
 * @param arg Argumento a procesar
 * @return int 0 si se procesó correctamente, 1 si hubo error
 */
static int	exec_export(t_shell *shell, char *arg)
{
	char	*equal_sign;
	char	*value;
	int		result;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (1);
	*equal_sign = '\0';
	value = equal_sign + 1;
	result = 0;
	if (!is_valid_varname(arg))
	{
		print_invalid_identifier(arg);
		result = 1;
	}
	else
		set_env(shell, arg, value);
	*equal_sign = '=';
	return (0);
}

/**
 * @brief Implementa el comando builtin 'export' para establecer variables 
 * de entorno
 * 
 * @param shell Estructura principal del shell
 * @param argv Argumentos del comando (argv[0] es "export")
 */
int	builtin_export(t_shell *shell, char **argv)
{
	int		i;
	char	*equal_sign;

	if (!argv[1])
		return (sort_and_print_env(shell), 0);
	i = 0;
	while (argv[++i])
	{
		equal_sign = ft_strchr(argv[i], '=');
		if (equal_sign)
		{
			if (exec_export(shell, argv[i]))
				shell->exit_status = 1;
		}
		else
		{
			if (!is_valid_varname(argv[i]))
			{
				print_invalid_identifier(argv[i]);
				shell->exit_status = 1;
				break ;
			}
		}
	}
	return (shell->exit_status);
}
