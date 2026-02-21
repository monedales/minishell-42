/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/19 22:02:04 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libraries/libft/libft.h"

/* ========================================================================== */
/*                                  DEFINES                                   */
/* ========================================================================== */

# define SUCCESS 0
# define ERROR -1
# define TRUE 1
# define FALSE 0

/* ========================================================================== */
/*                                   ENUMS                                    */
/* ========================================================================== */

// Token types
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TKN_REDIR_IN,			// <
	TKN_REDIR_OUT,		// >
	TKN_REDIR_APPEND,		// >>
	TKN_REDIR_HEREDOC,	// <<
	TOKEN_EOF
}	t_token_type;

// Quote states
typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_state;

// Error types
typedef enum e_error
{
	ERR_NONE,
	ERR_SYNTAX,
	ERR_CMD_NOT_FOUND,
	ERR_NO_FILE,
	ERR_PERMISSION,
	ERR_MALLOC,
	ERR_TOO_MANY_ARGS,
	ERR_NUM_REQUIRED,
	ERR_NOT_VALID_ID,
	ERR_HOME_NOT_SET,
	ERR_OLDPWD_NOT_SET
}	t_error;

/* ========================================================================== */
/*                                 STRUCTURES                                 */
/* ========================================================================== */

// Environment variable node
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;


// Token node
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// Redirection node
typedef struct s_redir
{
	t_token_type	type;		// tipo de redirecionamento
	char			*file;		// nome do arquivo
	struct s_redir	*next;
}	t_redir;

// Command node (cada comando da pipeline)
typedef struct s_cmd
{
	char			**args;		// comando + argumentos [cmd, arg1, arg2, NULL]
	t_redir			*redirs;	// lista de redirecionamentos
	pid_t			pid;
	struct s_cmd	*next;		// próximo comando (após pipe)
}	t_cmd;

// Main minishell structure
typedef struct s_mini
{
	t_env	*env;				// lista ligada de variáveis de ambiente
	t_cmd	*cmd_list;			// lista de comandos (separados por pipe)
	int		last_exit_status;	// status do último comando ($?)
	int		running;			// flag para manter o loop ativo
}	t_mini;

/* ========================================================================== */
/*                           PARSING (Pessoa A)                               */
/* ========================================================================== */

// Lexer - Tokenização
t_token		*lexer(char *input);
void		free_tokens(t_token *tokens);
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **head, t_token *new_token);

// Expander - Expansão de variáveis
void		expand_tokens(t_token *tokens, t_mini *mini);

// Parser - Construção da árvore de comandos
t_cmd		*parser(t_token *tokens);
t_cmd		*create_cmd_node(void);
void		add_cmd(t_cmd **head, t_cmd *new);
t_redir		*create_redir_node(t_token_type type, char *file);
t_redir		*create_redir_node(t_token_type type, char *file);
int			count_args(char **args);
void		add_redir_to_cmd(t_cmd *cmd, t_redir *redir);
void		free_redirs(t_redir *redirs);
void		free_cmd_list(t_cmd *cmd_list);

// Quote handling
char		*remove_quotes(char *str);
int			is_in_quotes(char *str, int pos, t_quote_state *state);
int			validate_quotes(char *str);

/* ========================================================================== */
/*                           ENVIRONMENT (Pessoa A)                           */
/* ========================================================================== */

t_env		*init_env(char **envp);
char		*get_env_value(t_env *env, const char *key);
int			set_env_value(t_env **env, const char *key, const char *value);
int			unset_env_value(t_env **env, const char *key);
void		free_env(t_env *env);
char		**env_to_array(t_env *env);
void		add_env_node(t_env **head, t_env *new);
t_env		*create_env_node(const char *key, const char *value);

/* ========================================================================== */
/*                          EXECUTION (Pessoa B)                              */
/* ========================================================================== */

int			execute_cmd_list(t_cmd *cmd_list, t_mini *mini);
int			execute_simple_cmd(t_cmd *cmd, t_mini *mini);
int			execute_pipeline(t_cmd *cmd_list, t_mini *mini);
char		*find_command_path(char *cmd, t_env *env);
int			setup_redirections(t_redir *redirs);
void		print_env(t_env *env);


/* ========================================================================== */
/*                           BUILTINS (Pessoa B)                              */
/* ========================================================================== */

int			is_builtin(char *cmd);
int			execute_builtin(t_cmd *cmd, t_mini *mini);

int			builtin_echo(char **args);
int			builtin_cd(char **args, t_mini *mini);
int			builtin_pwd(void);
int			builtin_export(char **args, t_mini *mini);
int			builtin_unset(char **args, t_mini *mini);
int			builtin_env(t_mini *mini);
int			builtin_exit(char **args, t_mini *mini);

/* ========================================================================== */
/*                             SIGNALS (Pessoa B)                             */
/* ========================================================================== */

void		setup_signals(void);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);

/* ========================================================================== */
/*                                  UTILS                                     */
/* ========================================================================== */

// Utils customizados (libft já tem strlen, strdup, strjoin, strcmp, split, calloc)
void		free_split(char **split);
void		safe_free(void **ptr);

// Error handling
int			handle_error(t_error error, char *cmd, char *detail);
void		exit_error(t_error error, char *cmd, char *detail, int code);

#endif
