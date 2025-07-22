/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:27 by makboga           #+#    #+#             */
/*   Updated: 2025/07/22 19:42:50 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
    
    shell->prompt = NULL;
	shell->current_dir = NULL;
	shell->display_info = NULL;
    shell->command_p = NULL;
    
    // Builtin komutları başlat
    shell->builtin[0] = ft_strdup("exit");
	shell->builtin[1] = ft_strdup("echo");
	shell->builtin[2] = ft_strdup("cd");
    shell->builtin[3] = NULL;
    
	shell->tokens[0] = ft_strdup("|");
	shell->tokens[1] = NULL;
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
		shell->envp[i] = strdup(envp[i]);
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
        get_prompt(shell);
		if(shell->command_p)
			execute(shell);
    }
}




int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	ft_memset(&shell, 0, sizeof(t_shell));
	
	init_shell(&shell, envp);
	start_minishell(&shell);
	free_shell(&shell);
	return (0);
}