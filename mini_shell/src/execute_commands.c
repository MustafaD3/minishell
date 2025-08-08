#include "../minishell.h"

char *expand_variables(char *line, t_shell *shell)
{
	char *temp_line = line;
	char *result = get_characters(&temp_line, shell);
	
	// Eğer line tamamen process edilmediyse, kalanını da ekle
	if (*temp_line)
	{
		char *remainder = ft_strdup(temp_line);
		char *final_result = ft_strjoin(result, remainder);
		free(result);
		free(remainder);
		return (final_result);
	}
	
	return (result);
}

void handle_heredoc(t_shell *shell, t_command *cmd)
{
	char *delimiter;
	char *line;
	char *heredoc_content;
	char *temp;
	int pipe_fd[2];
	pid_t pid;
	
	if (!cmd->parameters_p || !cmd->parameters_p->parameter)
	{
		write(STDERR_FILENO, "bash: syntax error near unexpected token `newline'\n", 52);
		shell->last_exit_code = 2;
		return;
	}
	
	delimiter = cmd->parameters_p->parameter;
	heredoc_content = ft_strdup("");
	
	// Heredoc içeriğini oku
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			break;
		}
		
		// Delimiter comparison - expansion yapmadan karşılaştır
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		// Eğer line delimiter değilse, variable expansion yap
		char *expanded_line = expand_variables(line, shell);
		
		// Heredoc content'e ekle
		temp = ft_strjoin(heredoc_content, expanded_line);
		free(heredoc_content);
		heredoc_content = ft_strjoin(temp, "\n");
		free(temp);
		free(expanded_line);
		free(line);
	}
	
	// Pipe oluştur ve cat komutunu çalıştır
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free(heredoc_content);
		return;
	}
	
	pid = fork();
	if (pid == 0)
	{
		// Child process: cat komutu
		close(pipe_fd[1]); // Write end'i kapat
		dup2(pipe_fd[0], STDIN_FILENO); // stdin'i pipe'dan oku
		close(pipe_fd[0]);
		
		char *cat_args[] = {"cat", NULL};
		execve("/bin/cat", cat_args, shell->envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		// Parent process: heredoc content'i pipe'a yaz
		close(pipe_fd[0]); // Read end'i kapat
		write(pipe_fd[1], heredoc_content, ft_strlen(heredoc_content));
		close(pipe_fd[1]);
		waitpid(pid, NULL, 0);
	}
	else
	{
		perror("fork");
	}
	
	free(heredoc_content);
}

void handle_redirection(t_shell *shell, t_command *cmd)
{
	int fd;
	char *filename;
	
	if (!cmd->parameters_p || !cmd->parameters_p->parameter)
	{
		write(STDERR_FILENO, "bash: syntax error near unexpected token `newline'\n", 52);
		shell->last_exit_code = 2;
		return;
	}
	
	filename = cmd->parameters_p->parameter;
	
	if (ft_strcmp(cmd->token, ">>") == 0)
	{
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			if (access(filename, F_OK) == 0)
			{
				// Dosya var ama yazma izni yok veya directory
				struct stat st;
				if (stat(filename, &st) == 0 && S_ISDIR(st.st_mode))
				{
					write(STDERR_FILENO, "bash: ", 6);
					write(STDERR_FILENO, filename, ft_strlen(filename));
					write(STDERR_FILENO, ": Is a directory\n", 17);
				}
				else
				{
					write(STDERR_FILENO, "bash: ", 6);
					write(STDERR_FILENO, filename, ft_strlen(filename));
					write(STDERR_FILENO, ": Permission denied\n", 20);
				}
			}
			else
			{
				perror("bash");
			}
			shell->last_exit_code = 1;
			return;
		}
		close(fd);
	}
	else if (ft_strcmp(cmd->token, ">") == 0)
	{
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("bash");
			shell->last_exit_code = 1;
			return;
		}
		close(fd);
	}
	else if (ft_strcmp(cmd->token, "<<") == 0)
	{
		handle_heredoc(shell, cmd);
		return;
	}
	// Diğer redirection türleri için de benzer işlemler...
	
	shell->last_exit_code = 0;
}

char **get_params(t_command *command)
{
    int i, count = 0;
    char **params;
    t_parameters *param;

    if (!command || !command->command)
        return NULL;

    param = command->parameters_p;
    while(param && param->parameter)
    {
        count++;
        param = param->next;
    }
    params = malloc(sizeof(char *) * (count + 2));
    if (!params)
        return NULL;
    
    params[0] = ft_strdup(command->command);
    if (!params[0])
    {
        free(params);
        return NULL;
    }
    
    i = 1;
    param = command->parameters_p;
    while(param && param->parameter)
    {
        params[i] = ft_strdup(param->parameter);
        if (!params[i])
        {
            while (--i >= 0)
                free(params[i]);
            free(params);
            return NULL;
        }
        i++;
        param = param->next;
    }
    params[i] = NULL;
    return params;
}

