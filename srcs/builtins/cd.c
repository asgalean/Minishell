/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:28:48 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Changes directory to the user's home directory ("cd" with no arguments).
// Returns 0 on success, 1 on failure (printing a suitable error message).
static int	cd_home(t_shell *shell)
{
	char	*home;

	home = get_env_value(shell, "HOME");
	if (chdir(home) == -1)
		return (perror("minishell: cd"), 1);
	return (0);
}

// Changes directory to the previous working directory ("cd -").
// Prints error if OLDPWD is not set or if chdir fails.
// Returns 0 on success, 1 on failure.
static int	cd_dash(t_shell *shell)
{
	char	*oldpwd;

	oldpwd = get_env_value(shell, "OLDPWD");
	if (!oldpwd)
		return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2), 1);
	if (chdir(oldpwd) == -1)
		return (perror("minishell: cd"), 1);
	return (0);
}

// Attempts to change the current working directory to the given path.
// Prints a system error message if chdir fails.
// Returns 0 on success, 1 on failure.
static int	try_chdir(const char *path)
{
	if (chdir(path) == -1)
		return (perror(path), 1);
	return (0);
}

// Changes directory to the given path (handles "cd <path>").
// Returns the result of try_chdir.
static int	cd_to(char *path)
{
	return (try_chdir(path));
}

/**
 * Implements the `cd` built-in command.
 * Handles the following cases:
 * - `cd` with no arguments: changes to $HOME.
 * - `cd -`: changes to $OLDPWD.
 * - `cd <path>`: changes to the specified path.
 * Updates the shell's PWD and OLDPWD variables after changing directory.
 * Prints debug information if DEBUG_MODE is enabled.
 * Returns 0 on success, 1 on failure.
 */
int	builtin_cd(t_shell *shell, char **argv)
{
	int		result;
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup(shell->pwd);
	result = 0;
	if (!argv[1] || argv[1][0] == '\0')
		result = cd_home(shell);
	else if (ft_strcmp(argv[1], "-") == 0)
		result = cd_dash(shell);
	else
		result = cd_to(argv[1]);
	newpwd = get_current_dir();
	if (!newpwd)
		newpwd = ft_strdup(shell->pwd);
	update_pwd(shell, oldpwd, newpwd);
	free(oldpwd);
	free(newpwd);
	return (result);
}
