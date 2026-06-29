/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdos-s <kimdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:24:03 by kimdos-s          #+#    #+#             */
/*   Updated: 2026/06/25 22:53:00 by kimdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char **stash)
{
	char	*newline;
	char	*result;
	char	*rest;

	newline = ft_strchr(*stash, '\n');
	if (!newline)
	{
		result = ft_strdup(*stash);
		free(*stash);
		*stash = NULL;
		return (result);
	}
	result = ft_substr(*stash, newline - *stash + 1);
	rest = ft_strdup(newline + 1);
	free(*stash);
	if (rest && rest[0] == '\0')
	{
		free(rest);
		rest = NULL;
	}
	*stash = rest;
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes = 1;
	while (!ft_strchr(stash, '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free(buffer), free(stash), stash = NULL, NULL);
		buffer[bytes] = '\0';
		stash = ft_strjoin(stash, buffer);
		if (!stash)
			return (free(buffer), NULL);
	}
	free(buffer);
	if (!stash || !stash[0])
		return (free(stash), stash = NULL, NULL);
	return (extract_line(&stash));
}

/* int	main(void)
{
	int	fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
} */