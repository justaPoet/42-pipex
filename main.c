/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/06/26 13:31:38 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "get_next_line.h"

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//! TEST PIPE CHAR*
// int main(int argc, char **argv)
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//     {
//         printf("Error pipex\n");
//         return(0) ;
//     }
//     int id;
//     id = fork();
//     printf("ID == %d\n", id);
//     if (id == -1)
//     {
//         printf("Error fork\n");
//         return(0) ;
//     }
//     if (id == 0) //? child process
//     {
//         close(fd[0]);
//         char *line;

//         int fds = open("infile.txt", O_RDWR);
//         line = get_next_line(fds);
//         printf("LINE first == %s", line);
//         if (write(fd[1], line, ft_strlen(line)) == -1)
//         {
//             printf("Error write\n");
//             return(0) ;
//         }   
//         printf("LINE second == %s", line);
//         close(fd[1]);
//         free(line);
//     }
//     else //? parent process
//     {
//         wait(0); //? sert a ren ici mais juste pour tester
//         close(fd[1]);
//         char *line2 = get_next_line(fd[0]);
//         if (line2 == NULL)
//         {
//             printf("CAMARCHEPAS\n");
//             return (0);
//         }
//         close(fd[0]);
//         printf("got from child process == %s\n", line2);
//         free(line2);
//     }
//     return(0) ;
// }

//! fn plus longue pour r, mm resultat
// char *init_fullline(int fds)
// {
//     char *line;
//     char *tmp;
//     char *tmp_line;
    
//     line = "";
//     while (tmp = get_next_line(fds))
//     {
//         if (tmp == NULL)
//             break ;
//         tmp_line = ft_strjoin(line, tmp);
//         line = tmp_line;
//         tmp_line = NULL;
//         free(tmp_line);
//         tmp = NULL;
//         free(tmp); 
//     }   
//     return (line);
// }

char *init_fullline(int fds)
{
    char *line;
    char *tmp;

    line = "";
    while (tmp = get_next_line(fds))
    {
        if (tmp == NULL)
            break ;
        line = ft_strjoin(line, tmp);
        tmp = NULL;
        free(tmp); 
    }
    return (line);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

//? Permet de verifier si file1 et file2 existent et s'ils sont uniques
void verif_files(int argc, char **argv)
{
    int search_file1 = 0;
    int search_file2 = 0;   
    int i = 0; 
    
    while (i++ < argc)
    {
        if (ft_strncmp("file1", argv[i], 4) == 0)
        {
            if (search_file1 > 0)
                break ;
            search_file1++;
        }
        if (ft_strncmp("file2", argv[i], 4) == 0)
        {
            if (search_file2 > 0)
                break ;
            search_file2++;
        }
    }
    if (search_file1 + search_file2 != 2)
        exit(1);
}


//! ./pipex infile "ls -l" "wc -l" outfile
//!        < infile ls -l | wc -l > outfile

//! TEST AVEC EXECVE()
int main(int argc, char **argv)
{
    if (argc < 5)
        exit(1);
    verif_files(argc, argv);
        
    //pipex();
    
    printf("FINI\n");
    return (0);
}

int pipex()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Error pipex\n");
        return(0) ;
    }
    int id;
    id = fork();
    printf("ID == %d\n", id);
    if (id == -1)
    {
        printf("Error fork\n");
        return(0) ;
    }
    if (id == 0) //? child process
    {
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            printf("Error dup2\n");
            return (0);
        }
        char *args[] = {"/usr/bin/ls", "-l", NULL};
        if (execve(args[0], args, NULL) == -1)
        {
            printf("Error execve\n");
            return (0);
        }
        close(fd[1]);
    }
    else //? parent process
    {
        wait(0); //! sert a rien ici mais juste pour tester
        close(fd[1]);
        char *line2 = init_fullline(fd[0]);
        //char *line2 = get_next_line(fd[0]);
        if (line2 == NULL)
        {
            printf("CAMARCHEPAS\n");
            return (0);
        }
        close(fd[0]);
        printf("got from child process ==\n%s\n", line2); 
        free(line2);
    }
    return (0);
}

//* gerer les arguments en main
//* trouver un moyen de reconnaitre et exec tout type de cmd donnee en args
//? ==> execve() avec pwd ?? OU strjoin("usr/bin", NOM_CMD) ??