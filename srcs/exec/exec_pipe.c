/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:57:30 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Handles the left side of a pipe in a child process.
 * Redirects STDOUT to the write end of the pipe and executes the left 
 * command subtree.
 * Exits the child process with the result of exec_tree.
 * 
 * @param cmd Pointer to the command AST node for the left side.
 * @param fds Pipe file descriptors (fds[1] is write end).
 * @param shell Pointer to shell state.
 */
int	child_pipe_left(struct s_command *cmd, int *fds, t_shell *shell)
{
	struct s_pipecmd	*p;
	int					exit_code;

	p = (struct s_pipecmd *)cmd;
	restore_default_signals();
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
	exit_code = exec_tree(p->left, shell);
	handle_exit_frees(shell, cmd);
	exit(exit_code);
}

/**
 * Handles the right side of a pipe in a child process.
 * Redirects STDIN to the read end of the pipe and executes the right 
 * command subtree.
 * Exits the child process with the result of exec_tree.
 * 
 * @param cmd Pointer to the command AST node for the right side.
 * @param fds Pipe file descriptors (fds[0] is read end).
 * @param shell Pointer to shell state.
 */
int	child_pipe_right(struct s_command *cmd, int *fds, t_shell *shell)
{
	struct s_pipecmd	*p;
	int					exit_code;

	p = (struct s_pipecmd *)cmd;
	restore_default_signals();
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	close(fds[1]);
	exit_code = exec_tree(p->right, shell);
	handle_exit_frees(shell, cmd);
	exit(exit_code);
}

/**
 * Handles the exit status of a process in a pipeline.
 * Updates the shell's exit_status field.
 * Handles signal-based exits (SIGINT, SIGQUIT) and prints corresponding 
 * messages.
 * 
 * @param status The status code from waitpid().
 * @param shell Pointer to shell state.
 * @return The updated exit status for the shell.
 */
int	handle_pipe_exit(int status, t_shell *shell)
{
	int	signal;

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
 * Executes a pipeline command by forking two child processes for the 
 * left and right sides.
 * Manages the pipe file descriptors, waits for both children, and 
 * restores signals.
 * 
 * @param p Pointer to the pipeline command struct.
 * @param shell Pointer to shell state.
 * @return The exit status of the right-side process (as in bash).
 */
int	exec_pipe(struct s_pipecmd *p, t_shell *shell)
{
	int		fds[2];
	int		status1;
	int		status2;
	pid_t	c1;
	pid_t	c2;

	pipe(fds);
	c1 = fork();
	if (c1 == 0)
		child_pipe_left((struct s_command *) p, fds, shell);
	c2 = fork();
	if (c2 == 0)
		child_pipe_right((struct s_command *) p, fds, shell);
	close(fds[0]);
	close(fds[1]);
	ignore_signals();
	waitpid(c1, &status1, 0);
	waitpid(c2, &status2, 0);
	setup_signals();
	return (handle_pipe_exit(status2, shell));
}
