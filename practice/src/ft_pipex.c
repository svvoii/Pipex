#include <../includes/pipex.h>

int			error(char *str);
int			pipex(int ac, char **av, char **envp);
static int	exec_pipex(char **cmds, int cmdn, char **envp, int *fdin_out);
static void	wait_pids(void *content);
static int	pid_list_add(t_list **pid_lst, int pid);
static int	next_cmd(char *cmd, char **envp, int fdout, int fdin);
int			create_pipe(int *fd);
void		print_fd_read(int fd);

int	main(int ac, char **av, char **envp)
{
	int		err;

	err = 0;
	if (ac >= 5)
		err += pipex(ac, av, envp);
	else
		err += error("Not enough arguments\n");
	if (err)
		return (1);
	else
		return (0);
}

int	error(char *str)
{
	ft_putstr_fd(str, 2);
	return (1);
}

/* ft_pipex */
int	pipex(int ac, char **av, char **envp)
{
	int	fdin_out[2];
	int	i;

	i = 1;
	fdin_out[1] = open_infile(av[1]);
	fdin_out[0] = open_outfile(av[1], av[ac - 1]);
	if (fdin_out[1] == -1 || fdin_out[0] == -1)
		return (1);
	else if (fdin_out[1] == -2)
	{
		fdin_out[1] = pipex_heredoc(av[2]);
		i++;
	}
	if (exec_pipex(&av[i + 1], ac - i - 2, envp, fdin_out))
		return (1);
	return (0);
}

static int	exec_pipex(char **cmds, int cmdn, char **envp, int *fdin_out)
{
	int		i;
	int		fdin;
	int		fdout;
	int		fd[2];
	t_list	*pid;

	pid = 0;
	i = -1;
	fdin = fdin_out[1];
	while (++i < cmdn)
	{
		if (i < cmdn - 1 && create_pipe(fd))
			return (1);
		if (i == cmdn - 1)
			fdout = fdin_out[0];
		else
			fdout = fd[1];
		if (pid_list_add(&pid, next_cmd(cmds[i], envp, fdout, fdin)))
			return (1);
		fdin = fd[0];
	}
	ft_lstiter(pid, wait_pids);
	ft_lstclear(&pid, free);
	return (0);
}

static void	wait_pids(void *content)
{
	waitpid(*(int *)content, NULL, 0);
}

static int	pid_list_add(t_list **pid_lst, int pid)
{
	int		*content;
	t_list	*new;

	content = ft_calloc(1, sizeof(int));
	if (!content)
		return (1);
	*content = pid;
	new = ft_lstnew(content);
	if (!new)
		return (1);
	if (!*pid_lst)
		*pid_lst = new;
	else
		ft_lstadd_back(pid_lst, new);
	return (0);
}

static int	next_cmd(char *cmd, char **envp, int fdout, int fdin)
{
	int	fork_res;

	fork_res = fork();
	if (fork_res < 0)
	{
		perror("Fork: ");
		return (-1);
	}
	else if (!fork_res)
	{
		exec_command(cmd, envp, fdout, fdin);
		exit (0);
	}
	close(fdin);
	close(fdout);
	return (fork_res);
}

/* ft_pipex_utils */
int	create_pipe(int *fd)
{
	if (pipe((int *)fd))
	{
		perror("Pipe: ");
		return (1);
	}
	return (0);
}

void	print_fd_read(int fd)
{
	int		n;
	char	*buff[16];

	n = 15;
	while (n >= 15)
	{
		ft_bzero(buff, 16);
		n = read(fd, buff, 15);
		//ft_putstr_fd(buff, 1);
		printf("%s", *buff);
	}
}
