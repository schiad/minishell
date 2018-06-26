/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schiad <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 11:36:29 by schiad            #+#    #+#             */
/*   Updated: 2017/05/01 14:23:32 by schiad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>

void	free_exec(char **exec)
{
	int	i;

	i = 0;
	if (exec)
		while (exec[i])
		{
			ft_memdel((void **)&exec[i]);
			i++;
		}
	ft_memdel((void **)&exec);
}

char	**env_value(char **env, char *str)
{
	int		ok;
	char	**tmp;
	char	**ret;
	int		i;
	int		j;

	ret = NULL;
	ok = 0;
	i = 0;
	while (!ok && env[i])
	{
		tmp = ft_strsplit(env[i], '=');
		if (!ft_strcmp(tmp[0], str))
		{
			ok = 1;
			ret = ft_strsplit(tmp[1], ':');
		}
		j = 0;
		while (tmp[j])
			ft_memdel((void **)&tmp[++j]);
		ft_memdel((void **)&tmp);
		i++;
	}
	j = 0;
	return (ret);
}

char	**get_path(char **env)
{
	int		ok;
	char	**tmp;
	char	**ret;
	int		i;
	int		j;

	ret = NULL;
	ok = 0;
	i = 0;
	while (!ok && env[i])
	{
		tmp = ft_strsplit(env[i], '=');
		if (!ft_strcmp(tmp[0], "PATH"))
		{
			ok = 1;
			ret = ft_strsplit(tmp[1], ':');
		}
		j = 0;
		while (tmp[j])
			ft_memdel((void **)&tmp[++j]);
		ft_memdel((void **)&tmp);
		i++;
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

char	**path_cmd(char *cmd, char **env)
{
	char	**path;
	char	**cmdargs;
	char	*pathcmd;
	int		i;

	if (ft_strlen(cmd) == 0)
		return (NULL);
	i = 0;
	path = get_path(env);
	while (path[i])
	{
		cmdargs = ft_strsplit(cmd, ' ');
		if (!access(pathcmd = path_join(path[i], cmdargs[0]), X_OK))
		{
			ft_memdel((void **)&cmdargs[0]);
			cmdargs[0] = pathcmd;
			return (cmdargs);
		}
		ft_memdel((void **)&pathcmd);
		ft_memdel((void **)&path[i]);
		i++;
	}
	return (NULL);
}

int			ft_cd(char **cmdargs)
{
	int	i;

	if (chdir(cmdargs[1]))
	{
		ft_putstr_fd("chdir error\n", 2);
	}
	i = 0;
	while (cmdargs[i])
	{
		ft_memdel((void **)&cmdargs[i]);
		i++;
	}
	ft_memdel((void **)&cmdargs);
	return (0);
}

int			ft_echo(char **cmdargs, char **env)
{
	long	x;
	long	y;

	x = 1;
	while (cmdargs[x])
	{
		y = 0;
		while (cmdargs[x][y])
		{
			ft_putchar(cmdargs[x][y]);
			y++;
		}
		if (cmdargs[x + 1])
			ft_putchar(' ');
		else
			ft_putchar('\n');
		x++;
	}
	env++;
	return (0);
}

int		build_in(char *cmd, char **env)
{
	char	**cmdargs;

	cmdargs = ft_strsplit(cmd, ' ');
	if (!ft_strcmp(cmdargs[0], "cd"))
		if (!ft_cd(cmdargs))
			return (0);
	if (!ft_strcmp(cmdargs[0], "echo"))
		if (!ft_echo(cmdargs, env))
			return (0);
	return (1);
}

int		execute(char *cmd, char **env)
{
	char	**exec;
	pid_t	father;

	if (build_in(cmd, env))
	{
		exec = path_cmd(cmd, env);
		if (exec != NULL)
		{
			father = fork();
			if (father)
			{
				wait(0);
			}
			if (!father)
			{
				execve(exec[0], exec, env);
				exit(0);
			}
			free_exec(exec);
		}
		else
		{
			ft_putstr(cmd);
			ft_putstr(": command not found\n");
		}
	}
	ft_memdel((void**)&cmd);
	return (0);
}

void	tr_cmd(char **cmd, char **env)
{
	long	i;
	char	*text;
	char	*tmp;

	text = NULL;
	if (!cmd)
		return;
	i = 0;
	while (cmd[0][i])
	{
		// 
		if (!text)
		{
			text = ft_strjoin("", &cmd[0][i]);
		}
		else
		{
			tmp = text;
			text = ft_strjoin(text, &cmd[0][i]);
			ft_strdel(&tmp);
		}
		i++;
	}
	env++;
}

int	main(int argc, char **argv, char **environ)
{
	char	*cmd;

	while (1)
	{
		ft_putstr(">$ ");
		if (!get_next_line(0, &cmd))
		{
			ft_putchar('\n');	//If ctrl + D exit
			exit(0);
		}
		tr_cmd(&cmd, environ);
		execute(cmd, environ);
		argc++;					//Remove
		argv++;					//Remove
	}
	return (0);
}
