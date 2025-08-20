/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 02:56:03 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/21 00:41:15 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_hostname(t_shell *shell)
{
	t_read_file	*host_f;

	shell->hostname = NULL;
	host_f = malloc(sizeof(t_read_file));
	if (!host_f)
		return (shell->hostname = ft_strdup("localhost"), (void)0);
	host_f->fd = open("/etc/hostname", O_RDONLY);
	if (host_f->fd < 0)
		return ((shell->hostname = ft_strdup("localhost"),
				free(host_f)), (void)0);
	host_f->b_read = 1;
	host_f->total_b_read = 0;
	while (host_f->b_read > 0)
	{
		host_f->b_read = read(host_f->fd, host_f->buffer, BUFFER_SIZE);
		if (host_f->b_read <= 0)
			break ;
		host_f->result = malloc(host_f->total_b_read + host_f->b_read + 1);
		if (!host_f->result)
			return (close(host_f->fd), free(host_f),
				shell->hostname = ft_strdup("localhost"), (void)0);
		if (shell->hostname)
			(ft_memcpy(host_f->result, shell->hostname,
					host_f->total_b_read), free(shell->hostname));
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

	user = getenv("USER");
	if (!user)
		user = "user";
	if (!shell->hostname)
		hostname = "localhost";
	else
		hostname = shell->hostname;
	if (!shell->current_dir)
		path = ft_strdup("~");
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
			user, "@", hostname, ":", BGREEN, path, RESET, "$ ", NULL});
	shell->display_info = set_and_free(shell->display_info, properties);
	free(path);
}
