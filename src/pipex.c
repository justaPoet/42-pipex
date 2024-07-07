/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/07 23:06:42 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

//? Permet d'executer n'importe quelle cmd avec ses flags.
void	exec_cmd(char *cmd_with_flags, t_Data data)
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

void	child_process(char *cmd_vanilla1, int *end, t_Data data)
{
	close(end[0]);
	if (dup2(end[1], STDOUT_FILENO) == -1)
		gestion_error_close(33, data);
	if (dup2(data.infile, STDIN_FILENO) == -1)
		gestion_error_close(33, data);
	exec_cmd(cmd_vanilla1, data);
}

void	parent_process(char *cmd_vanilla2, int *end, t_Data data)
{
	wait(0);
	close(end[1]);
	if (dup2(data.outfile, STDOUT_FILENO) == -1)
		gestion_error_close(33, data);
	if (dup2(end[0], STDIN_FILENO) == -1)
		gestion_error_close(33, data);
	exec_cmd(cmd_vanilla2, data);
}

void	pipex(char *cmd_vanilla1, char *cmd_vanilla2, t_Data data)
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
	t_Data	data;

	data.envp = envp;
	if (argc != 5)
		gestion_error(0);
	data.infile = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, 0777);
	if (access(argv[1], R_OK | F_OK) == -1)
		gestion_error(1);
	data.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (access(argv[argc - 1], W_OK | F_OK) == -1)
		gestion_error(1);
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		gestion_error_close(11, data);
	pipex(argv[2], argv[3], data);
	close(data.infile);
	close(data.outfile);
	return (0);
}

//* https://github.com/michmos/42_pipex_tester

//!?O_CREAT == creer si inexistant | O_TRUNC == ecrase si deja exisant
//* (bonus) O_APPEND == ajoute a la suite d'un fichier texte (//here_doc)
//* (bonus) faire fonctionner pipex avec n'importe quel nbr de cmd
//* (bonus) revoir chaque cas ou argv[x] est utiliser et le modifier
//* (bonus) l'adapter pour repondre a autant de cmd qu'on veut (rajouter boucle)
//* (bonus) cas avec here_doc ou le second arg n'est pas une cmd
//* (endgame) verifier que les exit debouchent sur la bonne sortie
//* (endgame) ft_putstr_fd("./pipex infile cmd cmd outfile\n", 2)

//! ======================================================================================
//! BONUS
//! ======================================================================================

//? Permet de regrouper dans un tableau de tableau toutes
//? les cmds passees en arguments, plus qu'a defiler .
// char **compil_cmds(int argc, char** argv)
// {
//     char **all_cmds;
//     int current_argv;
//     int i;
//     all_cmds = NULL;
//     all_cmds = malloc((argc - 3) * sizeof(char*));
//     current_argv = 2;
//     i = 0;
//     while (current_argv < (argc - 1))
//     {
//         all_cmds[i] = malloc((ft_strlen(argv[current_argv]) + 1) * sizeof(char));
//         ft_strlcpy(all_cmds[i], argv[current_argv],
	//ft_strlen(argv[current_argv]) + 1); //? rajout +1
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

//! ======================================================================================
//! INUTILE
//! ======================================================================================

//? gadget, pas tres util ?
// int cmds_counter(int limit, char **args)
// {
//     int counter;
//     int i;
//     counter = 0;
//     i = 1;
//     while (i < limit)
//     {
//         if (ft_strncmp("infile", args[i], 5) != 0 || ft_strncmp("outfile", args[i], 5) != 0 || ft_strncmp("here_doc", args[i], 8) != 0)
//             counter++;
//         i++;
//     }
//     return (counter)
// }

//? Permet de verifier si infile et outfile existent et s'ils sont uniques
// void verif_files(int argc, char **argv)
// {
//     int search_infile = 0;
//     int search_outfile = 0;
//     int i = 1;

//     while (i < argc)
//     {
//         if (ft_strncmp("infile", argv[i], 5) == 0)
//         {
//             if (search_infile != 0)
//                 break ;
//             search_infile++;
//         }
//         if (ft_strncmp("outfile", argv[i], 5) == 0)
//         {
//             if (search_outfile != 0)
//                 break ;
//             if (search_infile == 0)
//                 search_outfile = INT_MAX;
//             search_outfile++;
//         }
//         i++;
//     }
//     if (search_infile + search_outfile != 2)
//     {
//         perror("INVALID ARGUMENTS\n");
//         exit(1);
//     }
// }

//? Parent process avec testeur
// void parent_process(char *cmd_vanilla2, int *end, t_Data data)
// {
//     wait(0);
//     close(end[1]);
//     if (dup2(data.outfile, STDOUT_FILENO) == -1)
//     {
//         perror("Error dup2 entree 2.1\n");
//         return ;
//     }
//     if (dup2(end[0], STDIN_FILENO) == -1)
//     {
//         perror("Error dup2 sortie 2.2\n");
//         return ;
//     }
//     //outfile = outfile; //pour avoir retour terminal si comment dup2()

//     // char *line = init_fullline(outfile);
//     // if (line == NULL)
//     // {
//     //     perror("CAMARCHEPAS\n");
//     //     return ;
//     // }
//     // printf("got from child process ==%s\n", line);
//     // free(line);
//     exec_cmd(cmd_vanilla2, data.envp);
// }
