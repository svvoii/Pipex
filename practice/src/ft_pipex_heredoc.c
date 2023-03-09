#include <../includes/pipex.h>

int			pipex_heredoc(char *delimiter);
int			heredoc(char *delimiter, int fdout);
static int	str_add_to_list(t_list **lst, char *str);
static void	put_list_fd(t_list *lst, int fd);

int			open_infile(char *infile);
int			open_outfile(char *heredoc, char *outfile);

int			check_permission(char *file_name, int mode);
int			check_file_permissions(int ac, char **av);

int	pipex_heredoc(char *delimiter)
{
	int	fd[2];

	if (pipe((int *)fd))
	{
		perror("Pipe: ");
		return (-1);
	}
	if (heredoc(delimiter, fd[1]))
		return (-1);
	return (fd[0]);
}

int	heredoc(char *delimiter, int fdout)
{
	t_list	*lst;
	char	*str;

	lst = NULL;
	delimiter = ft_strjoin(delimiter, "\n");
	if (!delimiter)
		return (1);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		str = get_next_line(0);
		if (!str)
			return (1);
		if (!ft_strncmp(str, delimiter, ft_strlen(str)))
			break ;
		else if (str_add_to_list(&lst, str))
			return (1);
	}
	free(str);
	put_list_fd(lst, fdout);
	close(fdout);
	ft_lstclear(&lst, free);
	free(delimiter);
	return (0);
}

static int	str_add_to_list(t_list **lst, char *str)
{
	t_list	*new;

	new = ft_lstnew((void *)str);
	if (!new)
		return (1);
	if (!*lst)
		*lst = new;
	else
		ft_lstadd_back(lst, new);
	return (0);
}

static void	put_list_fd(t_list *lst, int fd)
{
	int		i;
	t_list	*item;

	i = 0;
	while (1)
	{
		item = ft_lstget_index(lst, i);
		if (!item)
			return ;
		ft_putstr_fd(item->content, fd);
		i++;
	}
}

/* ft_file_utils */
int	open_infile(char *infile)
{
	int	fdin;

	if (ft_strncmp(infile, "here_doc", 9))
	{
		fdin = open(infile, O_RDONLY);
		if (fdin < 0)
		{
			perror(infile);
			return (-1);
		}
		return (fdin);
	}
	return (-2);
}

int	open_outfile(char *heredoc, char *outfile)
{
	int	flags;
	int	fdout;

	flags = O_WRONLY | O_CREAT;
	if (!ft_strncmp(heredoc, "here_doc", 9))
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	fdout = open(outfile, flags, 0644);
	if (fdout < 0)
	{
		perror(outfile);
		return (-1);
	}
	return (fdout);
}

/* ft_check_file_prmission */
int	check_permission(char *file_name, int mode)
{
	if (access(file_name, mode))
	{
		ft_putstr_fd(file_name, 1);
		ft_putstr_fd(": permission denied\n", 1);
		return (1);
	}
	return (0);
}
/*
int	check_permission(char *file_name, int mode)
{
	if (access(file_name, mode))
	{
		ft_printf("%s: permission denied\n", file_name);
		return (1);
	}
	return (0);
}
*/
int	check_file_permissions(int ac, char **av)
{
	if (!ft_strncmp(av[1], "here_doc", 9))
	{
		if (check_permission(av[5], W_OK))
			return (1);
	}
	else
	{
		if (check_permission(av[1], R_OK))
			return (1);
		if (check_permission(av[ac - 1], W_OK))
			return (1);
	}
	return (0);
}

