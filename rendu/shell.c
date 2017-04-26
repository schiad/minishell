#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 1000

void	free_exec(char **exec)
{
	int	i;

	i = 0;
	while (exec[i])
	{
		ft_memdel((void **)&exec[i]);
		i++;
	}
	ft_memdel((void **)&exec);
}

char	**find_path(char **env)
{
	int		ok;
	char	**tmp;
	char	**ret;
	int 	i;
	int		j;

	ret = NULL;
	ok = 0;
	i = 0;
	while (!ok)
	{
		tmp = ft_strsplit(env[i], '=');
		if (!ft_strcmp(tmp[0], "PATH"))
			ok = 1;
		i++;
		if (ok)
			ret = ft_strsplit(tmp[1], ':');
		j = 0;
		while (tmp[j])
			ft_memdel((void **)&tmp[++j]);
		ft_memdel((void **)&tmp);
	}
	j = 0;
	return (ret);
}

char	*path_join(const char *str1, const char *str2)
{
	char	*result;
	int		i;
	int		j;

	result = ft_strnew(ft_strlen(str1) + ft_strlen(str2) + 1);
	i = 0;
	j = 0;
	while (str1[i])
	{
		if (!(str1[i] == '/' && (str1[i + 1] == '/' || str1[i + 1] == '\0')))
			result[j++] = str1[i];
		i++;
	}
	result[j] = '/';
	j++;
	i = 0;
	if (str2[0] == '/')
		j = 0;
	while (str2[i])
	{
		result[j++] = str2[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	**parse_cmd(char *cmd, char **env)
{
	char	**path;
	char	**cmdargs;
	char	*pathcmd;
	int		i;

	if (ft_strchr(cmd, '\n'))
		cmd[(ft_strchr(cmd, '\n') - cmd)] = '\0';
	if (ft_strlen(cmd) == 0)
		return (NULL);
	i = 0;
	path = find_path(env);
	while (path[i])
	{
		cmdargs = ft_strsplit(cmd, ' ');
		pathcmd = path_join(path[i], cmdargs[0]);
		if (!access(pathcmd, X_OK))
		{
			ft_memdel((void **)&cmdargs[0]);
			cmdargs[0] = pathcmd;
			return (cmdargs);
		}
		ft_memdel((void **)&pathcmd);
		i++;
	}
	return (NULL);
}

int	execute(char *cmd, char **env)
{
	char	**exec;
	pid_t	father;

	exec = parse_cmd(cmd, env);
	father = fork();
	if (exec)
	{
		if (father)
		{
			wait(0);
		}
		if (!father)
		{
			//		ft_putstr("\n***---execute cmd ...---***\n");
			execve(exec[0], exec, env);
			//		ft_putstr("***---execve error---***\n");
			exit (0);
		}
	}
	free_exec(exec);
	return (0);
}

int	main(int argc, char **argv, char **environ)
{
	char	buff[BUFF_SIZE];

	while (1)
	{
		ft_memset(buff, '\0', BUFF_SIZE);
		ft_putstr(">$ ");
		read(0, buff, BUFF_SIZE);
		if (!*buff && !ft_strlen(buff))
		{
			ft_putchar('\n');
			exit(0);
		}
		execute(buff, environ);
		argc = 0;
		argv = NULL;
	}
	return (0);
}