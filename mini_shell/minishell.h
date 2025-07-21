/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:07 by makboga           #+#    #+#             */
/*   Updated: 2025/07/21 20:48:55 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

# ifndef BUFFER_SIZE
# define BUFFER_SIZE	1024
# endif

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define BRED "\033[1;31m"
# define BGREEN "\033[1;32m"
# define BYELLOW "\033[1;33m"
# define BBLUE "\033[1;34m"
# define BMAGENTA "\033[1;35m"
# define RESET "\033[0m"


typedef struct s_parameters
{
	char *parameter;
	struct s_parameters *next;
}t_parameters;


typedef struct s_command
{
	char *command;
	struct s_parameters *parameters_p;
	char *direction;
	int flag;
	struct s_command *next;
}	t_command;

// Shell yapıları
typedef struct s_shell
{
	char	**builtin;
	char	*prompt;
	char	**envp;
	char	*current_dir;
	char	*hostname;
	char    *display_info;
	char 	**options;
	t_command *command_p;
	// İleride komut yapısı vb. ekleyebiliriz
}	t_shell;

typedef struct s_quote
{
	char	**parameters;
	char	*current_parameter;
	int		flag;
	int		len;
}t_quote;

typedef struct s_read_file_info
{
	int		fd;
	char	buffer[BUFFER_SIZE + 1];
	int		b_read;
	int		total_b_read;
	char	*result;
}t_read_file;

//SHELL
void	init_shell(t_shell *shell, char **envp);
void	start_minishell(t_shell *shell);
void	get_display_info(t_shell *shell);
void	get_hostname(t_shell *shell);
int		open_file(char *filename, int flags);

//PARSER
void	get_prompt(t_shell *shell);
char	*single_quote_control(char **prompt);
char	*double_quote_control(char **prompt);
t_quote *quote_init(void);
int		counter_quote(char *str, char *quoter);
char 	**copy_multiple_input(char **multiple_input,char *temp, int len);
char 	*get_characters(char **prompt);
char 	*expand_if_dollar(const char *str, int *i);
char *get_next_char(const char *str, int *i);
char	*string_concatation_heap(char **str);

//FREE
void 	free_argv(char **argv);
void	free_shell(t_shell *shell);
void	ft_free_split(char **split);
void	ft_free_matrix(char **matrix);
char	*ft_strjoin_free(char *s1, const char *s2);
char	*set_and_free(char *dest, char *src);
void	free_options(t_shell *shell);
void	free_quote(t_quote *quote);
void	free_multiple_input(char **multiple_input);

//BUİLTİN
int 	builtin_exe(t_shell *shell, char **argv);
int		builtin_echo(char **argv);
int		builtin_cd(t_shell *shell, char **args);
int 	builtin_pwd(void);
int 	builtin_env(char **envp);
int		builtin_export(char ***envp, char **argv);
int		builtin_unset(t_shell *shell, char *name);
void	builtin_exit(char **argv);

//ENVIRONMENT
char	*mini_getenv(const char *key, char **envp);
char 	**mini_setenv(char **envp, const char *key, const char *value, int overwrite);
char	**mini_unsetenv(char *key, char **envp);
char	**ft_double_extension(char **matrix, char *new_str);
char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3);

//EXECUTE
void execute_commands(t_shell *shell, char **commands, int n);
void	run_commands(t_shell *shell);
char	*get_path(char *cmd, char **envp);

//UTILS
char	*string_concatation(char **str);

//ERROR
void exit_with_error(char *msg);
#endif
