/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/14 17:05:01 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_pipe_error(t_token *current)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next || current->next->type == TOKEN_PIPE)
		{
			handle_error(ERR_SYNTAX, NULL, "'|'");
			return (TRUE);
		}
	}
	return (FALSE);
}

static int	is_redir_error(t_token *current)
{
	if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND || current->type == TOKEN_REDIR_HEREDOC)
	{
		if (!current->next)
		{
			handle_error(ERR_SYNTAX, NULL, "'newline'");
			return (TRUE);
		}
		if (current->next->type != TOKEN_WORD)
		{
			handle_error(ERR_SYNTAX, NULL, current->next->value);
			return (TRUE);
		}
	}
	return (FALSE);
}

/**
 * @brief Validates the syntax of tokens
 *
 * TODO: Implementar validação de erros comuns:
 * - Pipe no início ou no final
 * - Dois pipes seguidos (| |)
 * - Redirecionamento sem arquivo (< ou > ou >> sem word depois)
 * - Tokens inválidos
 *
 * @param tokens Lista de tokens
 * @return TRUE se válido, FALSE se houver erro
 */
static int	validate_syntax(t_token *tokens)
{
	t_token *current;

	if (tokens && tokens->type == TOKEN_PIPE)
	{
		handle_error(ERR_SYNTAX, NULL, "'|'");
		return (FALSE);
	}
	current = tokens;
	while (current)
	{
		if (is_pipe_error(current) || is_redir_error(current))
			return (FALSE);
		current = current->next;
	}
	return (TRUE);
}

/**
 * @brief Adds an argument to the arguments array
 *
 * Reallocates the args array to fit one more element and adds the new
 * argument while keeping the array NULL-terminated.
 *
 * EXAMPLE:
 * Before: ["ls", NULL]
 * After: ["ls", "-la", NULL]
 *
 * @param cmd Current command
 * @param arg Argument to add
 * @return SUCCESS or ERROR
 */
int	add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		j;

	i = count_args(cmd->args);
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (ERROR);
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (ERROR);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (SUCCESS);
}

static void parse_tokens(t_token *curr, t_cmd **cmd_list, t_cmd **current_cmd)
{
	t_redir	*redir;

    redir = NULL;
	while (curr)
	{
		if (curr->type == TOKEN_WORD)
		{
			if (*current_cmd == NULL)
			{
				*current_cmd = create_cmd_node();
				add_cmd(cmd_list, *current_cmd);
			}
			add_arg_to_cmd(*current_cmd, curr->value);
		}
		else if (curr->type == TOKEN_REDIR_APPEND || curr->type == TOKEN_REDIR_IN
			|| curr->type == TOKEN_REDIR_HEREDOC || curr->type == TOKEN_REDIR_OUT)
		{
			redir = create_redir_node(curr->type, curr->next->value);
			add_redir_to_cmd(*current_cmd, redir);
			curr = curr->next;
		}
		else if (curr && curr->type == TOKEN_PIPE)
			*current_cmd = NULL;
		curr = curr->next;
	}
}

/**
 * @brief Constrói a lista de comandos a partir dos tokens
 *
 * ESTRATÉGIA:
 * 1. Validar sintaxe primeiro
 * 2. Percorrer tokens:
 *    - TOKEN_WORD: adicionar como argumento ao comando atual
 *    - TOKEN_REDIR_*: o próximo WORD é o arquivo, criar t_redir
 *    - TOKEN_PIPE: finalizar comando atual, criar novo
 * 3. Retornar lista ligada de comandos
 *
 * EXEMPLO:
 * Tokens: [WORD:ls] [WORD:-la] [PIPE] [WORD:grep] [WORD:test]
 *
 * Resultado:
 * cmd1: args=["ls", "-la", NULL], next=cmd2
 * cmd2: args=["grep", "test", NULL], next=NULL
 *
 * EXEMPLO COM REDIRECIONAMENTO:
 * Tokens: [WORD:cat] [REDIR_IN] [WORD:file.txt] [PIPE] [WORD:wc]
 *
 * Resultado:
 * cmd1: args=["cat", NULL], redirs=[<file.txt>, next=cmd2
 * cmd2: args=["wc", NULL], redirs=NULL, next=NULL
 *
 * @param tokens Lista de tokens expandidos
 * @return Lista de comandos ou NULL em caso de erro
 */
t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	
	if (!tokens)
		return (NULL);
	if (validate_syntax(tokens) == FALSE)
		return (NULL);
	cmd_list = NULL;
	current_cmd = NULL;
	parse_tokens(tokens, &cmd_list, &current_cmd);
	return (cmd_list);
}
