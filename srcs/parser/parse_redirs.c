/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:12:51 by asgalean          #+#    #+#             */
/*   Updated: 2025/09/17 17:12:58 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Declaraciones de funciones auxiliares
/*
static int	is_invalid_token(char *token);
static struct s_command	*handle_redirection(struct s_command *subcmd,
		char ***tokens, t_shell *shell, char *op);
static struct s_command	*build_redirection_tree(struct s_command *subcmd,
		char **ops, char **files, int count);
*/
/**
 * @brief Comprueba si el token es un operador que no puede seguir a una
 *        redirección.
 * @param token El token a comprobar.
 * @return 1 si es inválido, 0 en caso contrario.
 */
static int	is_invalid_token(char *token)
{
	if (!token)
		return (1);
	if (is_redirection_operator(token) || !ft_strcmp(token, "|")
		|| !ft_strcmp(token, ";") || !ft_strcmp(token, "&&")
		|| !ft_strcmp(token, "||"))
		return (1);
	return (0);
}

/**
 * @brief Procesa una redirección, ya sea heredoc u otra.
 * @param subcmd El subcomando actual.
 * @param tokens Puntero al array de tokens.
 * @param shell Puntero a la estructura del shell.
 * @param op El operador de redirección.
 * @return El nuevo subcomando, o NULL si hay un error.
 */
static struct s_command	*handle_redirection(struct s_command *subcmd,
	char ***tokens, t_shell *shell, char *op)
{
	if (ft_strcmp(op, "<<") == 0)
		return (parse_heredoc(subcmd, tokens, shell));
	if (is_invalid_token(**tokens))
	{
		print_perror(shell, ERR_SYNTAX, NULL);
		free_command_tree(subcmd);
		return (NULL);
	}
	return (subcmd);
}

/**
 * @brief Construye el árbol de comandos para las redirecciones de ficheros.
 * @param subcmd El subcomando actual.
 * @param ops Array de operadores de redirección.
 * @param files Array de nombres de fichero.
 * @param count El número de redirecciones.
 * @return El nuevo subcomando, o NULL si hay un error.
 */
static struct s_command	*build_redirection_tree(struct s_command *subcmd,
	char **ops, char **files, int count)
{
	int	i;

	i = count - 1;
	while (i >= 0)
	{
		subcmd = handle_file_redirection_single(subcmd, files[i], ops[i]);
		if (!subcmd)
			return (NULL);
		i--;
	}
	return (subcmd);
}

/**
 * @brief Parsea y gestiona las redirecciones para un comando.
 * @param subcmd El subcomando a procesar.
 * @param tokens Puntero al array de tokens.
 * @param shell Puntero a la estructura del shell.
 * @return El subcomando con el árbol de redirecciones construido,
 *         o NULL si ocurre un error de sintaxis.
 */
struct s_command	*parse_redirs(struct s_command *subcmd,
	char ***tokens, t_shell *shell)
{
	char	*ops[64];
	char	*files[64];
	int		count;
	char	*op;

	count = 0;
	while (**tokens && is_redirection_operator(**tokens))
	{
		op = *(*tokens)++;
		subcmd = handle_redirection(subcmd, tokens, shell, op);
		if (!subcmd)
			return (NULL);
		if (ft_strcmp(op, "<<") != 0)
		{
			ops[count] = op;
			files[count] = *(*tokens)++;
			count++;
		}
	}
	return (build_redirection_tree(subcmd, ops, files, count));
}
