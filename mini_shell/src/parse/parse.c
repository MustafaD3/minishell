/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/08 17:56:07 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void append(t_shell *shell, char *str,int *command, t_command **temp, int is_quoted)
{
	char *temp_str;
	char *actual_str = str;
	int is_post_expansion_token = 0;
	
    if (!str || !shell)
        return;
    
    // Post-expansion token kontrolü
    if (ft_strncmp(str, "__TOKEN__", 9) == 0)
    {
        actual_str = str + 9; // "__TOKEN__" prefix'ini atla
        is_post_expansion_token = 1;
    }
    
    // Token kontrolü - quoted değilse veya post-expansion token ise
    if((!is_quoted || is_post_expansion_token) && prompt_type_control_loop(shell->tokens,0,actual_str) == 5)
    {
		append_token(actual_str,temp);
		// Token sonrası parameter bekleniyor, command bitti
		*command = 2; // Special case: token waiting for parameter
	}
    // Sonra builtin/command kontrolü yap
    else if(!*command && prompt_type_control_loop(shell->builtin,1,actual_str) >= 1 && prompt_type_control_loop(shell->builtin,1,actual_str) <= 4)
    {
		if(prompt_type_control_loop(shell->builtin,1,actual_str) == 2)
			temp_str = ft_strjoin("/bin/",actual_str);
		else
			temp_str = ft_strdup(actual_str);
		append_command(shell, temp_str, prompt_type_control_loop(shell->builtin,1,actual_str), temp);
		free(temp_str);
		*command = 1;
	}
    else
    {
        if (*temp)
        {
            t_parameters *new_param = malloc(sizeof(t_parameters));
            new_param->parameter = ft_strdup(actual_str);
            new_param->next = NULL;
            append_parameter(new_param,temp);
            // Token sonrası parameter aldıktan sonra command reset et
            if (*command == 2)
                *command = 1;
        }
    }
    
}


void parse_prompt(t_shell *shell)
{
	char	*temp_prompt;
	char	*current_option;
	char	*start;
	char * (*parse_func)(char **,t_shell *);
	int command;
	t_command *command_temp_p;
	int i;

	// Tab karakterlerini space'e çevir
	i = 0;
	while (shell->prompt && shell->prompt[i])
	{
		if (shell->prompt[i] == '\t')
			shell->prompt[i] = ' ';
		i++;
	}

	command = 0;
	free_command(shell);
	command_temp_p = NULL;
	shell->command_p = command_temp_p;
	temp_prompt = ft_strdup(shell->prompt);
	start = temp_prompt;
	while(temp_prompt && *temp_prompt)
	{
		parse_func = NULL;
		if(*temp_prompt == '\'')
			parse_func = single_quote_control;
		else if(*temp_prompt == '"')
			parse_func = double_quote_control;
		else if(*temp_prompt != ' ' && *temp_prompt != '\t')
			parse_func = get_characters;
		if(parse_func)
		{
			current_option = parse_func(&temp_prompt,shell);
			if(current_option != NULL)
			{
				// Quote function mu yoksa normal parsing mi?
				int is_quoted = (parse_func == single_quote_control || parse_func == double_quote_control);
				
				if(!command_temp_p)
				{
					append(shell,current_option,&command,&command_temp_p,is_quoted);
					shell->command_p = command_temp_p;
				}
				else
					append(shell,current_option,&command,&shell->command_p,is_quoted);
			}
			free(current_option);
			continue;
		}
		temp_prompt++;
	}
	shell->command_p = command_temp_p;
	free(start);
}

void	get_hostname(t_shell *shell)
{
	t_read_file	*host_f;

	shell->hostname = NULL;
	host_f = malloc(sizeof(t_read_file));
	if (!host_f)
	{
		shell->hostname = ft_strdup("localhost");
		return;
	}
	
	host_f->fd = open("/etc/hostname", O_RDONLY);
	if (host_f->fd < 0)
	{
		shell->hostname = ft_strdup("localhost");
		free(host_f);
		return;
	}
	
	host_f->b_read = 1;
	host_f->total_b_read = 0;
	while (host_f->b_read > 0)
	{
		host_f->b_read = read(host_f->fd, host_f->buffer, BUFFER_SIZE);
		if (host_f->b_read <= 0)
			break;
			
		host_f->result = malloc(host_f->total_b_read + host_f->b_read + 1);
		if (!host_f->result)
		{
			close(host_f->fd);
			free(host_f);
			shell->hostname = ft_strdup("localhost");
			return;
		}
		
		if (shell->hostname)
		{
			ft_memcpy(host_f->result, shell->hostname, host_f->total_b_read);
			free(shell->hostname);
		}
		ft_memcpy(host_f->result + host_f->total_b_read,
			host_f->buffer, host_f->b_read);
		host_f->total_b_read += host_f->b_read;
		host_f->result[host_f->total_b_read] = '\0';
		shell->hostname = host_f->result;
	}
	close(host_f->fd);
	
	if (shell->hostname && host_f->total_b_read > 0)
		shell->hostname[host_f->total_b_read - 1] = '\0';
	else if (!shell->hostname)
		shell->hostname = ft_strdup("localhost");
		
	free(host_f);
}

void	get_display_info(t_shell *shell)
{
	char	*path;
	char	*properties;
	char	*user;
	char	*hostname;
	
	// Güvenli değer atamaları
	user = getenv("USER");
	if (!user)
		user = "user";
	
	if (!shell->hostname)
		hostname = "localhost";
	else
		hostname = shell->hostname;
	
	// current_dir kontrolü
	if (!shell->current_dir)
	{
		path = ft_strdup("~");
	}
	else
	{
		path = ft_strstr(shell->current_dir, user);
		if (!path)
			path = ft_strdup("/");
		else if (ft_strcmp(path, user) == 0)
			path = ft_strdup("~");
		else
			path = ft_strjoin("~", path + ft_strlen(user));
	}
	
	properties = string_concatation((char *[]){BBLUE,
							user, "@", hostname,":", BMAGENTA, path, RESET, "$ ", NULL});
	shell->display_info = set_and_free(shell->display_info, properties);
	free(path);
}

int	get_prompt(t_shell *shell)
{
	shell->prompt = set_and_free(shell->prompt, readline(shell->display_info));
	if(!shell->prompt)
	{
		printf("exit\n");
		return 0;
	}
	if(shell->prompt && *shell->prompt)
	{
		parse_prompt(shell);
		add_history(shell->prompt);
	}
	return 1;
}
