/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/06/27 19:50:54 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

//? Permet de recuperer tout le contenu d'un fichier dans un char*. 
char *init_fullline(int fds)
{
    char *line;
    char *tmp;

    line = "";
    while ((tmp = get_next_line(fds)))
    {
        if (tmp == NULL)
            break ;
        line = ft_strjoin(line, tmp);
        tmp = NULL;
        free(tmp); 
    }
    return (line);
}

//? Permet de verifier si file1 et file2 existent et s'ils sont uniques
//! rajouter possibilite infile/outfile ??
void verif_files(int argc, char **argv)
{
    int search_file1 = 0;
    int search_file2 = 0;   
    int i = 1; 
    
    while (i < argc)
    {
        if (ft_strncmp("file1", argv[i], 5) == 0)
        {
            if (search_file1 != 0)
                break ;
            search_file1++;
        }
        if (ft_strncmp("file2", argv[i], 5) == 0)
        {
            if (search_file2 != 0)
                break ;
            search_file2++;
        }
        i++;
    }
    if (search_file1 + search_file2 == 2)
        return ;
    else
    {
        printf("No valid arguments\n");
        exit(1);
    }
}

//? Permet de renvoyer la ligne de cmd separee(s) de son/ses flags.
char **parse_cmd(char *args)
{
    if (!args)
        exit(1);
    return (ft_split(args, ' '));
}

//? Permet d'executer n'importe quelle cmd avec ses flags, 
//? necessite au prealable une string contenant la cmd voulue.
//! rajouter secu si cmd introuvable
void exec_cmd(char *cmd_flags)
{
    char **cmd_args = parse_cmd(cmd_flags);
    if (!cmd_args) //secu split
    {
        printf("Error ft_split\n");
        exit(1);
    }
    char *cmd = ft_strjoin("/usr/bin/", cmd_args[1]);
    char *args[] = {cmd, cmd_args[2], NULL};
    if (execve(cmd, args, NULL) == -1)
    {
        printf("Error execve\n");
        exit(1);
    }    
    return ;
}

int pipex(char *cmd_vanilla)
{    
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Error pipex\n");
        return(0) ;
    }
    int id;
    id = fork();
    //printf("ID ==%d\n", id);
    if (id == -1)
    {
        printf("Error fork\n");
        return(0) ;
    }
    if (id == 0) //? Child process
    {
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            printf("Error dup2\n");
            return (0);
        }
        exec_cmd(cmd_vanilla);
        close(fd[1]);
    }
    else //? parent process
    {
        wait(0); // sert a rien ici mais juste pour tester
        close(fd[1]);
        char *line2 = init_fullline(fd[0]);
        if (line2 == NULL)
        {
            printf("CAMARCHEPAS\n");
            return (0);
        }
        close(fd[0]);
        printf("got from child process ==%s\n", line2); 
        free(line2);
    }
    return (0);
}

//! RESULTAT VOULU
//! ./pipex infile "ls -l" "wc -l" outfile
//!        < infile ls -l | wc -l > outfile

//? Permet de regrouper dans un tableau de tableau toutes 
//? les cmds passees en arguments, plus qu'a defiler .
char **compil_cmds(int argc, char** argv)
{
    char **all_cmds;
    int current_argv;
    int i;
    all_cmds = malloc((argc - 3) * sizeof(char*));
    current_argv = 2;    
    i = 0;
    while (current_argv < (argc - 1))
    {
        all_cmds[i] = malloc((ft_strlen(argv[current_argv])) * sizeof(char));
        ft_strlcat(all_cmds[i], argv[current_argv], ft_strlen(argv[current_argv]));
        i++;
        current_argv++;
    }
    return (all_cmds);
}

int main(int argc, char **argv)
{
    if (argc < 4)
        exit(1);
    
    verif_files(argc, argv);
    
    char **all_cmds = compil_cmds(argc, argv);

    
    //pipex(argv[2]);

    
    printf("FINI\n");
    return (0);
}

//* comprendre et adapter pipex() pour bien faire le pont entre deux cmds
//* l'adapter pour repondre a autant de cmd qu'on veut  

//! BELEK (bonus) ==> cas avec here_doc ou le second arg n'est pas une cmd




//? gadget, pas tres util ?
//int cmds_counter(int limit, char **args)
// {
//     int counter;
//     int i;
//     counter = 0;
//     i = 1;
//     while (i < limit)
//     {
//         if (ft_strncmp("file1", args[i], 5) != 0 || ft_strncmp("file2", args[i], 5) != 0 || ft_strncmp("here_doc", args[i], 8) != 0)
//             counter++;
//         i++;
//     }
//     return (counter)
// }