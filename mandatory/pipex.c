/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/17 19:03:00 by apoet            ###   ########.fr       */
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
	if (dup2(data.infile, STDIN_FILENO) == -1)
		gestion_error_close(33, data);
	exec_cmd(cmd_vanilla1, data);
}

void	parent_process(char *cmd_vanilla2, int *end, t_pipex data)
{
	wait(0);
	close(end[1]);
	if (dup2(data.outfile, STDOUT_FILENO) == -1)
		gestion_error_close(33, data);
	if (dup2(end[0], STDIN_FILENO) == -1)
		gestion_error_close(33, data);
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
		parent_process(cmd_vanilla2, end, data);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;

	data.envp = envp;
	if (argc != 5)
		gestion_error(0);
	data.infile = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, 0777);
	if (access(argv[1], R_OK | F_OK) == -1 || data.infile == -1) //! OK
		gestion_error(1);
	data.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (access(argv[argc - 1], W_OK | F_OK) == -1 || data.outfile == -1) //! OK
		gestion_error(1);
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		gestion_error_close(11, data);
	pipex(argv[2], argv[3], data);
	close(data.infile); //!
	close(data.outfile); //!
	return (0);
}
