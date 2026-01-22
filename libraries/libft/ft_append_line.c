/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 21:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2025/12/12 21:01:02 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Appends a new line to an existing array of strings (map).
 *
 * Creates a new array with space for one additional string, copies all
 * existing strings from the old array to the new one, and adds the new
 * line at the end. The old array structure is freed, but the strings
 * themselves are preserved (shallow copy).
 *
 * This function is commonly used when reading a map file line by line,
 * dynamically growing the array as new lines are read.
 *
 * If memory allocation fails at any point, the function cleans up and
 * returns NULL to prevent memory leaks.
 *
 * @param map The existing NULL-terminated array of strings. Can be NULL
 *            if this is the first line being added.
 * @param line The new line string to append. Will be duplicated using
 *             ft_strdup before being added to the array.
 * @param count The current number of strings in the map array (excluding
 *              the NULL terminator).
 *
 * @return A new NULL-terminated array containing all previous strings plus
 *         the new line on success, or NULL if memory allocation fails.
 */
char	**ft_append_line(char **map, char *line, int count)
{
	char	**new_map;
	int		i;

	new_map = ft_calloc(count + 2, sizeof(char *));
	if (!new_map)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_map[i] = map[i];
		i++;
	}
	new_map[count] = ft_strdup(line);
	if (!new_map[count])
	{
		free(new_map);
		return (NULL);
	}
	free(map);
	return (new_map);
}
