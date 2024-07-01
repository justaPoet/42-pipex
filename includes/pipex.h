/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:19:28 by febouana          #+#    #+#             */
/*   Updated: 2024/07/01 19:19:31 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdio.h>
# define BUFFER_SIZE 42
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum Bool
{
	false,
	true
}		t_Bool;

// AUDD gnl
size_t	ft_strlen(const char *str);
char	*get_next_line(int fd);

//AUDD gnl_utils
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strchr(const char *str, int c);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);

// AUDD split
int		count_words(char *str, char separator);
char	*get_next_word(char *str, char separator);
char	**ft_split(char *str, char separator);

// AUDD pipex_utils
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlcat(char *dest, const char *src, size_t dest_size);

// AUDD main
int		pipex(char *cmd_vanilla1, char *cmd_vanilla2, int file1, int file2, char** envp);
char	*init_fullline(int fds);
void	verif_files(int argc, char **argv);

char	**parse_cmd(char *args);
void	exec_cmd(char *argv, char** envp);

#endif