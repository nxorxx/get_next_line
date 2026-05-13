/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernyk <dchernyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 14:22:44 by dchernyk          #+#    #+#             */
/*   Updated: 2026/05/13 16:41:33 by dchernyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*beforen(char *buffer)
{
	int		n_pos;
	int		i;
	char	*result;

	if (!ft_strchr(buffer, 10))
		return (NULL);
	n_pos = 0;
	while (buffer[n_pos] != 10)
		n_pos++;
	result = malloc(n_pos + 2);
	if (!result)
		return (NULL);
	result[n_pos + 1] = '\0';
	result[n_pos] = '\n';
	i = 0;
	while (i < n_pos)
	{
		result[i] = buffer[i];
		i++;	
	}
	return (result);
}

char	*aftern(char *buffer)
{
	int		n_pos;
	int		end_pos;
	int		i;
	char	*result;

	if (!ft_strchr(buffer, 10))
		return (NULL);
	n_pos = 0;
	while (buffer[n_pos] != 10)
		n_pos++;
	if (buffer[n_pos + 1] == '\0')
		return (NULL);
	end_pos = ft_strlen(buffer);
	result = malloc(end_pos - n_pos + 1);
	if (!result)
		return (NULL);
	result[end_pos - n_pos - 1] = '\0';
	i = 0;
	while (i + n_pos < end_pos)
	{
		result[i] = buffer[i + n_pos + 1];
		i++;	
	}
	return (result);
}

int gnl_logic(int fd, char **line, char *buffer)
{
	int		bytes;
	char	*temp;
	while (1)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (0);
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		if (ft_strchr(buffer, 10))
			break ;
		temp = ft_strjoin(*line, buffer);
		free(*line);
		*line = temp;
	}
	return (bytes);
}

char	*get_next_line(int fd)
{
	static char	*leftovers;
	char		*temp;
	char		*line;
	char		buffer[BUFFER_SIZE + 1];

	line = NULL;
	if (leftovers && ft_strchr(leftovers, 10))
	{
		line = beforen(leftovers);
		temp = aftern(leftovers);
		free(leftovers);
		leftovers = temp;
		return (line);
	}
	else if (leftovers)
	{
		line = ft_strjoin(line, leftovers);
		free(leftovers);
		leftovers = NULL;
	}
	gnl_logic(fd, &line, buffer);
	if (ft_strchr(buffer, 10))
	{
		temp = beforen(buffer);
		free(line);
		line = ft_strjoin (line, temp);
		free(temp);
		temp = aftern(buffer);
		if (temp)
			leftovers = temp;
		else
			leftovers = NULL;
		temp = NULL;
	}
	return (line);
}

/*int main(void)
{
	char *c;
	int			file;

	file = open("multiple_nl.txt", O_RDWR);
	if (file == -1)
		printf("\nNe dala nihuia");
	while (1)
	{
		c = get_next_line(file);
		if(!c)
			break;
		printf("%s", c);
	}
}*/