/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Busca o caminho completo de um comando
 * 
 * TODO (Pessoa B): Implementar
 * - Se cmd contém '/', usar ele diretamente
 * - Senão, buscar PATH na env
 * - Split PATH por ':'
 * - Para cada diretório, testar se existe cmd executável (access)
 * - Retornar o primeiro caminho válido
 * 
 * EXEMPLO:
 * cmd = "ls"
 * PATH = "/usr/bin:/bin"
 * Testar: /usr/bin/ls (OK!) -> retornar "/usr/bin/ls"
 * 
 * @param cmd Nome do comando
 * @param env Lista de ambiente
 * @return Caminho completo ou NULL se não encontrar
 */
char	*find_command_path(char *cmd, t_env *env)
{
	// TODO: Implementar
	(void)cmd;
	(void)env;
	return (NULL);
}
