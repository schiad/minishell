#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv, char **environ)
{
	int i;
	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}
