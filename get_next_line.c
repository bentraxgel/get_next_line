/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seok <seok@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:31:52 by seok              #+#    #+#             */
/*   Updated: 2023/01/20 02:20:59 by seok             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*my_save_buf(char *buf, int check)
{
	static char	*save;
	int			find;
	char		*ret;

	ret = save;
	save = ft_strjoin(save, buf);
	free(ret);
	while (save[find])
	{
		if (save[find] == '\n')
		{
			ret = malloc(find + 2);
		}
	}
}

char	*get_next_line(int fd)
{
	char	buf[BUFFER_SIZE];
	int		check;

	check = 0;
	while (check >= 0)
	{
		ft_memset(buf, 0, BUFFER_SIZE);
		check = read(fd, buf, BUFFER_SIZE);
		my_save_buf(buf, check);
	}
}