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
    param = command->parameters_p; // Orijinal pointer'ı korumak için yeniden başlat
    while(param && param->parameter)
    {
        params[i] = param->parameter;
        i++;
        param = param->next;
    }
    params[i] = NULL;
    return params;
}

void execute(t_shell *shell)
{
    char **params;

    params = get_params(shell->command_p);
	if(shell->command_p->builtin == 1)
		printf("makboga:mdalkili minishell:Command Not Found\n");
    if (shell->command_p->builtin == 3 || shell->command_p->builtin == 2)
		run(shell->command_p, params,shell);
	else if(shell->command_p->builtin == 4)
	{
		if(ft_strcmp(shell->command_p->command,"echo") == 0)
			builtin_echo(params);
		else if(ft_strcmp(shell->command_p->command,"pwd") == 0)
			builtin_pwd();
		else if(ft_strcmp(shell->command_p->command,"exit") == 0)
			builtin_exit(shell);
		else if(ft_strcmp(shell->command_p->command,"env") == 0)
			builtin_env(shell->envp);
	}
	if(params)
		free(params);

}

