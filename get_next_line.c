/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atamas <atamas@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:59:22 by atamas            #+#    #+#             */
/*   Updated: 2024/01/03 14:44:01 by atamas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 20
#endif

int	ft_strlen(const char *string)
{
	int	len;

	len = 0;
	if (!string)
		return (0);
	while (string[len] != '\0')
		len++;
	return (len);
}

char	*create_copy_return(char *string, char *temp)
{
	char	*joined;
	int		len_string;
	int		len_temp;
	int		i;
	int		j;

	len_string = ft_strlen(string);
	len_temp = ft_strlen(temp);
	i = -1;
	j = 0;
	joined = malloc(sizeof(char) * (len_string + len_temp + 1));
	if (!joined)
		return (NULL);
	joined[len_string + len_temp] = '\0';
	while (++i < len_string)
		joined[i] = string[i];
	while (j < len_temp)
		joined[i++] = temp[j++];
	return (free(string), free(temp), string = NULL, temp = NULL, joined);
}

char	*ft_strchr(const char *string, int c)
{
	char	ch;

	ch = (char)c;
	if (!string)
		return (NULL);
	while (*string != '\0')
	{
		if (*string == ch)
		{
			return ((char *)string);
		}
		string++;
	}
	if (ch == '\0')
		return ((char *)string);
	return (NULL);
}

char	*ft_static_handler(char	*static_str, char *string)
{
	int		i;
	int		j;
	char	*memory;

	i = 0;
	j = -1;
	if (!string)
		return (free(static_str), static_str = NULL, NULL);
	i = ft_strlen(string);
	if (i > 0 && string[i - 1] == '\n')
	{
		memory = malloc(sizeof(char) * (ft_strlen(static_str + i) + 1));
		while (static_str[++j + i] != '\0')
			memory[j] = static_str[j + i];
		memory[j] = '\0';
		free(static_str);
		static_str = NULL;
		return (memory);
	}
	else
		return (free(static_str), NULL);
}

char	*ft_copy_until_n(char *string)
{
	int		len;
	int		i;
	char	*copied;

	len = 0;
	i = 0;
	while (string[len] != '\0' && string[len] != '\n')
		len++;
	if (string[len] == '\n')
		len++;
	copied = malloc(sizeof(char) * (len + 1));
	if (!copied)
		return (NULL);
	while (i < len)
	{
		copied[i] = string[i];
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
		return (NULL, my_static = NULL);
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
	// WHat happens when i run the program again??
} */