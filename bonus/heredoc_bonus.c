/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:03:39 by apoet             #+#    #+#             */
/*   Updated: 2024/07/17 19:37:57 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex_bonus.h"

void print_heredoc(char *limiter, int *end, t_pipex data)
{
	char *line;
	
	close(end[0]);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			close(end[1]); //!
			close(data.outfile); //!
			exit(0);
		}
		ft_putstr_fd(line, end[1]);
		free(line);
	}	
}

void	heredoc(char **argv, t_pipex data)
{
	int	end[2];
	int	id;

	if (pipe(end) == -1)
		gestion_error_close_free(111, data);
	id = fork();
	if (id == -1)
		gestion_error_close(22, data);
	if (id == 0)
		print_heredoc(argv[2], end, data);
	else
	{
		close(end[1]);
		if (dup2(end[0], STDIN_FILENO) == -1)
			gestion_error_close(33, data);
		close(end[0]); //!
		wait(NULL);
	}
}

t_Bool search_heredoc(int argc, char **argv)
{	
	if (ft_strncmp("here_doc", argv[1], 8) == 0)
	{	
		if (argc < 6)
			gestion_error(3);
		return (true);
	}	
	return (false);
}
