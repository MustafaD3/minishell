/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:07 by makboga           #+#    #+#             */
/*   Updated: 2025/08/07 03:49:20 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
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
	char				*parameter;
	struct s_parameters	*next;
}	t_parameters;

typedef struct s_command
{
	char				*command;
	struct s_parameters	*parameters_p;
	char				*token;
	int					token_flag;
	int					flag;
	int					builtin;
	struct s_command	*next;
}	t_command;

typedef struct s_shell
{
	char		*builtin[8];
	char		*tokens[8];
	char		*prompt;
	char		**envp;
	char		*current_dir;
	char		*hostname;
	char		*display_info;
	char		**options;
	int			last_exit_code;
	t_command	*command_p;
}	t_shell;

typedef struct s_quote
{
	char	**parameters;
	char	*current_parameter;
	int		flag;
	int		len;
}	t_quote;

typedef struct s_read_file_info
{
	int		fd;
	char	buffer[BUFFER_SIZE + 1];
	int		b_read;
	int		total_b_read;
	char	*result;
}	t_read_file;

void	init_shell(t_shell *shell, char **envp);
void	start_minishell(t_shell *shell);
void	get_display_info(t_shell *shell);
void	get_hostname(t_shell *shell);
int		open_file(char *filename, int flags);
void	setup_signals(void);
//PARSER
int		get_prompt(t_shell *shell);
char	*single_quote_control(char **prompt, t_shell *shell);
char	*double_quote_control(char **prompt, t_shell *shell);
t_quote	*quote_init(void);
int		counter_quote(char *str, char *quoter);
char	**copy_multiple_input(char **multiple_input, char *temp, int len);
char	*get_characters(char **prompt, t_shell *shell);
char	*expand_if_dollar(const char *str, int *i, t_shell *shell);
char	*get_next_char(const char *str, int *i);
char	*string_concatation_heap(char **str);
void	append_command(t_shell *shell, char *str, int builtin,
			t_command **temp);
void	append_parameter(t_parameters *new_param, t_command **temp);
void	append_token(char *str, t_command **temp);
int		prompt_type_control_loop(char **control_list, int type, char *str);
void	execute(t_shell *shell);
int		run(t_command *command, char **params, t_shell *shell);
//FREE
void	free_argv(char **argv);
void	free_shell(t_shell *shell);
void	ft_free_split(char **split);
void	ft_free_matrix(char **matrix);
char	*ft_strjoin_free(char *s1, const char *s2);
char	*set_and_free(char *dest, char *src);
void	free_options(t_shell *shell);
void	free_quote(t_quote *quote);
void	free_multiple_input(char **multiple_input);
void	free_command(t_shell *shell);

//BUİLTİN
int		builtin_echo(char **params);
int		builtin_cd(t_shell *shell, char **params);
int		builtin_pwd(char **params);
int		builtin_env(char **envp);
int		builtin_export(char ***envp, char **params);
int		builtin_unset(t_shell *shell, char **argv);
int		builtin_exit(t_shell *shell, char **params);

//ENVIRONMENT
char	*mini_getenv(const char *key, char **envp);
char	**mini_setenv(char **envp, const char *key, const char *value,
			int overwrite);
char	**mini_unsetenv(char *key, char **envp);
char	**ft_double_extension(char **matrix, char *new_str);
char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3);
void	execute_commands(t_shell *shell, char **commands, int n);
//UTILS
char	*string_concatation(char **str);
//ERROR
void	exit_with_error(char *msg);
#endif
