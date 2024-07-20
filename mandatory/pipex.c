/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/20 18:44:29 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

//? Permet d'executer n'importe quelle cmd avec ses flags.
void	exec_cmd(char *cmd_with_flags, t_pipex data)
{
	data.cmd_split = ft_split(cmd_with_flags, ' ');
	if (!data.cmd_split)
		gestion_error_close(44, data);
	data.path = ft_strjoin("/usr/bin/", data.cmd_split[0]);
	if (access(data.path, X_OK) == -1)
		gestion_error_close_free(222, data);
	if (execve(data.path, data.cmd_split, data.envp) == -1)
		gestion_error_close_free(333, data);
	ft_free_tabtab(data.cmd_split);
	free(data.path);
	return ;
}

void	child_process(char *cmd_vanilla1, int *end, t_pipex data)
{
	close(end[0]);
	if (dup2(end[1], STDOUT_FILENO) == -1)
		gestion_error_close(33, data);
	close(end[1]);
	if (dup2(data.infile, STDIN_FILENO) == -1)
		gestion_error_close(33, data);
	close(data.infile);
	exec_cmd(cmd_vanilla1, data);
}

void	parent_process(char *cmd_vanilla2, int *end, t_pipex data)
{
	close(end[1]);
	if (dup2(data.outfile, STDOUT_FILENO) == -1)
		gestion_error_close(33, data);
	close(data.outfile);
	if (dup2(end[0], STDIN_FILENO) == -1)
		gestion_error_close(33, data);
	close(end[0]);
	exec_cmd(cmd_vanilla2, data);
}

void	pipex(char *cmd_vanilla1, char *cmd_vanilla2, t_pipex data)
{
	int	end[2];
	int	id;

	if (pipe(end) == -1)
		gestion_error_close_free(111, data);
	id = fork();
	if (id == -1)
		gestion_error_close(22, data);
	if (id == 0)
		child_process(cmd_vanilla1, end, data);
	else
	{
		wait(0);
		close(data.infile);
		data.outfile = open(data.arg_outline, O_WRONLY | O_CREAT | O_TRUNC,
				0777);
		parent_process(cmd_vanilla2, end, data);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;

	data.arg_outline = argv[argc - 1];
	data.envp = envp;
	if (argc != 5)
		gestion_error(0);
	data.infile = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, 0777);
	if (access(argv[1], R_OK | F_OK) == -1)
		gestion_error(1);
	if (data.infile == -1)
		gestion_error(2);
	data.outfile = open(data.arg_outline, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (access(data.arg_outline, W_OK | F_OK) == -1)
		error_outfile(data, 2);
	if (data.outfile == -1)
		error_outfile(data, 1);
	close(data.outfile);
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		gestion_error_close(11, data);
	pipex(argv[2], argv[3], data);
	close(data.infile);
	close(data.outfile);
	return (0);
}
