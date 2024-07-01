/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/01 19:32:20 by febouana         ###   ########.fr       */
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
        perror("No valid arguments\n");
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
//! rajouter secu si cmd introuvable //access()
//! rajouter boucle au cas ou plusieurs flags (possible ?)
void exec_cmd(char *cmd_with_flags, char** envp)
{
    char **cmd_args = parse_cmd(cmd_with_flags);
    if (!cmd_args) //secu split
    {
        perror("Error ft_split\n");
        exit(1);
    }

    // int i = 0;
    // while (cmd_args[i++] != NULL)
    //     printf("cmd_args == %s\n", cmd_args[i]);

    char *cmd = ft_strjoin("/usr/bin/", cmd_args[1]);
    if (access(cmd, X_OK) == -1)
    {
        perror("WRONG WAY");
        exit(1);
    }
    char *args[] = {cmd, cmd_args[2], NULL};
    
    // i = -1;
    // while (args[i++] != NULL)
    //     printf("args == %s\n", args[i]);
    if (execve(cmd, args, envp) == -1)
    {
        perror("Error execve");
        exit(1);
    }    
    return ;
}

int pipex(char *cmd_vanilla1, char *cmd_vanilla2, int file1, int file2, char** envp)
{   
    int end[2];
    if (pipe(end) == -1)
    {
        perror("Error pipex\n");
        return(0) ;
    }
    int id;
    id = fork();
    printf("ID ==%d\n", id);
    if (id == -1)
    {
        perror("Error fork\n");
        return(0) ;
    }
    if (id == 0) //? Child process
    {
        if (dup2(end[1], STDOUT_FILENO) == -1)
        {
            perror("Error dup2 sortie\n");
            return (0);
        }
        if (dup2(file1, STDIN_FILENO) == -1)
        {
            perror("Error dup2 entree\n");
            return (0);
        }
        close(end[0]);
        exec_cmd(cmd_vanilla1, envp);
    }
    
    else //? parent process
    {
        wait(0); // sert a rien ici mais juste pour tester
        // int status;
        // waitpid(id, &status, 0);

        if (dup2(file2, STDOUT_FILENO) == -1)
        {
            perror("Error dup2 entree 2.1\n");
            return (0);
        }
        if (dup2(end[0], STDIN_FILENO) == -1)
        {
            perror("Error dup2 sortie 2.2\n");
            return (0);
        }
        
        char *line = init_fullline(file2);
        if (line == NULL)
        {
            perror("CAMARCHEPAS\n");
            return (0);
        }
        close(end[1]);
        exec_cmd(cmd_vanilla2, envp);
        
        printf("got from child process ==%s\n", line); 
        free(line);
    }
    return (0);
}

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

//! RESULTAT VOULU
//! ./pipex infile "ls -l" "wc -l" outfile
//!        < infile ls -l | wc -l > outfile

int main(int argc, char **argv, char **envp)
{
    if (argc < 4)
        exit(1);
    
    verif_files(argc, argv);
    
    // char **all_cmds = compil_cmds(argc, argv); // a utiliser pour nvl fn pipex
    int file1 = open(argv[1], O_RDONLY, 0777);
    int file2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);

    pipex(argv[2], argv[3], file1, file2, envp);
    close(file1);
    close(file2);

    printf("FINI\n");
    return (0);
}
//* corriger version mandatory 
//* prevoir un cas d'erreur si pas les droits sur file1/file2 comme so_long
//* (bonus) revoir chaque cas ou argv[x] est utiliser et le modifier
//* (bonus) l'adapter pour repondre a autant de cmd qu'on veut (rajouter boucle) 
//* (bonus) cas avec here_doc ou le second arg n'est pas une cmd

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
