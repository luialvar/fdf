/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:39:29 by luialvar          #+#    #+#             */
/*   Updated: 2024/10/19 10:12:02 by luialvar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	size_t	i;
	char	*dinamic;

	len = ft_strlen(s1) + 1;
	i = 0;
	dinamic = (char *)malloc (len);
	if (!dinamic)
		return (NULL);
	while (i < len)
	{
		dinamic [i] = s1 [i];
		i++;
	}
	return (dinamic);
}

static char	*ft_cpyfile_to_backup(int fd, char *buf, char *backup)
{
	int		read_line;
	char	*temp;

	while (1)
	{
		read_line = read(fd, buf, BUFFER_SIZE);
		if (read_line == -1)
			return (0);
		else if (read_line == 0)
			break ;
		buf[read_line] = '\0';
		if (!backup)
			backup = ft_strdup("");
		temp = backup;
		backup = ft_strjoin(temp, buf);
		free(temp);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (backup);
}

static char	*ft_extract_bytes_after_newline(char *line)
{
	size_t	count;
	char	*backup;

	count = 0;
	while (line[count] != '\n' && line[count] != '\0')
		count++;
	if (line[count] == '\0')
		return (NULL);
	backup = ft_substr(line, count + 1, ft_strlen(line) - count - 1);
	if (backup[0] == '\0')
	{
		free(backup);
		backup = NULL;
	}
	line[count + 1] = '\0';
	return (backup);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup[1024];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (0);
	line = ft_cpyfile_to_backup(fd, buffer, backup[fd]);
	free(buffer);
	if (!line)
	{
		free(backup[fd]);
		backup[fd] = NULL;
		return (0);
	}
	backup[fd] = ft_extract_bytes_after_newline(line);
	if (backup[fd] == NULL && (line == NULL || line[0] == '\0'))
	{
		free(line);
		line = NULL;
		return (NULL);
	}
	return (line);
}
