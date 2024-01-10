/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atamas <atamas@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:59:22 by atamas            #+#    #+#             */
/*   Updated: 2024/01/10 15:50:49 by atamas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
#include "get_next_line_bonus.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 20
#endif

/* int	ft_strlen(const char *string)
{
	int	len;

	len = 0;
	if (!string)
		return (0);
	while (string[len] != '\0')
		len++;
	return (len);
} */

char	*create_copy_return(char *my_static, char *temp, int *myline)
{
	char	*joined;
	int		len_string;
	int		len_temp;
	int		i;
	int		j;

	len_string = ft_strlen(my_static);
	len_temp = ft_strlen(temp);
	*myline = len_string + len_temp;
	i = 0;
	j = 0;
	joined = malloc(sizeof(char) * (len_string + len_temp + 1));
	if (!joined)
		return (NULL);
	joined[len_string + len_temp] = '\0';
	while (i < len_string)
	{
		joined[i] = my_static[i];
		i++;
	}
	while (j < len_temp)
		joined[i++] = temp[j++];
	return (free(my_static), free(temp), my_static = NULL, temp = NULL, joined);
}

char	*ft_static_handler(char	*static_str, char *string, int nline_pos)
{
	int		i;
	int		j;
	char	*memory;

	i = 0;
	j = 0;
	if (!string)
		return (free(static_str), NULL);
	i = nline_pos + 1;
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

char	*ft_copy_until_n(char *my_static, int nline_pos, int myline)
{
	int		len;
	int		i;
	char	*copied;

	len = 0;
	i = 0;
	if (nline_pos == -1)
	{
		if (!my_static[0])
			return (NULL);
		len = myline;
	}
	else
		len = nline_pos + 1;
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

char	*ft_read(int fd, char	*my_static, int *nline_pos, int *myline)
{
	char	*temp;
	int		read_val;

	read_val = 1;
	while (ft_strchr(my_static, '\n', nline_pos) == NULL && read_val > 0)
	{
		temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		read_val = read(fd, temp, BUFFER_SIZE);
		if (read_val == -1)
			return (free(temp), free(my_static), NULL);
		temp[read_val] = '\0';
		my_static = create_copy_return(my_static, temp, myline);
	}
	return (my_static);
}

char	*get_next_line(int fd)
{
	char		*line_read;
	int			nline_pos;
	int			myline;
	static char	*my_static[4096];

	nline_pos = -1;
	myline = 0;
	if (read(fd, 0, 0) < 0 || fd < 0 || BUFFER_SIZE <= 0)
	{
		if (my_static[fd])
			free(my_static[fd]);
		return (NULL, my_static[fd] = NULL);
	}
	my_static[fd] = ft_read(fd, my_static[fd], &nline_pos, &myline);
	if (my_static[fd] == NULL)
		return (NULL);
	line_read = ft_copy_until_n(my_static[fd], nline_pos, myline);
	my_static[fd] = ft_static_handler(my_static[fd], line_read, nline_pos);
	return (line_read);
}

/* int	main(void)
{
	int	fd;
	char	*ptrtoread;

	fd = open("file.txt", O_RDONLY);
	ptrtoread =  get_next_line(fd);
	printf("%s\n", ptrtoread);
	free(ptrtoread);
	ptrtoread =  get_next_line(fd);
	printf("%s\n", ptrtoread);
	free(ptrtoread);
	close(fd);
} */