/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/17 19:32:22 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex_bonus.h"

//* TESTEUR == https://github.com/michmos/42_pipex_tester
//* GRILLE CORRECTEUR == https://github.com/mharriso/school21-checklists/blob/master/ng_2_pipex.pdf
//! REFAIRE TOUS LES HEADERS + NORMINETTE
//! revoir les close (+ besoin de close infile/outfile systematiquement ??)  

//? Permet de verifier si les files decriptors sont bien valides.
void verif_fds(t_pipex data, int argc, char **argv)
{
	if (data.heredoc)
	{		
		if (access(argv[argc - 1], W_OK | F_OK) == -1 || data.outfile == -1)
			gestion_error(1);
	}
	else 
	{
		if (access(argv[1], R_OK | F_OK) == -1 || data.infile == -1)
			gestion_error(1);
		if (access(argv[argc - 1], W_OK | F_OK) == -1 || data.outfile == -1)
			gestion_error(1);	
	}
}

//? Permet de verifier si les cmds ne sont pas vides.
void verif_cmds(t_pipex data, int argc, char **argv)
{
	int i;
	
	if (data.heredoc == true)
		i = 3;
	else
		i = 2;
	while (i < (argc - 1))
	{
		if (argv[i][0] == '\0')
			gestion_error_close(11, data);
		i++;
	}
}

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

void	pipex(char *cmd, t_pipex data)
{
	int	end[2];
	int	id;

	if (pipe(end) == -1)
		gestion_error_close_free(111, data);
	id = fork();
	if (id == -1)
		gestion_error_close(22, data);
	if (id == 0)
	{
		close(end[0]);
		if (dup2(end[1], STDOUT_FILENO) == -1)
			gestion_error_close(33, data);
		close(end[1]); //!
		exec_cmd(cmd, data);
	}
	else
	{
		close(end[1]);
		if (dup2(end[0], STDIN_FILENO) == -1)
			gestion_error_close(33, data);
		close(end[0]); //!
		wait(0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;
	int i;

	if (argc < 5)
		gestion_error(0);
	data.envp = envp;
	data.heredoc = search_heredoc(argc, argv);
	if (data.heredoc)
	{		
		data.outfile = open(argv[argc - 1], O_WRONLY | O_APPEND, 0777);
		i = 3;
		heredoc(argv, data);
	}
	else 
	{
		data.infile = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, 0777);
		data.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(data.infile, STDIN_FILENO);
		close(data.infile); //!
		i = 2;
	}
	verif_fds(data, argc, argv);
	verif_cmds(data, argc, argv);
	while (i < argc - 2)
		pipex(argv[i++], data);
	dup2(data.outfile, STDOUT_FILENO);
	close(data.outfile); //!
	exec_cmd(argv[i], data);

}

//? Permet de regrouper dans un tableau de tableau toutes
//? les cmds passees en arguments, plus qu'a defiler.
// char **compil_cmds(int argc, char **argv)
// {
//     char **all_cmds;
//     int current_argv;
//     int i;
//     all_cmds = NULL;	
// 	all_cmds = malloc((argc - 3) * sizeof(char*)); //! probleme si here_doc
// 	current_argv = 2;
//     i = 0;
//     while (current_argv < (argc - 1))
//     {
//         all_cmds[i] = malloc((ft_strlen(argv[current_argv]) + 1) * sizeof(char));
//         ft_strlcpy(all_cmds[i], argv[current_argv],
// 		ft_strlen(argv[current_argv]) + 1);
//         i++;
//         current_argv++;
//     }
//     return (all_cmds);
// }

//? Permet de recuperer tout le contenu d'un fichier dans un char*.
// char *init_fullline(int fds)
// {
//     char *line;
//     char *tmp;

//     line = "";
//     while ((tmp = get_next_line(fds)))
//     {
//         if (tmp == NULL)
//             break ;
//         line = ft_strjoin(line, tmp);
//         tmp = NULL;
//         free(tmp);
//     }
//     return (line);
// }
