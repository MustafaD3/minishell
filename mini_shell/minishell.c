/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:27 by makboga           #+#    #+#             */
/*   Updated: 2025/08/06 04:31:27 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
    
    shell->prompt = NULL;
	shell->current_dir = NULL;
	shell->display_info = NULL;
	shell->last_exit_code = 0;
    shell->command_p = NULL;
    
    // Builtin komutları başlat
    shell->builtin[0] = ft_strdup("echo");
	shell->builtin[1] = ft_strdup("pwd");
	shell->builtin[2] = ft_strdup("cd");
	shell->builtin[3] = ft_strdup("export");
	shell->builtin[4] = ft_strdup("unset");
	shell->builtin[5] = ft_strdup("env");
	shell->builtin[6] = ft_strdup("exit");
    shell->builtin[7] = NULL;
    
	shell->tokens[0] = ft_strdup("|");
	shell->tokens[1] = ft_strdup(">>");
	shell->tokens[2] = ft_strdup("<<");
	shell->tokens[3] = ft_strdup(">");
	shell->tokens[4] = ft_strdup("<");
	shell->tokens[5] = NULL;
    get_hostname(shell);
	///
    int	i;
	i = 0;
	while (envp[i])
		i++;
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	shell->envp[i] = NULL;
}

void start_minishell(t_shell *shell)
{

    while (1)
    {
        if (shell->current_dir)
            free(shell->current_dir);
        shell->current_dir = getcwd(NULL, 0);
        get_display_info(shell);
        if(!get_prompt(shell))
			break;
		if(shell->command_p)
			execute(shell);
    }
}




int	main(int argc, char **argv, char **envp)
{
	int exit_code;
	t_shell	shell;

	(void)argc;
	(void)argv;

	ft_memset(&shell, 0, sizeof(t_shell));
	
	init_shell(&shell, envp);
	setup_signals();
	start_minishell(&shell);
	exit_code = shell.last_exit_code;
	free_shell(&shell);
	return (exit_code);
}