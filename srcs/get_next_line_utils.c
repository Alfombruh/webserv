/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jofernan <jofernan@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 01:24:37 by jofernan          #+#    #+#             */
/*   Updated: 2022/05/24 01:24:45 by jofernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_gnl_strlen(char	*s)
{
	size_t	c;

	c = 0;
	if (!s)
		return (0);
	while (s[c] != '\0')
	{
		c++;
	}
	return (c);
}

char	*ft_gnl_strjoin(char *cache, char *buff)
{
	size_t	pos;
	size_t	pos2;
	char	*str;

	if (!cache)
	{
		cache = (char *)malloc(1 * sizeof(char));
		cache[0] = '\0';
	}
	if (!cache || !buff)
		return (NULL);
	str = malloc(sizeof(char) * ((ft_gnl_strlen(cache)
					+ ft_gnl_strlen(buff)) + 1));
	if (str == NULL)
		return (NULL);
	pos = -1;
	pos2 = 0;
	if (cache)
		while (cache[++pos] != '\0')
			str[pos] = cache[pos];
	while (buff[pos2] != '\0')
		str[pos++] = buff[pos2++];
	str[ft_gnl_strlen(cache) + ft_gnl_strlen(buff)] = '\0';
	free(cache);
	return (str);
}

char	*ft_line(char *cache)
{
	int		pos;
	char	*str;

	pos = 0;
	if (!cache[pos])
		return (NULL);
	while (cache[pos] && cache[pos] != '\n')
		pos++;
	str = (char *)malloc(sizeof(char) * (pos + 2));
	if (!str)
		return (NULL);
	pos = 0;
	while (cache[pos] && cache[pos] != '\n')
	{
		str[pos] = cache[pos];
		pos++;
	}
	if (cache[pos] == '\n')
	{
		str[pos] = cache[pos];
		pos++;
	}
	str[pos] = '\0';
	return (str);
}

char	*ft_newcache(char *cache)
{
	int		pos;
	int		pos2;
	char	*str;

	pos = 0;
	while (cache[pos] && cache[pos] != '\n')
		pos++;
	if (!cache[pos])
	{
		free(cache);
		return (NULL);
	}
	str = (char *)malloc(sizeof(char) * (ft_gnl_strlen(cache) - pos + 1));
	if (!str)
		return (NULL);
	pos++;
	pos2 = 0;
	while (cache[pos])
		str[pos2++] = cache[pos++];
	str[pos2] = '\0';
	free(cache);
	return (str);
}

char	*ft_gnl_strchr(char *s, int c)
{
	int	pos;

	pos = 0;
	if (!s)
		return (0);
	if (c == '\0')
		return ((char *)&s[ft_gnl_strlen(s)]);
	while (s[pos] != '\0')
	{
		if (s[pos] == (char) c)
			return ((char *)&s[pos]);
		pos++;
	}
	return (0);
}
