#include "get_next_line.h"
#include <stdio.h> // TODO

size_t	ft_strlen(const char *s)
{
	int	idx;

	idx = 0;
	if (s == NULL)
		return (0);
	while (s[idx])
		idx++;
	return ((size_t) idx);
}

void	*ft_memset(void *str, int value, size_t len)
{
	while (len--)
		*(unsigned char *)(str + len) = (unsigned char)value;
	return (str);
}

char	*ft_strdup(const char *s1)
{
	char	*ret;
	int		idx;

	idx = 0;
	while (s1[idx])
	{
		idx++;
	}
	ret = (char *)malloc(sizeof(char) * (idx + 1));
	if (!ret)
		return (0);
	idx = 0;
	while (s1[idx])
	{
		ret[idx] = s1[idx];
		idx++;
	}
	ret[idx] = 0;
	return (ret);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	sstart;
	size_t	i;
	size_t	s_len;

	i = 0;
	sstart = (size_t)start;
	s_len = ft_strlen(s);
	if (s_len <= sstart)
		return (ft_strdup(""));
	if (s_len - start <= len)
		str = (char *)malloc(sizeof(char) * (s_len - start + 1));
	if (s_len - start > len)
		str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	while (s[sstart + i] && i < len)
	{
		str[i] = s[sstart + i];
		i++;
	}
	str[i] = 0;
	return (str);
}

void	*ft_memcpy(void *dest, const void *src, size_t len)
{
	char	*adr;

	adr = dest;
	if (dest == src)
		return (dest);
	while (len--)
	{
		*(char *)dest++ = *(char *)src++;
	}
	dest = adr;
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		s1_len;
	int		s2_len;
	char	*ret;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ret = (char *)malloc(s1_len + s2_len + 1);
	if (!ret)
		return (0);
	ft_memcpy(ret, s1, s1_len);
	ft_memcpy(ret + s1_len, s2, s2_len);
	ret[s1_len + s2_len] = 0;
	free(s1);
	return (ret);
}

t_list	*my_lst_find(t_list **head, int f_fd)
{
	t_list	*temp;

	// if (*head == 0)
	// {
	// 	*head = malloc(sizeof(t_list));
	// 	(*head)->fd = -1;
	// 	(*head)->next = NULL;
	// 	// return (*head);
	// }
	// if (*head == 0)
	// {
	// 	temp = malloc(sizeof(t_list));
	// 	temp->fd = f_fd;
	// 	temp->next = NULL;
	// 	*head = temp;
	// 	return (temp);
	// }
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

// void	my_lst_free(t_list *find, t_list *head)
// {
// 	// head = head + 0;
// 	// find = find+0;
// 	//TODO : check while(!=)
// 	//need head leak check
// 	// if (head->next == find)
// 	while(head->next != find)
// 		head = head->next;
// 	find->fd = 0;
// 	head->next = find->next;
// 	find->next = NULL;
// 	free(find);
// }
void my_lst_free(t_list *find, t_list *head)
{
    while (head->next != NULL && head->next != find)
        head = head->next;

    if (head->next == NULL)
        return ;

    head->next = find->next;
    find->next = NULL;
    free(find);
}

char	*my_save_buf(t_list *find, t_list *head)
{
	char		*ret;
	int			check;
	int			idx;

	ret = 0;
	//idx = -1;
	while (ret == 0)
	{
		ft_memset(find->buf, 0, BUFFER_SIZE + 1);
		check = read(find->fd, find->buf, BUFFER_SIZE);
		if (check < 0)
		{
			my_lst_free(find, head);
			return (NULL);
		}
		else if (check == 0 && find->save != 0)
		{
			ret = ft_substr(find->save, 0, ft_strlen(find->save));
			my_lst_free(find, head);
			return (ret);
		}
		else if (check == 0 && find->save == 0)
		{
			// ret = ft_strdup("");
			my_lst_free(find, head);
			return (NULL);
		}
		else
		{
			find->save = ft_strjoin(find->save, find->buf);
			idx = -1;
			while (find->save[++idx])
			{
				if (find->save[idx] == '\n')
				{
					ret = ft_substr(find->save, 0, idx + 1);
					find->save = ft_substr(find->save, idx + 1,\
								 ft_strlen(find->save) - (idx + 1));
					// printf("save: %s\n", find->save);
					// printf("buff: %s\n", find->buf);
					// printf("ret: %s\n", ret);
					return (ret);
				}
			}
		}
	}
	return (ret);
}
char	*get_next_line(int fd)
{

	static t_list	*head;
	t_list			*find;
	char			*ret;

	if (BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (0);
	//printf("before : %s, %p",lst->save, lst);
	//TODO
	if (head == NULL)
	{
		head = malloc(sizeof(t_list));
		if (!head)
			return (NULL);
		head->fd = -1;
		head->next = NULL;
	}
	find = my_lst_find(&head, fd);

	// printf("save : %s\n", find->save);
//	printf("before : %d, %p",lst->fd, lst);
	ret = my_save_buf(find, head);
	if (head->next == NULL)
	{
		free(head);
		head = NULL;
	}
	return (ret);
}

// void	leak_check()
// {
// 	system("leaks a.out");
// }

// #include <errno.h>
// #include <string.h>

// extern int errno;
			
// int	main(void)
// {
// 	int		idx;
// 	int		fd;
// 	char	*str;

// 	// atexit(leak_check);
// 	idx = 1;
// 	fd = open("text.txt", O_RDWR);
// 	while (1)
// 	{
// 		str = get_next_line(fd);
// 		printf("%d: <%s\n", idx, str);
// 		idx++;
// 		if (!str)
// 		{
// 			free(str);
// 			break ;
// 		}
// 		free(str);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	main()
// {
// 	int	fd;
// 	char *ret;

// 	//ret = 0;
// 	atexit(leak_check);
// 	fd = open("text.txt", O_RDONLY);
// 	printf("===main_print===\n");
// 	ret = get_next_line(fd);
// 	printf("1 ret : %s\n", ret);
// 	printf("---------------------------------------\n");
// 	ret = get_next_line(fd);
// 	printf("2 ret : %s\n", ret);
// 	printf("---------------------------------------\n");
// 	ret = get_next_line(fd);
// 	printf("3 ret : %s\n", ret);
// 	printf("---------------------------------------\n");
// 	ret = get_next_line(fd);
// 	printf("4 ret : %s\n", ret);
// 	free(ret);
// }