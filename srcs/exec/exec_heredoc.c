/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:19:07 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/07/27 20:05:15 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Reserva o amplía memoria para el contenido del heredoc
 * @param content Puntero actual al contenido o NULL si es la primera asignación
 * @param curr_size Tamaño actual del contenido
 * @param needed_size Tamaño total necesario incluyendo el nuevo contenido
 * @return Puntero a la nueva memoria asignada, NULL en caso de error
 */
static char	*heredoc_allocate(char *content, size_t curr_size,
	size_t needed_size)
{
	char	*new_content;
	size_t	new_capacity;

	new_capacity = curr_size + needed_size + 128;
	new_content = malloc(new_capacity);
	if (!new_content)
	{
		free(content);
		return (NULL);
	}
	if (content)
	{
		ft_memcpy(new_content, content, curr_size);
		free(content);
	}
	return (new_content);
}

/**
 * Añade una línea al contenido del heredoc
 * @param content Puntero al contenido actual
 * @param line Línea a añadir
 * @param content_size Puntero a la variable que mantiene el tamaño actual
 * @return Puntero actualizado al contenido
 */
static char	*add_line_to_content(char *content, char *line,
	size_t *content_size)
{
	size_t	line_len;
	char	*new_content;

	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] != '\n')
	{
		line[line_len] = '\n';
		line_len++;
	}
	new_content = heredoc_allocate(content, *content_size, line_len + 1);
	if (!new_content)
		return (NULL);
	content = new_content;
	ft_memcpy(content + *content_size, line, line_len);
	*content_size += line_len;
	content[*content_size] = '\0';
	return (content);
}

/**
 * Read herEdoc lines until EOF
 * @param delimiter 
 * @param content_size 
 * @return char* 
 */
static char	*read_heredoc_lines(char *delimiter, size_t *content_size)
{
	char	*content;
	char	*line;

	content = NULL;
	*content_size = 0;
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = gnl(STDIN_FILENO);
	while (!interrumped_flag(2) && line != NULL)
	{
		if (is_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		content = add_line_to_content(content, line, content_size);
		free(line);
		if (!content)
			return (NULL);
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = gnl(STDIN_FILENO);
	}
	if (line == NULL && !interrumped_flag(2))
		show_eof_warning(delimiter);
	return (content);
}

/**
 * Handles reading and signals of heredoc
 * @param delimiter 
 * @return char* 
 */
char	*read_heredoc_content(char *delimiter)
{
	char				*content;
	size_t				content_size;
	struct sigaction	sa_int_old;

	setup_heredoc_signals(&sa_int_old);
	content = read_heredoc_lines(delimiter, &content_size);
	sigaction(SIGINT, &sa_int_old, NULL);
	if (interrumped_flag(2))
	{
		ioctl(STDIN_FILENO, TCFLSH, 0);
		free(content);
		return (NULL);
	}
	if (!content)
		content = ft_strdup("");
	return (content);
}

/**
 * Executes a command with heredoc input.
 * Creates a temporary pipe, writes heredoc content to it, and
 * executes the command with STDIN redirected from the pipe.
 * 
 * @param h Pointer to the heredoc command struct.
 * @param shell Pointer to shell state.
 * @return The exit status of the command execution.
 */
int	exec_heredoc(struct s_heredoccmd *h, t_shell *shell)
{
	int		fds[2];
	pid_t	child_pid;
	int		status;
	int		exit_status;

	if (pipe(fds) < 0)
		return (perror("pipe"), 1);
	write(fds[1], h->content, strlen(h->content));
	close(fds[1]);
	child_pid = fork();
	if (child_pid == 0)
	{
		restore_default_signals();
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		exit_status = exec_tree(h->cmd, shell);
		handle_exit_frees(shell, (struct s_command *)h);
		exit(exit_status);
	}
	close(fds[0]);
	ignore_signals();
	waitpid(child_pid, &status, 0);
	setup_signals();
	return (handle_pipe_exit(status, shell));
}
