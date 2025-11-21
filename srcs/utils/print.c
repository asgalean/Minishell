/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:25:12 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Error conventions:
 * ERR_SYNTAX     → exit 2
 * ERR_CMD        → exit 127
 * ERR_PERMISSION → exit 126
 * ERR_VAR        → exit 1
 */

/**
 * @brief EXPORT Imprime mensaje de error para un identificador inválido
 * 
 * @param arg El argumento que causó el error
 */
void	print_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/**
 * @brief print envs with export format
 * 
 * @param env 
 * @param size 
 */
void	print_env_export(char **env, int size)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (i < size)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		equal_sign = ft_strchr(env[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			ft_putstr_fd(env[i], STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(equal_sign + 1, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
			*equal_sign = '=';
		}
		else
			ft_putstr_fd(env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

void	show_eof_warning(char *delimiter)
{
	write(STDOUT_FILENO, "\n", 1);
	ft_putstr_fd("minishell: warning: here-document delimited by "
		"end-of-file (wanted '", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}
