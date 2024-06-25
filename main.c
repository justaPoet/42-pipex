/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:24:58 by apoet             #+#    #+#             */
/*   Updated: 2024/06/25 12:12:14 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//! ./pipex infile "ls -l" "wc -l" outfile
//!        < infile ls -l | wc -l > outfile

#include <stdio.h>
#include "get_next_line.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// void main(int argc, char **argv)
// {
//     int fd[2];
//     if (pipe(fd) == -1)
//     {
//         printf("Error pipex\n");
//         return ;
//     }
//     int id;
//     id = fork();
//     printf("ID == %d\n", id);
//     if (id == -1)
//     {
//         printf("Error fork\n");
//         return ;
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
//             return ;
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
//             return ;
//         } 
//         close(fd[0]);
//         printf("got from child process == %d\n", y);
//     }
// }

void main(int argc, char **argv)
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Error pipex\n");
        return ;
    }
    int id;
    id = fork();
    printf("ID == %d\n", id);
    if (id == -1)
    {
        printf("Error fork\n");
        return ;
    }
    if (id == 0) //? child process
    {
        close(fd[0]);
        char *line;

        int fds = open("infile.txt", O_RDONLY);
        read(fds, &line, sizeof(char*));
        printf("===== %s =====\n", &line);
        if (write(fd[1], &line, sizeof(char*)) == -1)
        {
            printf("Error write\n");
            return ;
        }   
        close(fd[1]);
    }
    else //? parent process
    {
        close(fd[1]);
        char *line2 = get_next_line(fd[0]);
        

        close(fd[0]);
        printf("got from child process == %s\n", line2);
        free(line2);
    }
}
