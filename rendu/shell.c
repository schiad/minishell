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

int	execute(char *cmd)
{
	char	**exec;
	pid_t	father;

	if (ft_strchr(cmd, '\n'))
		cmd[(ft_strchr(cmd, '\n') - cmd)] = '\0';
	if (!ft_strlen(cmd))
		return (-1);
	exec = ft_strsplit(cmd, ' ');
	father = fork();
	if (father)
	{
		wait(0);
	}
	if (!father)
	{
		//		ft_putstr("\n***---execute cmd ...---***\n");
		execve(exec[0], exec, NULL);
		//		ft_putstr("***---execve error---***\n");
		exit (0);
	}
	free_exec(exec);
	return (0);
}

int	main()
{
	char	buff[BUFF_SIZE];

	while (1)
	{
		ft_memset(buff, '\0', BUFF_SIZE);
		ft_putstr(">$ ");
		read(0, buff, BUFF_SIZE);
		if (!*buff)
		{
			ft_putchar('\n');
			exit(0);
		}
		execute(buff);
	}
}
