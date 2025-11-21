/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:04:18 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/07/27 20:24:32 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Comprueba si una línea contiene el delimitador del heredoc
 * @param line Línea a comprobar
 * @param delimiter Delimitador del heredoc
 * @return 1 si es el delimitador, 0 si no lo es
 */
int	is_delimiter(char *line, char *delimiter)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
		len--;
	}
	if (ft_strcmp(line, delimiter) == 0)
		return (1);
	if (len > 0)
		line[len] = '\n';
	return (0);
}

struct s_command	*heredoc_cmd(struct s_command *subcmd, char *delim,
	char *content)
{
	struct s_heredoccmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->type = HEREDOC;
	cmd->cmd = subcmd;
	cmd->delimiter = delim;
	cmd->content = content;
	return ((struct s_command *)cmd);
}

/**
 * @brief Set the up heredoc signals object
 * 
 * @param sa_int_old 
 */
void	setup_heredoc_signals(struct sigaction *sa_int_old)
{
	struct sigaction	sa_new;

	sa_new.sa_handler = heredoc_sigint_handler;
	ft_memset(&sa_new.sa_mask, 0, sizeof(sa_new.sa_mask));
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, sa_int_old);
	interrumped_flag(0);
}
