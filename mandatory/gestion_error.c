/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 22:47:47 by febouana          #+#    #+#             */
/*   Updated: 2024/07/20 16:38:21 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

void	gestion_error(int type)
{
	if (type == 0)
		write(2, "\033[1;31m$> ./pipex file1 cmd1 cmd2 file2\n\033[0m",
			ft_strlen("\033[1;31m$> ./pipex file1 cmd1 cmd2 file2\n\033[0m"));
	if (type == 1)
		write(2, "\033[1;31mError no permission file\n\033[0m",
			ft_strlen("\033[1;31mError no permission file\n\033[0m"));
	if (type == 2)
		write(2, "\033[1;31mError file not found\n\033[0m",
			ft_strlen("\033[1;31mError file not found\n\033[0m"));
	exit(EXIT_FAILURE);
}

void	gestion_error_close(int type, t_pipex data)
{
	if (type == 11)
		write(2, "\033[1;31mError no command\n\033[0m",
			ft_strlen("\033[1;31mError no command\n\033[0m"));
	if (type == 22)
		write(2, "\033[1;31mError fork\n\033[0m",
			ft_strlen("\033[1;31mError fork\n\033[0m"));
	if (type == 33)
		write(2, "\033[1;31mError dup2\n\033[0m",
			ft_strlen("\033[1;31mError dup2\n\033[0m"));
	if (type == 44)
		write(2, "\033[1;31mError ft_split\n\033[0m",
			ft_strlen("\033[1;31mError ft_split\n\033[0m"));
	close(data.infile);
	close(data.outfile);
	exit(EXIT_FAILURE);
}

void	gestion_error_close_free(int type, t_pipex data)
{
	if (type == 111)
		write(2, "\033[1;31mError pipe\n\033[0m",
			ft_strlen("\033[1;31mError pipe\n\033[0m"));
	if (type == 222)
		write(2, "\033[1;31mError command not found\n\033[0m",
			ft_strlen("\033[1;31mError command not found\n\033[0m"));
	if (type == 333)
		write(2, "\033[1;31mError exec command\n\033[0m",
			ft_strlen("\033[1;31mError exec command\n\033[0m"));
	close(data.infile);
	close(data.outfile);
	free(data.path);
	ft_free_tabtab(data.cmd_split);
	exit(EXIT_FAILURE);
}

void	error_outfile(t_pipex data, int type)
{
	if (type == 1)
		write(2, "\033[1;31mError file not found\n\033[0m",
			ft_strlen("\033[1;31mError file not found\n\033[0m"));
	if (type == 2)
		write(2, "\033[1;31mError no permission file\n\033[0m",
			ft_strlen("\033[1;31mError no permission file\n\033[0m"));
	close(data.infile);
	exit(EXIT_FAILURE);
}
