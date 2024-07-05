/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apoet <apoet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:20:49 by apoet             #+#    #+#             */
/*   Updated: 2024/07/04 16:14:27 by apoet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

// ! plus besoin a supp ??
size_t	ft_strlcat(char *dest, const char *src, size_t dest_size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((!dest || !src) && (dest_size == 0))
		return (ft_strlen(""));
	while ((dest[i] != '\0') && (i < dest_size))
		i++;
	while ((src[j]) && (i + j + 1) < dest_size)
	{
		dest[i + j] = src[j];
		j++;
	}
	if (i < dest_size)
		dest[i + j] = '\0';
	return (i + ft_strlen(src));
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
