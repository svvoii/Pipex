#include <../includes/pipex.h>

int			exec_command(char *cmd, char **envp, int fdout, int fdin);
static int	dup_fd_io(int fdout, int fdin);
static void	command_not_found_clear_print(char *cmd, void *cmdargs, void *env_path);

t_env		parse_env_path(char **envp);
static char	**parse_env_path_add_slash(char **env_path);
void		free_word_list(char **env_path);

int	exec_command(char *cmd, char **envp, int fdout, int fdin)
{
	int		i;
	t_env	env;
	char	*pathname;
	char	**cmdargs;

	env = parse_env_path(envp);
	i = -2;
	cmdargs = ft_split(cmd, ' ');
	if (!cmdargs || !env.env_path || dup_fd_io(fdout, fdin))
		return (1);
	while (++i == -1 || env.env_path[i])
	{
		if (i == -1)
			pathname = cmd;
		else
			pathname = ft_strjoin(env.env_path[i], cmdargs[0]);
		if (!pathname)
			return (1);
		execve(pathname, cmdargs, env.envp);
		if (i != -1)
			free(pathname);
	}
	command_not_found_clear_print(cmd, cmdargs, env.env_path);
	return (0);
}

static int	dup_fd_io(int fdout, int fdin)
{
	if (fdout >= 0)
	{
		if (dup2(fdout, 1) == -1)
			return (1);
	}
	if (dup2(fdin, 0) == -1)
		return (1);
	return (0);
}

static void	command_not_found_clear_print(char *cmd, void *cmdargs,
		void *env_path)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found...\n", 2);
	free_word_list(cmdargs);
	free_word_list(env_path);
}

/* ft_env_path */

t_env	parse_env_path(char **envp)
{
	int		i;
	char	**env_path;
	t_env	env;

	env.envp = envp;
	env.env_path = 0;
	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5) == envp[i])
		{
			env_path = ft_split(&envp[i][5], ':');
			if (!env_path)
				return (env);
			env.env_path = parse_env_path_add_slash(env_path);
			return (env);
		}
	}
	return (env);
}

static char	**parse_env_path_add_slash(char **env_path)
{
	int		i;
	char	*tmp;

	i = -1;
	while (env_path[++i])
	{
		tmp = env_path[i];
		env_path[i] = ft_strjoin(tmp, "/");
		free(tmp);
	}
	return (env_path);
}

void	free_word_list(char **env_path)
{
	int	i;

	i = -1;
	while (env_path[++i])
		free(env_path[i]);
	free(env_path);
}
