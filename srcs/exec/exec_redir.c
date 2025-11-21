/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:57:36 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dupping(struct s_redircmd *r, t_shell *shell, int *save, int fd)
{
	int	st;

	*save = dup(r->fd);
	dup2(fd, r->fd);
	close(fd);
	st = exec_tree(r->cmd, shell);
	dup2(*save, r->fd);
	close(*save);
	return (st);
}

/**
 * Executes a redirection command by opening the specified file and 
 * redirecting STDIN/STDOUT.
 * 
 * @param r Pointer to the redirection command struct.
 * @param shell Pointer to shell state.
 * @return The exit status of the executed command.
 */
int	exec_redir(struct s_redircmd *r, t_shell *shell)
{
	int					fd;
	int					st;
	int					save;
	struct s_redircmd	*inner_redir;

	save = 0;
	fd = open(r->file, r->mode, 0644);
	if (fd < 0)
		return (print_perror(shell, ERR_NO_DIR, r->file), shell->exit_status);
	if (r->cmd->type == REDIR)
	{
		inner_redir = (struct s_redircmd *)r->cmd;
		if (r->fd != inner_redir->fd)
			st = dupping(r, shell, &save, fd);
		else
		{
			close(fd);
			st = exec_tree(r->cmd, shell);
		}
	}
	else
		st = dupping(r, shell, &save, fd);
	return (st);
}
