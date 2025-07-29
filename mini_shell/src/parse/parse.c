/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/29 18:53:19 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void append(t_shell *shell, char *str,int *command, t_command **temp)
{
	char *temp_str;
    if (!str || !shell)
        return;
    if(!*command && prompt_type_control_loop(shell->builtin,1,str) >= 1 && prompt_type_control_loop(shell->builtin,1,str) <= 4)
    {
		if(prompt_type_control_loop(shell->builtin,1,str) == 2)
			temp_str = ft_strjoin("/bin/",str);
		else
			temp_str = ft_strdup(str);
		append_command(shell, temp_str, prompt_type_control_loop(shell->builtin,1,str), temp);
		free(temp_str);
		*command = 1;
	}
    else if(prompt_type_control_loop(shell->tokens,0,str) == 5)
    {
		append_token(str,temp);
		*command = 0;
	}
    else
    {
        if (*temp)
        {
            t_parameters *new_param = malloc(sizeof(t_parameters));
            new_param->parameter = ft_strdup(str);
            new_param->next = NULL;
            append_parameter(new_param,temp);
        }
    }
    
}


void parse_prompt(t_shell *shell)
{
	char	*temp_prompt;
	char	*current_option;
	char	*start;
	char * (*parse_func)(char **);
	int command;
	t_command *command_temp_p;

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
		else if(*temp_prompt != ' ')
			parse_func = get_characters;
		if(parse_func)
		{
			current_option = parse_func(&temp_prompt);
			if(current_option != NULL)
			{
				if(!command_temp_p)
				{
					append(shell,current_option,&command,&command_temp_p);
					shell->command_p = command_temp_p;
				}
				else
					append(shell,current_option,&command,&shell->command_p);
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
