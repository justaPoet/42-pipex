/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:20:49 by apoet             #+#    #+#             */
/*   Updated: 2024/06/27 19:24:31 by febouana         ###   ########.fr       */
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
