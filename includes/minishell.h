/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:38:34 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ---- INCLUDES ----------------------------------------------------------- */
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>

/* ---- CONSTANTS ---------------------------------------------------------- */
# define MAX_ARGS 64
# define MAX_TOKENS 256

/* ---- COMMAND TYPES ------------------------------------------------------ */
# define EXEC		1
# define REDIR		2
# define PIPE		3
# define LIST		4
# define BACK		5
# define AND		6
# define OR			7
# define HEREDOC	8

/* ---- STRUCTURES ---------------------------------------------------------- */
typedef struct s_shell
{
	char	**env;
	char	*user;
	char	*home;
	char	*pwd;
	char	*oldpwd;
	int		exit_status;
	pid_t	pid;
	char	**tokens;
	int		quote_error;
}	t_shell;

// nodo base de arbol de comandos con type
typedef struct s_command
{
	int	type;
}	t_command;

// comando con argumentos
typedef struct s_execcmd
{
	int		type;
	char	*argv[MAX_ARGS];
	char	*eargv[MAX_ARGS];
}	t_execcmd;

// redirección
typedef struct s_redircmd
{
	int					type;
	struct s_command	*cmd;
	char				*file;
	char				*efile;
	int					mode;
	int					fd;
}	t_redircmd;

// pipeline
typedef struct s_pipecmd
{
	int					type;
	struct s_command	*left;
	struct s_command	*right;
}	t_pipecmd;

// HEREDOC (<<)
typedef struct s_heredoccmd
{
	int					type;
	struct s_command	*cmd;
	char				*delimiter;
	char				*content;
}	t_heredoccmd;

// listas de comandos (; o &&, ||)
typedef struct s_listcmd
{
	int					type;
	struct s_command	*left;
	struct s_command	*right;
}	t_listcmd;

// manejo de errores
typedef enum s_ms_error
{
	ERR_SYNTAX,
	ERR_CMD,
	ERR_PERMISSION,
	ERR_VAR,
	ERR_NO_DIR
	/* ... otros errores */
}	t_ms_error;

typedef struct s_error_info
{
	const char	*format;
	int			exit_status;
}	t_error_info;

/* ---- BUILTINS ------------------------------------------------------------ */
int					is_builtin(char *cmd);
int					is_valid_builtin(char *token);
int					builtin_cd(t_shell *shell, char **argv);
int					builtin_echo(char **argv);
int					builtin_env(t_shell *shell);
int					builtin_exit(t_shell *shell, struct s_execcmd *e);
int					builtin_pwd(t_shell *shell);
int					builtin_export(t_shell *shell, char **argv);
int					builtin_unset(t_shell *shell, char **argv);

/* ---- MAIN ---------------------------------------------------------------- */
char				*build_prompt(t_shell *shell);
void				setup_signals(void);
void				restore_default_signals(void);
void				ignore_signals(void);
t_shell				*setup_shell(char **envp);
void				run_shell_loop(t_shell *shell);
void				heredoc_sigint_handler(int sig);

/* ---- ENV ----------------------------------------------------------------- */
char				**duplicate_envp(char **envp);
void				set_env(t_shell *shell, char *key, const char *value);
void				set_shell_level(t_shell *shell);

/* ---- UNSET, EXPORT ------------------------------------------------------- */
int					is_valid_varname(const char *name);

/* ---- EXEC ---------------------------------------------------------------- */
int					exec_builtin(struct s_execcmd *e, t_shell *shell);
int					exec_tree(struct s_command *cmd, t_shell *shell);
int					exec_simple(struct s_execcmd *e, t_shell *shell);
int					exec_pipe(struct s_pipecmd *p, t_shell *shell);
int					exec_redir(struct s_redircmd *r, t_shell *shell);
//int					redirect_stdio(int input_fd, int output_fd);
char				*resolve_cmd_path(char *cmd_name, char **env);
struct s_command	*handle_file_redirection(struct s_command *subcmd,
						char ***tokens, char *op, t_shell *shell);
