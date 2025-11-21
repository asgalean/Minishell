/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:29:41 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Prints a system error message and terminates the program with failure.
 */
void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

/**
 * @brief init the errors, the format is awfull cause norm
 * 
 * @param a error msgs struct
 */
static void	init_error_table(t_error_info a[])
{
	a[ERR_SYNTAX].format = "minishell: syntax error near unexpected "
		"token `%s`\n";
	a[ERR_SYNTAX].exit_status = 2;
	a[ERR_CMD].format = "minishell: %s: command not found\n";
	a[ERR_CMD].exit_status = 127;
	a[ERR_PERMISSION].format = "minishell: %s: permission denied\n";
	a[ERR_PERMISSION].exit_status = 126;
	a[ERR_VAR].format = "minishell: %s: variable not found\n";
	a[ERR_VAR].exit_status = 1;
	a[ERR_NO_DIR].format = "minishell: no such file or directory: "
		"%s\n";
	a[ERR_NO_DIR].exit_status = 126;
}

/**
 * Prints a minishell-formatted error message for the specified error type,
 * sets the shell's exit status accordingly, and, if needed, includes details.
 *
 * @param shell Pointer to the shell state structure.
 * @param err   Error type (enum t_ms_error).
 * @param detail Optional string for details (e.g., command or variable name).
 */
void	print_perror(t_shell *shell, t_ms_error err, const char *detail)
{
	static t_error_info	error_info[5];
	static int			initialized;

	initialized = 0;
	if (!initialized)
	{
		init_error_table(error_info);
		initialized = 1;
	}
	if (err < 0 || err >= sizeof(error_info) / sizeof(error_info[0]))
	{
		printf("Error code: %d\n", err);
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = error_info[err].exit_status;
	if (err == ERR_SYNTAX && !detail)
		printf("minishell: syntax error near unexpected token\n");
	else if (detail)
		printf(error_info[err].format, detail);
	else
		printf(error_info[err].format, "");
}
