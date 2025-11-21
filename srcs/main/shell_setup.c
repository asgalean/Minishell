/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:52:18 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Frees an array of token strings created by the tokenizer.
 *
 * @param tokens NULL-terminated array of dynamically allocated strings.
 */
void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

static char	*get_current_directory(void)
{
	char	*cwdbuf;
	char	*result;

	cwdbuf = getcwd(NULL, 0);
	if (cwdbuf)
	{
		result = ft_strdup(cwdbuf);
		free(cwdbuf);
	}
	else
		result = ft_strdup("");
	return (result);
}

/**
 * Initializes and allocates the t_shell structure.
 * Sets up shell PID, exit status, environment variables, current directory,
 * user and home variables.
 *
 * @param envp The environment variables array to duplicate.
 * @return Pointer to initialized t_shell struct, or NULL on failure.
 */
t_shell	*setup_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->pid = getpid();
	shell->exit_status = 0;
	shell->env = duplicate_envp(envp);
	if (!shell->env)
		return (free(shell), NULL);
	shell->pwd = get_current_directory();
	shell->oldpwd = ft_strdup(shell->pwd);
	set_env(shell, "PWD", shell->pwd);
	set_env(shell, "OLDPWD", shell->oldpwd);
	shell->user = ft_strdup(get_env_value(shell, "USER"));
	shell->home = ft_strdup(get_env_value(shell, "HOME"));
	interrumped_flag(0);
	if (!shell->user)
		return (free_shell(shell), NULL);
	return (shell);
}

/**
 * @brief tokenize and process the input
 * 
 * @param input 
 * @param shell 
 */
static void	process_command_input(char *input, t_shell *shell)
{
	struct s_command	*cmd;

	if (*input)
	{
		add_history(input);
		shell->tokens = tokenize_input(input, shell);
		if (shell->quote_error)
			return ;
		if (shell->tokens)
		{
			cmd = parse_cmd_tree(shell->tokens, shell);
			if (cmd)
			{
				exec_tree(cmd, shell);
				free_command_tree(cmd);
			}
			free_tokens(shell->tokens);
		}
	}
}

/**
 * Main loop of the interactive shell.
 * Continuously reads user input, tokenizes it, parses it into a command tree,
 * executes the commands, and frees resources after each command.
 *
 * @param shell Pointer to the shell state struct.
 */
void	run_shell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	while (1)
	{
		shell->quote_error = 0;
		prompt = build_prompt(shell);
		if (!prompt)
			break ;
		input = readline(prompt);
		free(prompt);
		if (!input)
			break ;
		process_command_input(input, shell);
		free(input);
	}
}
