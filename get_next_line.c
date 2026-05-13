/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernyk <dchernyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 14:22:44 by dchernyk          #+#    #+#             */
/*   Updated: 2026/05/13 16:43:54 by dchernyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_memset(void *p, int c, size_t n)
{
	size_t		i;
	char		*currentp;

	currentp = p;
	i = 0;
	while (i < n)
	{
		currentp[i] = c;
		i++;
	}
	return (p);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)s + i);
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	s1_len;
	unsigned int	s2_len;
	unsigned int	i;
	unsigned int	ri;
	char			*result;

	if (!s1)
		s1_len = 0;
	else
		s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (!s2)
		return (NULL);
	result = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	ri = 0;
	while (i < s1_len)
		result[ri++] = s1[i++];
	i = 0;
	while (i < s2_len)
		result[ri++] = s2[i++];
	result[ri] = '\0';
	return (result);
}

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
		//remember free
		line = ft_strjoin (line, temp);
		free(temp);
		temp = aftern(buffer);
		free(buffer);
		if (temp)
			leftovers = temp;
		else
			leftovers = NULL;
	}
	return (line);
}

int main(void)
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
	// printf("%s", get_next_line(file));
	// printf("%s", get_next_line(file));
	// printf("%s", get_next_line(file));
	// printf("%s", get_next_line(file));
	// printf("%s", get_next_line(file));
}