/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:07 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 16:47:25 by makboga          ###   ########.fr       */
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
 #include <sys/stat.h> 
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

typedef struct s_pipe_info
{
	int	*pipefds;
	int	n;
	int	i;
}t_pipe_info;

// Redirection türleri
typedef enum e_redirect_type
{
	REDIRECT_NONE,
	REDIRECT_IN,        // <
	REDIRECT_OUT,       // >
	REDIRECT_APPEND,    // >>
	REDIRECT_HEREDOC    // <<
} t_redirect_type;

// Redirection yapısı
typedef struct s_redirect
{
	t_redirect_type type;
	char *filename;
	int fd;
	struct s_redirect *next;
} t_redirect;

typedef struct s_command
{
	char *command;
	struct s_parameters *parameters_p;
	char *token;
	int token_flag;
	int flag;
	int builtin;
	t_redirect *redirections;    // Yeni: redirection listesi
	struct s_command *next;
}	t_command;

// Shell yapıları
typedef struct s_shell
{
	char	*builtin[8];
	char	*tokens[8];// && || | > >> << <
	char	*prompt;
	char	**envp;
	char	*current_dir;
	char	*hostname;
	char    *display_info;
	char 	**options;
	int		last_exit_code;
	int		is_quote;
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

void	init_shell(t_shell *shell, char **envp);
void	start_minishell(t_shell *shell);
void	get_display_info(t_shell *shell);
void	get_hostname(t_shell *shell);
int		open_file(char *filename, int flags);


//PARSER
int		get_prompt(t_shell *shell);
void 	parse_prompt(t_shell *shell);
char	*single_quote_control(char **prompt,t_shell *shell);
char	*single_quote(char **prompt);
char	*double_quote_control(char **prompt,t_shell *shell);
char	*double_quote(char **prompt, t_shell *shell);
t_quote *quote_init(void);
int		counter_quote(char *str, char *quoter);
char 	**copy_multiple_input(char **multiple_input,char *temp, int len);
char 	*get_characters(char **prompt,t_shell *shell);
char 	*get_redirect_operator(char **prompt,t_shell *shell);
char 	*expand_if_dollar(const char *str, int *i,t_shell *shell);
char 	*get_next_char(const char *str, int *i);
char	*string_concatation_heap(char **str);
void 	append_command(t_shell *shell, char *str,int builtin, t_command **temp);
void 	append_parameter(t_parameters *new_param, t_command **temp);
void	append_token(char *str, t_command **temp);
int 	prompt_type_control_loop(char **control_list,int type,char *str);
int		is_whitespace(char c);

//EXECUTE
void 	execute(t_shell *shell);
void 	execute_single_command(t_shell *shell);
int 	handle_command_execution(t_shell *shell, char **params);
int 	setup_and_restore_redirections(t_shell *shell, int *original_stdin, int *original_stdout, int setup_mode);
int 	run(t_command *command,char **params,t_shell *shell);
void 	execute_commands(t_shell *shell, char **commands, int n);
int		has_slash(const char *s);
void	close_and_free_pipes(int *pipefds, int n);

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
void	free_command(t_shell *shell);

//BUİLTİN
int		builtin(t_command **command);
int		builtin_echo(char **argv);
int		builtin_cd(t_shell *shell, char **args);
int 	builtin_pwd(char **args);
int 	builtin_env(char **params, char **envp);
int		builtin_export(t_shell *shell, char **args);
int		builtin_unset(t_shell *shell, char *name);
int	builtin_exit(t_shell *shell, char **argv, int last_exit_code);

//EXPORT UTILS
int		export_single_var(char ***envp, char *arg);
void	print_export_list(char **export_arr, int fd);
void	sort_export_list(char **export_arr);
int		count_env_vars(char **envp);
char	**create_export_array(char **envp);

//ENVIRONMENT
char	*mini_getenv(const char *key, char **envp);
char 	**mini_setenv(char **envp, const char *key, const char *value, int overwrite);
char	**ft_double_extension(char **matrix, char *new_str);
void	increment_shlvl(t_shell *shell);

//executor/execute.c
void	execute(t_shell *shell);
void	execute_main(t_shell *shell);
void	handle_pipe_commands(t_shell *shell);
int		has_pipe_outside_quotes(char *str);
void	fork_and_execute(t_shell *shell, char **commands, int n, int *pipefds);

//executor/execute_utils.c
char	*get_path(char *cmd, char **envp);
void	execute_commands(t_shell *shell, char **commands, int n);
void	cleanup_and_wait(int *pipefds, int n, pid_t last_pid, t_shell *shell);
void	execute_child_process(t_shell *shell, char *command, t_pipe_info pipe_info);

//executor/execute_builtins.c
int	handle_builtin_command(t_shell *shell, char **params, char *cmd_name);
char	**get_params(t_command *command);
int	handle_command_execution(t_shell *shell, char **params);
void	handle_single_command_exec(t_shell *shell);
void	execute_single_command(t_shell *shell);

//executor/execute_pipe_helpers.c
char	*strip_path(char *cmd);
int		create_pipes(int **pipefds, int n);
void	setup_child_pipes(int *pipefds, int n, int i);
int		check_pipe_syntax(char *str);
int		count_parameters(t_command *command);
void	fill_params_array(char **params, t_command *command, int count);



//UTILS
char	*string_concatation(char **str);

// REDIRECTION
t_redirect	*create_redirect(t_redirect_type type, char *filename);
void		add_redirect(t_command *cmd, t_redirect *redirect);
void		free_redirections(t_redirect *redirections);
int			setup_redirections(t_command *cmd);
int			restore_redirections(t_command *cmd);
int			handle_heredoc(char *delimiter);
int			is_redirect_token(char *token);
t_redirect_type	get_redirect_type(char *token);
void		process_redirections(t_shell *shell);
void		process_param_redirections(t_command *cmd);
void		process_token_redirections(t_command *cmd);
void		remove_parameter(t_parameters **head, t_parameters *to_remove);
char		*parse_redirect_token(char **prompt);
int			handle_param_redirect(t_command *cmd, t_parameters *param,
				t_parameters *prev_param);
int			process_input_redirections(t_redirect *current, int *input_fd,
				int *output_fd);
int			process_output_redirections(t_redirect *current, int *input_fd,
				int *output_fd);
int			handle_out_redirect(t_redirect *current, int *output_fd);
int			handle_append_redirect(t_redirect *current, int *output_fd);
void		handle_token_redirect(t_command *cmd, t_command *next_cmd);
int			validate_redirections(t_command *cmd);
int			setup_input_redirect(t_redirect *current, int *input_fd);
int			setup_output_redirect(t_redirect *current, int *output_fd);
int			apply_redirections(int input_fd, int output_fd);

// Command preprocessing
char		*normalize_redirections(char *command);

//ERROR
void exit_with_error(char *msg);

#endif