int					is_redirection_operator(char *token);

/* ---- PARSER -------------------------------------------------------------- */
struct s_command	*parse_cmd_tree(char **tokens, t_shell *shell);
struct s_command	*parse_exec(char ***tokens, t_shell *shell);
struct s_command	*parse_redirs(struct s_command *subcmd,
						char ***tokens, t_shell *shell);
struct s_command	*parse_heredoc(struct s_command *subcmd,
						char ***tokens, t_shell *shell);
char				*get_env_value(t_shell *shell, char *var_name);
char				*expand_var(char **p, t_shell *shell);
//void				expand_argv_vars(char **argv, t_shell *shell);
char				**tokenize_input(char *input, t_shell *shell);
char				*read_unquoted(char **p, t_shell *shell);
char				*read_double(char **p, t_shell *shell);
char				*read_single(char **p, t_shell *shell);
int					handle_escape(char **p, char *buf, int i);
int					handle_single_quote(char **p, char *buf, int i,
						t_shell *shell);
int					handle_double_quote(char **p, char *buf, int i,
						t_shell *shell);

struct s_command	*handle_file_redirection_single(struct s_command *subcmd,
						char *file, char *op);

/* ---- UTILS --------------------------------------------------------------- */
char				*get_current_dir(void);
void				update_pwd(t_shell *shell, char *oldpwd, char *newpwd);
void				err_exit(char *str);
void				free_3d_arr(void ***str);
void				free_2d_arr(void **array);
void				free_array(char **arr);
void				print_perror(t_shell *shell, t_ms_error err,
						const char *detail);
void				free_command_tree(struct s_command *cmd);
void				cleanup(void);
void				free_shell(t_shell *shell);
void				wait_all_children(char ***cmds, int exit_status);
void				print_invalid_identifier(char *arg);
void				sort_and_print_env(t_shell *shell);
void				print_env_export(char **env, int size);
void				free_tokens(char **tokens);
void				show_eof_warning(char *delimiter);
void				setup_heredoc_signals(struct sigaction *sa_int_old);
int					interrumped_flag(int option);
/* ---- PIPE --------------------------------------------------------------- */
int					child_pipe_left(struct s_command *cmd, int *fds,
						t_shell *shell);
int					child_pipe_right(struct s_command *cmd, int *fds,
						t_shell *shell);
int					handle_pipe_exit(int status, t_shell *shell);
void				handle_exit_frees(t_shell *shell, struct s_command *e);
/* ---- HERE_DOC ----------------------------------------------------------- */
struct s_command	*heredoc_cmd(struct s_command *subcmd, char *delim,
						char *content);
char				*read_heredoc_content(char *delimiter);
int					exec_heredoc(struct s_heredoccmd *h, t_shell *shell);
int					is_delimiter(char *line, char *delimiter);
/* ---- FREE TREE ----------------------------------------------------------- */
void				free_exec_command(struct s_execcmd *e);
void				free_redir_command(struct s_redircmd *r);
void				free_pipe_command(struct s_pipecmd *p);
void				free_heredoc_command(struct s_heredoccmd *h);
void				free_list_command(struct s_listcmd *l);

/* ---- COLORS -------------------------------------------------------------- */
// ANSI color codes (readline-safe with \001 and \002)
# define CLR_RESET     "\001\033[0m\002"
# define CLR_BOLD      "\001\033[1m\002"
# define CLR_RED       "\001\033[1;31m\002"
# define CLR_GREEN     "\001\033[1;32m\002"
# define CLR_YELLOW    "\001\033[1;33m\002"
# define CLR_BLUE      "\001\033[1;34m\002"
# define CLR_MAGENTA   "\001\033[1;35m\002"
# define CLR_CYAN      "\001\033[1;36m\002"
# define CLR_WHITE     "\001\033[1;37m\002"

#endif
