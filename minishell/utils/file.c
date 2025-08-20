/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:20:21 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 16:21:37 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags);
	if (fd < 0)
		exit_with_error("Error opening file");
	return (fd);
}
