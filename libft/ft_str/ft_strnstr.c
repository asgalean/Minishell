/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:05:42 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	lenlittle;
	size_t	start;

	if (!little || !*little)
		return ((char *) big);
	lenlittle = ft_strlen(little);
	if (len < lenlittle)
		return (NULL);
	start = 0;
	while (big[start] && start <= len - lenlittle)
	{
		i = 0;
		while (i < lenlittle)
		{
			if (big[start + i] != little[i])
				break ;
			i++;
		}
		if (i == lenlittle)
			return ((char *)(big + start));
		start++;
	}
	return (NULL);
}
