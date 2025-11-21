/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:04:25 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// construcción del árbol de comandos
static struct s_command	*parse_list(char ***tokens, t_shell *shell);
static struct s_command	*parse_pipeline(char ***tokens, t_shell *shell);

/**
 * Entry point for the command parser.
 * Parses a list of tokens into a command tree representing the shell's AST.
 *
 * @param tokens Array of token strings (NULL-terminated).
 * @param shell Pointer to shell state/context.
 * @return Pointer to the root node of the command tree.
 */
struct s_command	*parse_cmd_tree(char **tokens, t_shell *shell)
{
	return (parse_list(&tokens, shell));
}

/**
 * Parses a list of commands separated by logical ('&&', '||') 
 * or sequential (';') operators.
 * Logical and sequential connectors have lower precedence than pipes.
 * Builds the corresponding nodes in the command tree.
 *
 * @param tokens Pointer to pointer to the tokens array.
 * @param shell Pointer to shell state/context.
 * @return Pointer to the leftmost node in the built command tree.
 */
static struct s_command	*parse_list(char ***tokens, t_shell *shell)
{
	struct s_command	*cmd;
	struct s_command	*right;
	struct s_listcmd	*lcmd;
	char				*op;

	cmd = parse_pipeline(tokens, shell);
	while (**tokens && (ft_strcmp(**tokens, "&&") == 0 || \
		ft_strcmp(**tokens, "||") == 0 || ft_strcmp(**tokens, ";") == 0))
	{
		op = *(*tokens)++;
		right = parse_pipeline(tokens, shell);
		lcmd = calloc(1, sizeof(*lcmd));
		if (ft_strcmp(op, "&&") == 0)
			lcmd->type = AND;
		else if (ft_strcmp(op, "||") == 0)
			lcmd->type = OR;
		else
			lcmd->type = LIST;
		lcmd->left = cmd;
		lcmd->right = right;
		cmd = (struct s_command *)lcmd;
	}
	return (cmd);
}

/**
 * Parses a pipeline of commands separated by the pipe operator ('|').
 * Each pipeline node connects left and right subcommands.
 *
 * @param tokens Pointer to pointer to the tokens array.
 * @param shell Pointer to shell state/context.
 * @return Pointer to the pipeline command node.
 */
static struct s_command	*parse_pipeline(char ***tokens, t_shell *shell)
{
	struct s_command	*cmd;
	struct s_command	*right;
	struct s_pipecmd	*pc;

	cmd = parse_exec(tokens, shell);
	while (**tokens && ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		right = parse_exec(tokens, shell);
		if (!right)
			return (NULL);
		pc = ft_calloc(1, sizeof(*pc));
		pc->type = PIPE;
		pc->left = cmd;
		pc->right = right;
		cmd = (struct s_command *) pc;
	}
	return (cmd);
}

/**
 * Procesa un heredoc y crea el nodo correspondiente en el AST.
 * 
 * @param subcmd Comando que recibirá la entrada del heredoc
 * @param tokens Puntero a los tokens de entrada
 * @param shell Estado del shell
 * @return Un nuevo nodo de comando que representa el heredoc
 */
struct s_command	*parse_heredoc(struct s_command *subcmd,
	char ***tokens, t_shell *shell)
{
	char	*delim;
	char	*content;

	delim = *(*tokens)++;
	if (!delim)
	{
		print_perror(shell, ERR_SYNTAX, NULL);
		free_command_tree(subcmd);
		return (NULL);
	}
	content = read_heredoc_content(delim);
	if (!content)
	{
		if (interrumped_flag(2))
			shell->exit_status = 130;
		free_command_tree(subcmd);
		return (NULL);
	}
	return (heredoc_cmd(subcmd, ft_strdup(delim), content));
}
