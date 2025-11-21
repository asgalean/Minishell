/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:35:46 by cvon-hee          #+#    #+#             */
/*   Updated: 2024/11/14 15:33:58 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"
#include <stdio.h>

static char	*ft_read_line(int fd, char *stash, char *buf)
{
	ssize_t	read_size;
	char	*aux;

	read_size = 1;
	while (read_size > 0)
	{
		read_size = read(fd, buf, BUFFER_SIZE);
		if (read_size == -1)
			return (0);
		if (read_size == 0)
			break ;
		if (!stash)
			stash = ft_strdup("");
		buf[read_size] = '\0';
		aux = stash;
		stash = ft_strjoin(aux, buf);
		free(aux);
		aux = NULL;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (stash);
}

static char	*ft_next_stash(char *line)
{
	size_t	i;
	char	*stash;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\0')
		return (0);
	stash = ft_substr(line, i + 1, ft_strlen(line) - i - 1);
	if (!stash || stash[0] == '\0')
	{
		free(stash);
		stash = NULL;
	}
	line[i + 1] = '\0';
	return (stash);
}

char	*gnl(int fd)
{
	char		*line;
	char		*buf;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (0);
	line = ft_read_line(fd, stash, buf);
	free (buf);
	if (!line)
	{
		free (stash);
		stash = NULL;
		return (0);
	}
	stash = ft_next_stash(line);
	return (line);
}
