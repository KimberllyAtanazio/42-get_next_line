/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdos-s <kimdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 20:42:34 by kimdos-s          #+#    #+#             */
/*   Updated: 2026/06/29 13:24:52 by kimdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
	static char	*stash[OPEN_MAX];
	char		*buffer;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= OPEN_MAX)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes = 1;
	while (!ft_strchr(stash[fd], '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free(buffer), free(stash[fd]), stash[fd] = NULL, NULL);
		buffer[bytes] = '\0';
		stash[fd] = ft_strjoin(stash[fd], buffer);
		if (!stash[fd])
			return (free(buffer), NULL);
	}
	free(buffer);
	if (!stash[fd] || !stash[fd][0])
		return (free(stash[fd]), stash[fd] = NULL, NULL);
	return (extract_line(&stash[fd]));
}

int	main(void)
{
	int	fd1;
	int	fd2;
	char	*line1;
	char	*line2;

	fd1 = open("test.txt", O_RDONLY);
	fd2 = open("test2.txt", O_RDONLY);
	while ((line1 = get_next_line(fd1)) != NULL 
			&& (line2 = get_next_line(fd2)) != NULL)
	{
		printf("%s", line1);
		printf("%s", line2);
		free(line1);
		free(line2);
	}
	close(fd1);
	close(fd2);
	return (0);
}