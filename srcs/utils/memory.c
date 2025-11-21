/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:29:08 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Frees a 3D array of strings.
 * 
 * @param str Pointer to a 3D array (NULL-terminated at all levels).
 */
void	free_3d_arr(void ***str)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!str)
		return ;
	while (str[i] != NULL)
	{
		j = 0;
		while (str[i][j] != NULL)
		{
			free(str[i][j]);
			j++;
		}
		free(str[i]);
		i++;
	}
	free(str);
}

/**
 * Frees a 2D array of pointers.
 *
 * @param array Pointer to a 2D array (NULL-terminated).
 */
void	free_2d_arr(void **array)
{
	size_t	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
}

/**
 * Frees a NULL-terminated array of strings.
 * (Helper for freeing command argument arrays.)
 *
 * @param arr Array of strings.
 */
void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr && arr[i])
		free(arr[i++]);
	free(arr);
}
