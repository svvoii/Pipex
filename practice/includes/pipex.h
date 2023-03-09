
#ifndef PIPEX_H
# define PIPEX_H

// TEST LIBRARIES
# include <sys/stat.h>
// END

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_env
{
	char	**envp;
	char	**env_path;
}	t_env;

/* ft_pipex.c */
int			error(char *str);
int			pipex(int ac, char **av, char **envp);
//static int	exec_pipex(char **cmds, int cmdn, char **envp, int *fdin_out);
//static void	wait_pids(void *content);
//static int	pid_list_add(t_list **pid_lst, int pid);
//static int	next_cmd(char *cmd, char **envp, int fdout, int fdin);
int			create_pipe(int *fd);
void		print_fd_read(int fd);
/* ft_pipex_heredoc.c */
int			pipex_heredoc(char *delimiter);
int			heredoc(char *delimiter, int fdout);
//static int	str_add_to_list(t_list **lst, char *str);
//static void	put_list_fd(t_list *lst, int fd);
int			open_infile(char *infile);
int			open_outfile(char *heredoc, char *outfile);
int			check_permission(char *file_name, int mode);
int			check_file_permissions(int ac, char **av);
/* ft_execution.c */
int			exec_command(char *cmd, char **envp, int fdout, int fdin);
//static int	dup_fd_io(int fdout, int fdin);
//static void	command_not_found_clear_print(char *cmd, void *cmdargs, void *env_path);
t_env		parse_env_path(char **envp);
//static char	**parse_env_path_add_slash(char **env_path);
void		free_word_list(char **env_path);
/* ft_libft.c */
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstnew(void *content);
t_list	*ft_lstget_index(t_list *lst, int index);

char	**ft_split(char const *s, char c);
char	*get_next_line(int fd);

#endif