void execute_piped_commands(t_shell *shell)
{
    t_command *cmd = shell->command_p;
    int fd[2];
    int in_fd = 0; // İlk komut için stdin
    pid_t pid;

    while (cmd)
    {
        if (cmd->next) // Eğer sonraki komut varsa pipe oluştur
            pipe(fd);
        
        pid = fork();
        if (pid == 0)
        {
            // Child process
            if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO); // stdin'i önceki pipe'ın çıkışına bağla
                close(in_fd);
            }
            if (cmd->next)
            {
                dup2(fd[1], STDOUT_FILENO); // stdout'u pipe'ın girişine bağla
                close(fd[1]);
            }
            if (cmd->next)
                close(fd[0]);
            
            char **params = get_params(cmd);
            if (!params)
                exit(EXIT_FAILURE);
            
            // Builtin mi external mi kontrol et
            if (cmd->builtin == 4)
            {
                // Builtin komutlar fork içinde çalışmaz, parent'ta çalışmalı
                // Ama pipe'da mecburen child'da çalışacak
                int exit_code = 0;
                if(ft_strcmp(cmd->command,"echo") == 0)
                    exit_code = builtin_echo(params);
                else if(ft_strcmp(cmd->command,"pwd") == 0)
                    exit_code = builtin_pwd(params);
                else if(ft_strcmp(cmd->command,"env") == 0)
                    exit_code = builtin_env(shell->envp);
                ft_free_split(params);
                exit(exit_code);
            }
            else if (cmd->builtin == 3 || cmd->builtin == 2)
            {
                execve(cmd->command, params, shell->envp);
                perror("execve");
                ft_free_split(params);
                exit(EXIT_FAILURE);
            }
            else
            {
                write(STDERR_FILENO, "minishell: command not found\n", 29);
                ft_free_split(params);
                exit(127);
            }
        }
        else if (pid > 0)
        {
            // Parent process
            if (cmd->next)
                close(fd[1]);
            if (in_fd != 0)
                close(in_fd);
            if (cmd->next)
                in_fd = fd[0]; // Sonraki komut için stdin
            cmd = cmd->next;
        }
        else
        {
            perror("fork");
            return;
        }
    }
    
    // Tüm child process'leri bekle
    while (wait(NULL) > 0);
}

void execute(t_shell *shell)
{
    char **params;
	int exit_code;
	t_command *cmd;
	int has_pipe;

	has_pipe = 0;
	exit_code = 0;
	cmd = shell->command_p;
	
	// Redirection kontrolü
	if (cmd && cmd->token_flag && cmd->token)
	{
		handle_redirection(shell, cmd);
		return;
	}
	
	if (cmd && cmd->next)
		has_pipe = 1;
	
	if (has_pipe)
	{
		execute_piped_commands(shell);
		return;
	}
    params = get_params(shell->command_p);
	if(shell->command_p->builtin == 4)
	{
		if(ft_strcmp(shell->command_p->command,"echo") == 0)
        	exit_code = builtin_echo(params);
		else if(ft_strcmp(shell->command_p->command,"pwd") == 0)
			exit_code = builtin_pwd(params);
		else if(ft_strcmp(shell->command_p->command,"cd") == 0)
			exit_code = builtin_cd(shell, params);
		else if(ft_strcmp(shell->command_p->command,"export") == 0)
			exit_code = builtin_export(&(shell->envp), params);
		else if(ft_strcmp(shell->command_p->command,"unset") == 0)
			exit_code = builtin_unset(shell, params); 
		else if(ft_strcmp(shell->command_p->command,"env") == 0)
			exit_code = builtin_env(shell->envp);
		else if(ft_strcmp(shell->command_p->command,"exit") == 0)
			exit_code = builtin_exit(shell, params);
	}
	// External komutlar (return 2: /bin/, return 3: current dir)
    else if (shell->command_p->builtin == 3 || shell->command_p->builtin == 2)
		exit_code = run(shell->command_p, params,shell);
	// Command not found (return 1)
	else
	{
		write(STDERR_FILENO, "sh: ", 4);
		write(STDERR_FILENO, shell->command_p->command, ft_strlen(shell->command_p->command));
		write(STDERR_FILENO, ": command not found\n", 20);
		shell->last_exit_code = 127;
		if(params)
			ft_free_split(params);
		return;
	}
	
	shell->last_exit_code = exit_code;
	if(params)
		ft_free_split(params);
}

