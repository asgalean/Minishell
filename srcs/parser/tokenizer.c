/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:36:13 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Reads and processes a single-quoted string from the input pointer.
 * Returns a dynamically allocated string containing the content between
 * the single quotes.
 *
 * @param p Pointer to the input string pointer (will be advanced).
 * @return Newly allocated string with the quoted content.
 */
char	*read_single(char **p, t_shell *shell) //version gorka
{
	char	*start;
	char	*token;

	start = ++*p;
	while (**p && **p != '\'')
		(*p)++;
	if (**p != '\'')
	{
		print_perror(shell, ERR_SYNTAX, "quote");
		shell->quote_error = 1;
		return (NULL);
	}
	token = ft_substr(start, 0, *p - start);
	if (**p == '\'')
		(*p)++;
	return (token);
}

/**
 * Fills an array with operator strings for command parsing
 * @param ops Array to be filled with operator strings
 */
static void	fill_operator_array(char *ops[])
{
	ops[0] = "&&";
	ops[1] = "||";
	ops[2] = ">>";
	ops[3] = "<<";
	ops[4] = ";";
	ops[5] = "|";
	ops[6] = "<";
	ops[7] = ">";
	ops[8] = NULL;
}

/**
 * Matches operators (`&&`, `||`, `>`, `<`, etc.) from the input pointer.
 * Returns a dynamically allocated string of the matched operator,
 * or NULL if no match is found.
 *
 * @param p Pointer to the input string pointer (will be advanced).
 * @return Newly allocated string with the operator, or NULL if not matched.
 */
static char	*match_operator(char **p)
{
	int		i;
	size_t	len;
	char	*op;
	char	*ops[9];

	fill_operator_array(ops);
	i = 0;
	while (ops[i])
	{
		len = ft_strlen(ops[i]);
		if (ft_strncmp(*p, ops[i], len) == 0)
		{
			op = ft_substr(*p, 0, len);
			*p += len;
			return (op);
		}
		i++;
	}
	return (NULL);
}

/**
 * Reads the next token from the input pointer.
 * Handles operators, quotes, and unquoted strings.
 * Returns a dynamically allocated string of the token.
 *
 * @param input Pointer to the input string pointer (will be advanced).
 * @param shell Pointer to shell state/context.
 * @return Newly allocated string with the next token, or NULL if end of input.
 */
static char	*next_token(char **input, t_shell *shell)
{
	char	*tok;

	while (**input == ' ')
		(*input)++;
	if (!**input)
		return (NULL);
	if (**input == '\'')
	{
		tok = read_single(input, shell);
		if (!tok && shell->quote_error)
			return (NULL);
	}
	else if (**input == '"')
	{
		tok = read_double(input, shell);
		if (!tok && shell->quote_error)
			return (NULL);
	}
	else
	{
		tok = match_operator(input);
		if (!tok)
			tok = read_unquoted(input, shell);
	}
	return (tok);
}

/**
 * Tokenizes the input string into an array of tokens.
 * Supports handling of quotes, operators, and variable expansions.
 *
 * @param input Input string to tokenize.
 * @param shell Pointer to shell state/context.
 * @return Dynamically allocated array of tokens (NULL-terminated) 
 * or NULL on failure.
 */
char	**tokenize_input(char *input, t_shell *shell)
{
	char	**tokens;
	char	*tok;
	int		i;

	i = 0;
	tokens = malloc(sizeof(*tokens) * MAX_TOKENS);
	if (!tokens)
		return (NULL);
	while (*input && !shell->quote_error)
	{
		tok = next_token(&input, shell);
		if (tok && tok[0] != '\0')
			tokens[i++] = tok;
		else
			free(tok);
	}
	tokens[i] = (NULL);
	return (tokens);
}
