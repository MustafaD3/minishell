/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:30:18 by makboga           #+#    #+#             */
/*   Updated: 2025/07/17 19:16:25 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int builtin_exe(t_shell *shell, char **argv)
{
    if (argv && argv[0])
    {
        if (ft_strcmp(argv[0], "echo") == 0)
            return builtin_echo(argv);
        else if (ft_strcmp(argv[0], "pwd") == 0)
            return builtin_pwd();
        else if (ft_strcmp(argv[0], "env") == 0)
            return builtin_env(shell->envp);
        else if (ft_strcmp(argv[0], "exit") == 0)
        {
            builtin_exit(argv);
            return 0;
        }
        else if (ft_strcmp(argv[0], "export") == 0)
            return builtin_export(&shell->envp, argv);
        else if (ft_strcmp(argv[0], "unset") == 0)
            return builtin_unset(shell, argv[1]);
        else if (ft_strcmp(argv[0], "cd") == 0)
            return builtin_cd(shell, argv);
        else
            return (-1);
    }
    return 0;
}
