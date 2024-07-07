/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:20:49 by apoet             #+#    #+#             */
/*   Updated: 2024/07/07 23:08:35 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

//? Permet de free entierement un tableau de tableau.
void	ft_free_tabtab(char **tabtab)
{
	int	i;

	i = 0;
	while (tabtab[i])
		free(tabtab[i++]);
	free(tabtab);
	tabtab = NULL;
}

size_t	ft_strlcpy(char *dest, const char *src, size_t dest_size)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (src_len + 1 < dest_size)
		ft_memcpy(dest, src, src_len + 1);
	else if (dest_size != 0)
	{
		ft_memcpy(dest, src, dest_size - 1);
		dest[dest_size - 1] = 0;
	}
	return (src_len);
}

void	*ft_memcpy(void *dest, const void *src, size_t len)
{
	char	*char_dest;
	char	*char_src;
	size_t	i;

	if (dest == (void *)0 && src == (void *)0)
		return (dest);
	char_dest = (char *)dest;
	char_src = (char *)src;
	i = 0;
	while (i < len)
	{
		char_dest[i] = char_src[i];
		i++;
	}
	return (dest);
}
