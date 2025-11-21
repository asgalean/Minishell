/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:07:36 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/07/27 21:09:07 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exec_command(struct s_execcmd *e)
{
	int	i;

	i = 0;
	while (e->argv[i])
		free(e->argv[i++]);
	free(e);
}

void	free_redir_command(struct s_redircmd *r)
{
	free(r->file);
	free_command_tree(r->cmd);
	free(r);
}

void	free_pipe_command(struct s_pipecmd *p)
{
	free_command_tree(p->left);
	free_command_tree(p->right);
	free(p);
}

void	free_heredoc_command(struct s_heredoccmd *h)
{
	free(h->content);
	free(h->delimiter);
	free_command_tree(h->cmd);
	free(h);
}

void	free_list_command(struct s_listcmd *l)
{
	free_command_tree(l->left);
	free_command_tree(l->right);
	free(l);
}
