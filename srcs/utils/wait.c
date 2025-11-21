/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:29:45 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Waits for all child processes to finish, frees a 3D array of commands,
 * and exits the program with the specified exit status.
 *
 * @param cmds Pointer to a 3D array of command strings.
 * @param exit_status Exit code for the process.
 */
void	wait_all_children(char ***cmds, int exit_status)
{
	int	saved_errno;

	saved_errno = errno;
	while (wait(NULL) != -1)
		;
	errno = saved_errno;
	if (cmds)
		free_3d_arr((void ***)cmds);
	exit(exit_status);
}
