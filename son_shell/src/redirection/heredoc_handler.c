/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:41:48 by makboga           #+#    #+#             */
/*   Updated: 2025/08/29 18:50:24 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	heredoc_read_loop(char *delim_copy, int pipe_fd[2], t_req *req)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipe_fd[1]);
			free(delim_copy);
			free_shell(req->shell);
			exit(0);
		}
		if (ft_strcmp(line, delim_copy) == 0)
		{
			free(line);
			close(pipe_fd[1]);
			free(delim_copy);
			free_shell(req->shell);
			exit(0);
		}
		process_heredoc_line(line, pipe_fd[1], req);
	}
}

static void	do_heredoc_child(const char *delimiter, int pipe_fd[2], t_req *req)
{
	char	*delim_copy;
	int		saved_stdin;
	int		saved_stdout;

	close(pipe_fd[0]);
	delim_copy = ft_strdup(delimiter);
	if (!delim_copy)
	{
		close(pipe_fd[1]);
		exit(1);
	}
	setup_heredoc_tty(&saved_stdin, &saved_stdout);
	heredoc_read_loop(delim_copy, pipe_fd, req);
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static int	start_heredoc_process(const char *delimiter,
	int pipe_fd[2], t_req *req, void (*old_sigint)(int))
{
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		req->exit_stat = 1;
		signal(SIGINT, old_sigint);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		req->exit_stat = 1;
		signal(SIGINT, old_sigint);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		do_heredoc_child(delimiter, pipe_fd, req);
	}
	return (pid);
}

static int	check_heredoc_status(int status, int pipe_fd[2], t_req *req)
{
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		req->exit_stat = 130;
		req->heredoc_interrupted = 1;
		close(pipe_fd[0]);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		req->exit_stat = WEXITSTATUS(status);
		close(pipe_fd[0]);
		return (-1);
	}
	return (0);
}

int	handle_heredoc(const char *delimiter, t_req *req)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	void	(*old_sigint)(int);

	if (req && req->heredoc_interrupted)
		return (-1);
	old_sigint = signal(SIGINT, SIG_IGN);
	pid = start_heredoc_process(delimiter, pipe_fd, req, old_sigint);
	if (pid == -1)
		return (-1);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	if (check_heredoc_status(status, pipe_fd, req) == -1)
		return (-1);
	return (pipe_fd[0]);
}
