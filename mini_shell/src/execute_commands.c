#include "../minishell.h"

char **get_params(t_command *command)
{
    int i, count = 0;
    char **params;
    t_parameters *param;

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
    i = 1;
    param = command->parameters_p;
    while(param && param->parameter)
    {
        params[i] = ft_strdup(param->parameter);
        i++;
        param = param->next;
    }
    params[i] = NULL;
    return params;
}

void execute(t_shell *shell)
{
    char **params;
	int exit_code;

	exit_code = 0;
    params = get_params(shell->command_p);
	
	// Builtin komutlar (return 4)
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
		printf("minishell: %s: command not found\n", params[0]);
		shell->last_exit_code = 127;
		if(params)
			ft_free_split(params);
		return;
	}
	
	shell->last_exit_code = exit_code;
	if(params)
		ft_free_split(params);
}

