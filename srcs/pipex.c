/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/07/02 16:28:57 by apoet            ###   ########.fr       */
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

//? Permet de verifier si infile et outfile existent et s'ils sont uniques
//! rajouter possibilite infile/outfile ??
void verif_files(int argc, char **argv)
{
    int search_infile = 0;
    int search_outfile = 0;   
    int i = 1; 
    
    while (i < argc)
    {
        if (ft_strncmp("infile", argv[i], 5) == 0)
        {
            if (search_infile != 0)
                break ;
            search_infile++;
        }
        if (ft_strncmp("outfile", argv[i], 5) == 0)
        {
            if (search_outfile != 0)
                break ;
            if (search_infile == 0)
                search_outfile = INT_MAX;
            search_outfile++;
        }
        i++;
    }
    if (search_infile + search_outfile != 2)
    {
        perror("INVALID ARGUMENTS\n");
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
    if (access(cmd, X_OK) == -1) //? secu fonctionne bien 
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

// int pipex(char *cmd_vanilla1, char *cmd_vanilla2, int infile, int outfile, char** envp)
// {   
//     int end[2];
//     if (pipe(end) == -1)
//     {
//         perror("Error pipex\n");
//         return(0) ;
//     }
//     int id;
//     id = fork();
//     printf("ID ==%d\n", id);
//     if (id == -1)
//     {
//         perror("Error fork\n");
//         return(0) ;
//     }
//     if (id == 0) //? Child process
//     {
//         if (dup2(end[1], STDOUT_FILENO) == -1)
//         {
//             perror("Error dup2 sortie\n");
//             return (0);
//         }
//         if (dup2(infile, STDIN_FILENO) == -1)
//         {
//             perror("Error dup2 entree\n");
//             return (0);
//         }
//         close(end[0]);
//         exec_cmd(cmd_vanilla1, envp);
//     }
    
//     else //? parent process
//     {
//         wait(0); // sert a rien ici mais juste pour tester
//         // int status;
//         // waitpid(id, &status, 0);

//         if (dup2(outfile, STDOUT_FILENO) == -1)
//         {
//             perror("Error dup2 entree 2.1\n");
//             return (0);
//         }
//         if (dup2(end[0], STDIN_FILENO) == -1)
//         {
//             perror("Error dup2 sortie 2.2\n");
//             return (0);
//         }
        
//         char *line = init_fullline(outfile);
//         if (line == NULL)
//         {
//             perror("CAMARCHEPAS\n");
//             return (0);
//         }
//         close(end[1]);
//         exec_cmd(cmd_vanilla2, envp);
        
//         printf("got from child process ==%s\n", line); 
//         free(line);
//     }
//     return (0);
// }

void child_process(char *cmd_vanilla1, int *end, int infile, char **envp)
{
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
    close(end[0]);
    exec_cmd(cmd_vanilla1, envp);
}

void parent_process(char *cmd_vanilla2, int *end, int outfile, char **envp)
{
        wait(0);
        // int status;
        // waitpid(id, &status, 0);

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
        
        char *line = init_fullline(outfile);
        if (line == NULL)
        {
            perror("CAMARCHEPAS\n");
            return ;
        }
        close(end[1]);
        exec_cmd(cmd_vanilla2, envp);
        
        printf("got from child process ==%s\n", line); 
        free(line);
}

int pipex(char *cmd_vanilla1, char *cmd_vanilla2, int infile, int outfile, char** envp)
{   
    int end[2];
    if (pipe(end) == -1)
    {
        perror("Error pipex\n");
        return(0) ;
    }
    int id;
    id = fork();
    printf("ID ==%d\n", id); //! a supp
    if (id == -1)
    {
        perror("Error fork\n");
        return(0) ;
    }
    if (id == 0)
        child_process(cmd_vanilla1, end, infile, envp);
    else 
        parent_process(cmd_vanilla2, end, outfile, envp);
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
        ft_strlcat(all_cmds[i], argv[current_argv], ft_strlen(argv[current_argv]) + 1); //? rajout +1
        i++;
        current_argv++;
    }
    return (all_cmds);
}

//! RESULTAT VOULU
//! ./pipex infile "ls -l" "wc -l" outfile
//!        < infile ls -l | wc -l > outfile

// verif argc
// verif infile/outfile (pas here_doc)
//

//? Permet de free entierement un char**
void	ft_free_tabtab(char **tabtab) //todo FONCTIONNE
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
    char **all_cmds;
    
    if (argc < 4)
        exit(1);
    verif_files(argc, argv); //! modifs pour here_doc plus tard
    if (access(argv[1], R_OK) == -1)
    {
        perror("infile MARCHE PAS");
        return (0);
    }
    infile = open(argv[1], O_RDONLY);
    if (access(argv[argc - 1], W_OK) == -1) 
    {
        perror("outfile MARCHE PAS");
        return (0);
    }
    outfile = open(argv[argc - 1], O_WRONLY);    
    

    
    all_cmds = compil_cmds(argc, argv); // a utiliser pour nvl fn pipex

    pipex(all_cmds[0], all_cmds[1], infile, outfile, envp);

    
    

    dup2(STDOUT_FILENO, outfile);

    close(infile);
    close(outfile);
    
    ft_free_tabtab(all_cmds);

    printf("FINI\n");
    return (0);
}

//* (bonus) faire fonctionner pipex avec n'importe quel nbr de cmd 
//* (bonus) revoir chaque cas ou argv[x] est utiliser et le modifier
//* (bonus) l'adapter pour repondre a autant de cmd qu'on veut (rajouter boucle) 
//* (bonus) cas avec here_doc ou le second arg n'est pas une cmd
//* (endgame) verifier que les exit soit sur la bonne sortie 


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
