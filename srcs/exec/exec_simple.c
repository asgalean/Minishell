/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:57:40 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fork_and_exec(char *path, char **argv, t_shell *shell);
static int	wait_and_get_status(pid_t pid, t_shell *shell);

static int	get_exit_code(int error_num)
{
	if (error_num == ENOENT)
		return (127);
	else if (error_num == EACCES)
		return (126);
	return (1);
}

/**
 * Forks a child process and executes an external command using execve.
 * Handles errors like command not found and permission denied and exits 
 * accordingly.
 * 
 * @param path The executable path for the command.
 * @param argv Argument vector for execve.
 * @param shell Pointer to shell state.
 * @return The exit status of the child process.
 */
static int	fork_and_exec(char *path, char **argv, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), shell->exit_status = 1);
	if (pid == 0)
	{
		restore_default_signals();
		free_tokens(shell->tokens);
		execve(path, argv, shell->env);
		if (errno == ENOENT)
			print_perror(shell, ERR_CMD, argv[0]);
		else if (errno == EACCES)
			print_perror(shell, ERR_PERMISSION, argv[0]);
		else
			perror("execve");
		free(path);
		exit(get_exit_code(errno));
	}
	return (wait_and_get_status(pid, shell));
}

/**
 * Waits for a child process to finish and handles its exit status.
 * Updates the shell's exit status and prints signal messages (Ctrl+C, Ctrl+\).
 * 
 * @param pid The process ID of the child.
 * @param shell Pointer to shell state.
 * @return The exit status as returned by the child process.
 */
static int	wait_and_get_status(pid_t pid, t_shell *shell)
{
	int	status;
	int	signal;

	ignore_signals();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == SIGINT)
			write(1, "\n", 1);
		else if (signal == SIGQUIT)
			write(1, "Quit: 3\n", 8);
		shell->exit_status = 128 + signal;
	}
	else
		shell->exit_status = WEXITSTATUS(status);
	return (shell->exit_status);
}

/**
 * Executes a simple command.
 * If the command is a built-in, it executes it in the current process.
 * Otherwise, it resolves the command path, forks, and executes it with execve.
 * 
 * @param e Pointer to the simple command struct.
 * @param shell Pointer to shell state.
 * @return The exit status of the executed command.
 */
int	exec_simple(struct s_execcmd *e, t_shell *shell)
{
	char	*path;
	int		result;

	if (!e->argv[0])
		return (0);
	if (is_builtin(e->argv[0]) && !is_valid_builtin(e->argv[0]))
		return (print_perror(shell, ERR_CMD, e->argv[0]), 127);
	if (is_builtin(e->argv[0]))
		return (shell->exit_status = exec_builtin(e, shell));
	path = resolve_cmd_path(e->argv[0], shell->env);
	if (!path)
		return (print_perror(shell, ERR_CMD, e->argv[0]), 127);
	result = fork_and_exec(path, e->argv, shell);
	free(path);
	return (result);
}
