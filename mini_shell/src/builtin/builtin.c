/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:30:18 by makboga           #+#    #+#             */
/*   Updated: 2025/07/28 18:35:54 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int run(t_command *command,char **params,t_shell *shell)
{
    int pid;
    int status;
    
    pid = fork();
    if (pid == 0)
    {
        // Çocuk proses: execve çağır
        execve(command->command, params, shell->envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
    {
        perror("fork");
        return -1;
    }
    return 0;
}

// if ((*command)->builtin) == 1)
        //     return builtin_echo(argv);
        // else if (ft_strcmp(argv[0], "pwd") == 0)
        //     return builtin_pwd();
        // else if (ft_strcmp(argv[0], "env") == 0)
        //     return builtin_env(shell->envp);
        // else if (ft_strcmp(argv[0], "exit") == 0)
        // {
        //     builtin_exit(argv);
        //     return 0;
        // }
        // else if (ft_strcmp(argv[0], "export") == 0)
        //     return builtin_export(&shell->envp, argv);
        // else if (ft_strcmp(argv[0], "unset") == 0)
        //     return builtin_unset(shell, argv[1]);
        // else if (ft_strcmp(argv[0], "cd") == 0)
        //     return builtin_cd(shell, argv);
        // else
        //     return (-1);