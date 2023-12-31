/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atamas <atamas@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:59:22 by atamas            #+#    #+#             */
/*   Updated: 2024/01/04 16:05:51 by atamas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
#include "get_next_line.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 20
#endif

char	*create_copy_return(char *my_static, char *temp)
{
	char	*joined;
	int		len_string;
	int		len_temp;
	int		i;
	int		j;

	len_string = ft_strlen(my_static);
	len_temp = ft_strlen(temp);
	i = -1;
	j = 0;
	joined = malloc(sizeof(char) * (len_string + len_temp + 1));
	if (!joined)
		return (NULL);
	joined[len_string + len_temp] = '\0';
	while (++i < len_string)
		joined[i] = my_static[i];
	while (j < len_temp)
		joined[i++] = temp[j++];
	return (free(my_static), free(temp), my_static = NULL, temp = NULL, joined);
}

char	*ft_static_handler(char	*static_str, char *string)
{
	int		i;
	int		j;
	char	*memory;

	i = 0;
	j = 0;
	if (!string)
		return (free(static_str), NULL);
	i = ft_strlen(string);
	if (i > 0 && string[i - 1] == '\n')
	{
		memory = malloc(sizeof(char) * (ft_strlen(static_str + i) + 1));
		if (!memory)
			return (free(static_str), NULL);
		while (static_str[j + i] != '\0')
		{
			memory[j] = static_str[j + i];
			j++;
		}
		memory[j] = '\0';
		return (free(static_str), memory);
	}
	else
		return (free(static_str), NULL);
}

char	*ft_copy_until_n(char *my_static)
{
	int		len;
	int		i;
	char	*copied;

	len = 0;
	i = 0;
	while (my_static[len] != '\0' && my_static[len] != '\n')
		len++;
	if (my_static[len] == '\n')
		len++;
	copied = malloc(sizeof(char) * (len + 1));
	if (!copied)
		return (NULL);
	while (i < len)
	{
		copied[i] = my_static[i];
		i++;
	}
	copied[i] = '\0';
	return (copied);
}

char	*ft_read(int fd, char	*my_static)
{
	char	*temp;
	int		read_val;

	read_val = 1;
	while (ft_strchr(my_static, '\n') == NULL && read_val > 0)
	{
		temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		read_val = read(fd, temp, BUFFER_SIZE);
		temp[read_val] = '\0';
		if (read_val == -1)
			return (free(temp), free(my_static), NULL);
		my_static = create_copy_return(my_static, temp);
	}
	return (my_static);
}

char	*get_next_line(int fd)
{
	char		*line_read;
	static char	*my_static;

	if (read(fd, 0, 0) < 0 || fd < 0 || BUFFER_SIZE <= 0)
	{
		if (my_static)
			free(my_static);
		return (NULL, my_static = NULL);
	}
	my_static = ft_read(fd, my_static);
	if (my_static == NULL)
		return (NULL);
	line_read = ft_copy_until_n(my_static);
	my_static = ft_static_handler(my_static, line_read);
	if (line_read[0] == '\0')
		return (free(line_read), NULL);
	return (line_read);
}

/* int	main(void)
{
	int	fd;
	char	*ptrtoread;

	fd = open("file.txt", O_RDONLY);
	ptrtoread =  get_next_line(fd);
	printf("%s\n", ptrtoread);
	ptrtoread =  get_next_line(fd);
	printf("%s\n", ptrtoread);
	close(fd);
} */