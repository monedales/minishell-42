/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// int	ft_strncmp(const char *s1, const char *s2, size_t num)
// {
// 	unsigned char	*ps1;
// 	unsigned char	*ps2;
// 	size_t			count;

// 	count = 0;
// 	ps1 = (unsigned char *)s1;
// 	ps2 = (unsigned char *)s2;
// 	while (count < num && (ps1[count] || ps2[count]))
// 	{
// 		if (ps1[count] != ps2[count])
// 			return (ps1[count] - ps2[count]);
// 		count++;
// 	}
// 	return (0);
// }

// size_t	ft_strlen(const char *str)
// {
// 	size_t	len;

// 	len = 0;
// 	while (str[len])
// 		len++;
// 	return (len);
// }


// void	ft_putstr_fd(char *str, int fd)
// {
// 	size_t	len;

// 	len = ft_strlen(str);
// 	write(fd, str, len);
// }

int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;
		
	n_flag = 0;
	i = 1;

	while (args[i] &&  ft_strncmp(args[i], "-n", 2) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		if (n_flag == 0)
		{
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

// int main()
// {
// 	// char	*args[] = {"echo", "-n", "texto 1", NULL};
// 	char	*args[] = {"echo", NULL};
// 	builtin_echo(args);
// 	return 0;
// }
