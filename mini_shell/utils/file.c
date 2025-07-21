#include "../minishell.h"
int	open_file(char *filename, int flags)
{
	int	fd;

	fd = open(filename,flags);
	if (fd < 0)
		exit_with_error("Error opening file");
	return (fd);
}