/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/15 04:49:33 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void append(t_shell *shell, char *str,int *command, t_command **temp)
{
	char *temp_str;
    if (!str || !shell)
        return;

    // Heredoc: önceki token '<<' ise, bu gelen str delimiter'dır
    if (*temp && (*temp)->token_flag && (*temp)->token && ft_strcmp((*temp)->token, "<<") == 0)
    {
        free((*temp)->token);
        (*temp)->token = NULL;
        (*temp)->token_flag = 0;
        return;
    }
    if(!*command && prompt_type_control(shell->builtin,1,str) >= 1 && prompt_type_control(shell->builtin,1,str) <= 4)
    {
		if(prompt_type_control(shell->builtin,1,str) == 2)
			temp_str = ft_strjoin("/bin/",str);
		else
			temp_str = ft_strdup(str);
		append_command(shell, temp_str, prompt_type_control(shell->builtin,1,str), temp);
		free(temp_str);
		*command = 1;
	}
    else if(prompt_type_control(shell->tokens,0,str) == 5)
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
	char		*temp_prompt;
	char		*current_option;
	char		*start;
	char 		*(*parse_func)(char **,t_shell *);
	int			command;
	t_command	*command_temp_p;

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
		else if(!ft_isspace(*temp_prompt))
			parse_func = get_characters;
		if(parse_func)
		{
			current_option = parse_func(&temp_prompt,shell);
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

char	*my_getline(void)
{
    size_t	cap;
    size_t	len;
    char	*s;
    int		ch;
    char	*tmp;

    cap = 128;
    len = 0;
    s = malloc(cap);
    if (!s)
        return (NULL);
    while ((ch = getchar()) != EOF && ch != '\n')
    {
        if (len + 1 >= cap)
        {
            cap *= 2;
            tmp = realloc(s, cap);
            if (!tmp)
            {
                free(s);
                return (NULL);
            }
            s = tmp;
        }
        s[len++] = (char)ch;
    }
    if (ch == EOF && len == 0)
    {
        free(s);
        return (NULL);
    }
    s[len] = '\0';
    return (s);
}

static int	non_interactive_mode(t_shell *shell)
{
    char	*line;
    size_t	n;
    ssize_t	r;

    line = NULL;
    n = 0;
    r = getline(&line, &n, stdin);
    if (r <= 0)
    {
        free(line);
        return (0);
    }
    if (r > 0 && line[r - 1] == '\n')
        line[r - 1] = '\0';
    shell->prompt = set_and_free(shell->prompt, line);
    parse_prompt(shell);
    return (1);
}

int	get_prompt(t_shell *shell)
{
	if(isatty(STDIN_FILENO))
	{
		shell->prompt = set_and_free(shell->prompt, readline(shell->display_info));
		if(!shell->prompt)
			return 0;
		if(shell->prompt && *shell->prompt)
		{
			parse_prompt(shell);
			add_history(shell->prompt);
		}
		else
		{
			free_command(shell);
			shell->command_p = NULL;
		}
	}
	else
		return (non_interactive_mode(shell));
	return 1;
}
