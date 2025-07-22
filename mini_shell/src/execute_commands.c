#include "../minishell.h"

char **get_params(t_command *command)
{
	int i;
    char **params;
    t_parameters *param;

    param = command->parameters_p;
    i = 0;
    while(param && param->parameter)
    {
        i++;
        param = param->next;
    }
    params = malloc(sizeof(char *) * (i + 2));
    if (!params)
    {
        perror("malloc");
        return NULL;
    }
    params[0] = command->command;
    i = 1;
    while(command->parameters_p && command->parameters_p->parameter)
    {
        params[i] = command->parameters_p->parameter;
        i++;
        command->parameters_p = command->parameters_p->next;
    }
    params[i] = NULL;
    return params;
}

void execute(t_shell *shell)
{
    char **params;

    params = get_params(shell->command_p);
	printf("Executing command: %s\n", shell->command_p->command);
    if (shell->command_p->builtin == 2 || shell->command_p->builtin == 1)
		run(shell->command_p, params,shell);
	else if(shell->command_p->builtin == 3)
	{
		if(ft_strcmp(shell->command_p->command,"echo") == 0)
			builtin_echo(params);
		else if(ft_strcmp(shell->command_p->command,"pwd") == 0)
			builtin_pwd();
		else if(ft_strcmp(shell->command_p->command,"exit") == 0)
			builtin_exit(params);
		else if(ft_strcmp(shell->command_p->command,"env") == 0)
			builtin_env(shell->envp);
		
		free(params);
	}
}

