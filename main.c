/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/06/25 20:04:33 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//! ./pipex infile "ls -l" "wc -l" outfile
//!        < infile ls -l | wc -l > outfile

#include <stdio.h>
#include "get_next_line.h"

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//! TEST PIPE INT
// int main(int argc, char **argv)
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//     {
//         printf("Error pipex\n");
//         return (0);
//     }
//     int id;
//     id = fork();
//     printf("ID == %d\n", id);
//     if (id == -1)
//     {
//         printf("Error fork\n");
//         return (0);
//     }
//     if (id == 0) //? child process
//     {
//         close(fd[0]);
//         int x;
//         printf("input nbr : ");
//         scanf("%d", &x);
//         if (write(fd[1], &x, sizeof(int)) == -1)
//         {
//             printf("Error write\n");
//             return (0);
//         }    
//         close(fd[1]);
//     }
//     else //? parent process
//     {
//         close(fd[1]);
//         int y;
//         if (read(fd[0], &y, sizeof(int)) == -1)
//         {
//             printf("Error read\n");
//             return (0);
//         } 
//         close(fd[0]);
//         printf("got from child process == %d\n", y);
//     }
//     return (0);
// }

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

//! TEST AVEC EXECVE()
int main(int argc, char **argv)
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
        char *line;

        int fds = open("infile.txt", O_RDWR);
        line = get_next_line(fds);
        printf("LINE first == %s", line);


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
        printf("LINE sqecond == %s", line);
        close(fd[1]);
        free(line);
    }
    else //? parent process
    {
        wait(0); //! sert a rien ici mais juste pour tester
        close(fd[1]);
        char *line2 = get_next_line(fd[0]);
        if (line2 == NULL)
        {
            printf("CAMARCHEPAS\n");
            return (0);
        }
        close(fd[0]);
        printf("got from child process == %s\n", line2); 
        free(line2);
    }
    return(0) ;
}
//* FONCTIONNE ==> la premiere ligne de "ls -l" s'affiche
//* manque plus qu'une boucle  

//* rajouter fn init_tabtab()
//* print_tabtab()
