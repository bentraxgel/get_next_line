/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seok <seok@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 02:12:45 by seok              #+#    #+#             */
/*   Updated: 2023/02/18 03:22:28 by seok             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "get_next_line.h"

t_list	*my_lst_find(t_list **head, int f_fd)
{
	t_list	*temp;

	temp = *head;
	while (temp)
	{
		if (temp->fd == f_fd)
			return (temp);
		temp = temp->next;
	}
	while ((*head)->next)
		(*head) = (*head)->next;
	temp = malloc(sizeof(t_list));
	if (!temp)
		return (NULL);
	(*head)->next = temp;
	temp->save = NULL;
	temp->next = NULL;
	temp->fd = f_fd;
	return (temp);
}

void	my_lst_free(t_list *find, t_list *head)
{
	if (find == NULL) //TODO : 필요한 예외처리인가?
		return ;
	while (head->next != NULL && head->next != find)
		head = head->next;
	if (head->next == NULL)
		return ;
	if (find->save != NULL)
		free(find->save);
	head->next = find->next;
	find->next = NULL;
	free(find);
}

void	my_check_zero(t_list *head, t_list *find, int check, char **ret)
{
	if (check < 0)
	{
		my_lst_free(find, head);
		ret = NULL;
	}
	else if (check == 0 && find->save != 0)
	{
		*ret = ft_substr(find->save, 0, ft_strlen(find->save));
		my_lst_free(find, head);
	}
	else if (check == 0 && find->save == 0)
	{
		my_lst_free(find, head);
		ret = NULL;
	}
	return ;
}

char	*my_save_buf(t_list *find, t_list *head, char *ret)
{
	char		*temp;
	int			check;
	int			idx;

	while (ret == 0)
	{
		ft_memset(find->buf, 0, BUFFER_SIZE + 1);
		check = read(find->fd, find->buf, BUFFER_SIZE);
		find->save = ft_strjoin(find->save, find->buf);
		idx = -1;
		while (find->save[++idx])
		{
			if (find->save[idx] == '\n')
			{
				ret = ft_substr(find->save, 0, idx + 1);
				temp = ft_substr(find->save, idx + 1, \
								ft_strlen(find->save) - (idx + 1));
				free(find->save);
				find->save = temp;
				return (ret);
			}
		}
		my_check_zero(head, find, check, &ret);
	}
	return (ret);
}

char	*get_next_line(int fd)
{
	static t_list	*head;
	t_list			*find;
	char			*ret;

	ret = 0;
	if (BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (0);
	if (head == NULL)
	{
		head = malloc(sizeof(t_list));
		if (!head)
			return (NULL);
		head->fd = -1;
		head->next = NULL;
	}
	find = my_lst_find(&head, fd);
	ret = my_save_buf(find, head, ret);
	if (head && head->next == NULL)
	{
		free(head);
		head = NULL;
	}
	return (ret);
}