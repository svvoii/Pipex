/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:29:45 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/10 13:56:15 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	else if ((*s1 != *s2) || !*s1)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	return (ft_strncmp(++s1, ++s2, --n));
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		l_len;

	l_len = ft_strlen(little);
	i = 0;
	if (len == 0 && (!big || !little))
		return (NULL);
	if (!*little)
		return ((char *)big);
	while (big[i] && (l_len + i) <= len)
	{
		if (ft_strncmp(big + i, little, l_len) == 0)
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}
