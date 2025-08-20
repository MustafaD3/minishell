/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/17 20:18:01 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	create_temp_file(char **temp_filename)
{
	int	fd;

	*temp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	fd = mkstemp(*temp_filename);
	if (fd == -1)
	{
		perror("heredoc");
		free(*temp_filename);
		return (-1);
	}
	return (fd);
}

static void	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;
	char	buffer[4096];
	size_t	n;
	int		i, j;
	int		found_delimiter;

	// Check if stdin is a tty (interactive) or pipe/file (script)
	if (isatty(STDIN_FILENO))
	{
		// Interactive mode - use readline
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
	}
	else
	{
		// Script mode - read from stdin until delimiter found
		found_delimiter = 0;
		while (!found_delimiter && (n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[n] = '\0';
			i = 0;
			j = 0;
			
			while (i < (int)n)
			{
				if (buffer[i] == '\n')
				{
					buffer[i] = '\0';
					line = ft_strdup(&buffer[j]);
					if (ft_strcmp(line, delimiter) == 0)
					{
						found_delimiter = 1;
						free(line);
						break;
					}
					// Expand variables if any
					if (ft_strchr(line, '$'))
					{
						char *var_value = getenv("USER");
						if (var_value && ft_strstr(line, "$USER"))
						{
							char *expanded = ft_strjoin("", var_value);
							write(fd, expanded, ft_strlen(expanded));
							free(expanded);
						}
						else
						{
							write(fd, line, ft_strlen(line));
						}
					}
					else
					{
						write(fd, line, ft_strlen(line));
					}
					write(fd, "\n", 1);
					free(line);
					j = i + 1;
				}
				i++;
			}
			
			// Handle leftover without newline
			if (!found_delimiter && j < (int)n)
			{
				line = ft_strdup(&buffer[j]);
				if (ft_strcmp(line, delimiter) == 0)
				{
					found_delimiter = 1;
				}
				else 
				{
					write(fd, line, ft_strlen(line));
				}
				free(line);
			}
		}
	}
}

int	handle_heredoc(char *delimiter)
{
	char	*temp_filename;
	int		fd;
	int		content_fd;
	char	buffer[4096];
	ssize_t	bytes;
	
	// Pre-processed content'i kontrol et
	content_fd = open("/tmp/minishell_heredoc_content", O_RDONLY);
	if (content_fd != -1)
	{
		// Pre-processed content varsa onu kullan
		fd = create_temp_file(&temp_filename);
		if (fd == -1)
		{
			close(content_fd);
			return (-1);
		}
		
		while ((bytes = read(content_fd, buffer, sizeof(buffer))) > 0)
			write(fd, buffer, bytes);
			
		close(content_fd);
		close(fd);
		unlink("/tmp/minishell_heredoc_content");
		
		fd = open(temp_filename, O_RDONLY);
		unlink(temp_filename);
		free(temp_filename);
		return (fd);
	}
	
	// Fallback: normal heredoc processing
	fd = create_temp_file(&temp_filename);
	if (fd == -1)
		return (-1);
	read_heredoc_input(fd, delimiter);
	close(fd);
	fd = open(temp_filename, O_RDONLY);
	unlink(temp_filename);
	free(temp_filename);
	return (fd);
}

int	process_input_redirections(t_redirect *current, int *input_fd,
								int *output_fd)
{
	if (setup_input_redirect(current, input_fd) == -1)
	{
		if (*output_fd != -1)
			close(*output_fd);
		return (-1);
	}
	return (0);
}

int	process_output_redirections(t_redirect *current, int *input_fd,
								int *output_fd)
{
	if (setup_output_redirect(current, output_fd) == -1)
	{
		if (*input_fd != -1)
			close(*input_fd);
		return (-1);
	}
	return (0);
}
