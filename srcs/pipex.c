/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/05 20:44:44 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

//? Permet d'executer n'importe quelle cmd avec ses flags, 
//? necessite au prealable une string contenant la cmd voulue.
void exec_cmd(char *cmd_with_flags, char** envp)
{
    char *path;
    char **cmd_split;
    
    cmd_split = ft_split(cmd_with_flags, ' ');
    if (!cmd_split)
    {
        perror("Error ft_split\n");
        exit(EXIT_FAILURE);
    }
    path = ft_strjoin("/usr/bin/", cmd_split[0]);
    if (access(path, X_OK) == -1)
    {
        free(path);
        ft_free_tabtab(cmd_split); 
        perror("Error command not found");
        exit(EXIT_FAILURE);
    }
    if (execve(path, cmd_split, envp) == -1)
    {
        free(path);
        ft_free_tabtab(cmd_split);    
        perror("Error exec command");
        exit(EXIT_FAILURE);
    }
    ft_free_tabtab(cmd_split);    
    return ;
}

void child_process(char *cmd_vanilla1, int *end, int infile, char **envp)
{
    close(end[0]);
    if (dup2(end[1], STDOUT_FILENO) == -1)
    {
        perror("Error dup2 sortie\n");
        return ;
    }
    if (dup2(infile, STDIN_FILENO) == -1)
    {
        perror("Error dup2 entree\n");
        return ;
    }
    //infile = infile; //pour avoir retour terminal si comment dup2()
    exec_cmd(cmd_vanilla1, envp);
}

void parent_process(char *cmd_vanilla2, int *end, int outfile, char **envp)
{
    wait(0);
    close(end[1]);
    if (dup2(outfile, STDOUT_FILENO) == -1)
    {
        perror("Error dup2 entree 2.1\n");
        return ;
    }
    if (dup2(end[0], STDIN_FILENO) == -1)
    {
        perror("Error dup2 sortie 2.2\n");
        return ;
    }
    //outfile = outfile; //pour avoir retour terminal si comment dup2()
    
    // char *line = init_fullline(outfile);
    // if (line == NULL)
    // {
    //     perror("CAMARCHEPAS\n");
    //     return ;
    // }
    // printf("got from child process ==%s\n", line); 
    // free(line);
    exec_cmd(cmd_vanilla2, envp); 
}

//? Permet de free entierement un char**
void	ft_free_tabtab(char **tabtab)
{
	int	i;

	i = 0;
	while (tabtab[i])
		free(tabtab[i++]);
	free(tabtab);
	tabtab = NULL;
}

int main(int argc, char **argv, char **envp)
{
    int infile;
    int outfile;
    
    if (argc != 5)
        exit(EXIT_FAILURE);
    infile = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, 0777); //! pk 0777 ???
    if (access(argv[1], R_OK | F_OK) == -1)
    {
        perror("Error no permission infile\n");
        exit(EXIT_FAILURE);
    }
    outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777); //!?O_CREAT == creer si inexistant | O_TRUNC == ecrase si deja exisant
    if (access(argv[argc - 1], W_OK | F_OK) == -1) 
    {
        close(infile);
        perror("Error no permission outfile\n");
        exit(EXIT_FAILURE);
    }
    if (argv[2][0] == '\0' || argv[3][0] == '\0')
    {
        close(infile);
        close(outfile);
        exit(EXIT_FAILURE);
    }
    pipex(argv[2], argv[3], infile, outfile, envp);
    //dup2(STDOUT_FILENO, outfile); //balek ???
    
    close(infile);
    close(outfile);
    return (0);
}

//* https://github.com/michmos/42_pipex_tester
//* LEAKS && CLOSE == OKOK  |   Qlq pbl avc testeur inexistant avc test perso 
//* TODOLIST : norminette + gestion_errors.c + remplacer perror() avec strerror() 

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

// void pipex(char *cmd_vanilla1, char *cmd_vanilla2, int infile, int outfile, char** envp)
// {   
//     int end[2];
//     if (pipe(end) == -1)
//     {
//         perror("Error pipex\n");
//         return ;
//     }
//     int id;
//     id = fork();
//     //printf("ID ==%d\n", id); //! a supp
//     if (id == -1)
//     {
//         perror("Error fork\n");
//         return ;
//     }
//     if (id == 0)
//         child_process(cmd_vanilla1, end, infile, envp);
//     else 
//         parent_process(cmd_vanilla2, end, outfile, envp);
// }

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
//         ft_strlcpy(all_cmds[i], argv[current_argv], ft_strlen(argv[current_argv]) + 1); //? rajout +1
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
