/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:19:28 by febouana          #+#    #+#             */
/*   Updated: 2024/07/17 19:12:43 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# define BUFFER_SIZE 42
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum Bool
{
	false,
	true
}			t_Bool;

typedef struct Data
{
	int		infile;
	int		outfile;
	char	**envp;
	char	**cmd_split;
	char	*path;
}			t_pipex;

//+ pipex.c
void	exec_cmd(char *cmd_with_flags, t_pipex data);
void	child_process(char *cmd_vanilla1, int *end, t_pipex data);
void	parent_process(char *cmd_vanilla2, int *end, t_pipex data);
void	pipex(char *cmd_vanilla1, char *cmd_vanilla2, t_pipex data);

//+ pipex_utils.c
void	ft_free_tabtab(char **tabtab);
size_t	ft_strlcpy(char *dest, const char *src, size_t dest_size);
void	*ft_memcpy(void *dest, const void *src, size_t len);

//+ gestion_error.c
void	gestion_error(int type);
void	gestion_error_close(int type, t_pipex data);
void	gestion_error_close_free(int type, t_pipex data);

//+ get_next_line.c
size_t		ft_strlen(const char *str);
char		*get_next_line(int fd);

//+ get_next_line_utils.c
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strchr(const char *str, int c);
char		*ft_strjoin(char const *s1, char const *s2);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(const char *s);

//+ ft_split.c
int			count_words(char const *s, char c);
void		ft_free_tabtab(char **tab);
char		*ft_str(char const *s, char c);
char		**ft_split(char const *s, char c);

#endif
