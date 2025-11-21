/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lvl_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvon-hee <cvon-hee@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:36:22 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/07/09 20:09:42 by cvon-hee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_shell_level(void)
{
	char	*shlvl_str;
	int		level;

	shlvl_str = getenv("SHLVL");
	if (!shlvl_str)
		return (0);
	level = ft_atoi(shlvl_str);
	return (level);
}

void	set_shell_level(t_shell *shell)
{
	int		current_level;
	char	*level_str;

	current_level = get_shell_level();
	current_level++;
	if (current_level > 100)
	{
		printf("warning: shell level (%d) too high, resetting to 1\n",
			get_shell_level());
		set_env(shell, "SHLVL", "2");
	}
	level_str = ft_itoa(current_level);
	set_env(shell, "SHLVL", level_str);
	free(level_str);
}
