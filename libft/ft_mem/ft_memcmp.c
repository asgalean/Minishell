/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:32:40 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;
	size_t				i;

	ptr1 = (const unsigned char *)s1;
	ptr2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (ptr1[i] != ptr2[i])
			return (ptr1[i] - ptr2[i]);
		i++;
	}
	return (0);
}

/*#include <stdio.h>

int	main(void)
{
	const char s1[] = "abcdef";
	const char s2[] = "abcdeg";
	const char s3[] = "abcdef";

	// Caso 1: Comparar s1 y s2 (deben ser diferentes)
	if (ft_memcmp(s1, s2, 6) == 0)
		printf("Caso 1 - Las cadenas son iguales.\n");
	else
		printf("Caso 1 - Las cadenas son diferentes.\n");

	// Caso 2: Comparar s1 y s3 (deben ser iguales)
	if (ft_memcmp(s1, s3, 6) == 0)
		printf("Caso 2 - Las cadenas son iguales.\n");
	else
		printf("Caso 2 - Las cadenas son diferentes.\n");

	// Caso 3: Comparar solo los primeros 3 bytes de s1 y s2 (deben ser iguales)
	if (ft_memcmp(s1, s2, 3) == 0)
		printf("Caso 3 - Las cadenas son iguales en los primeros 3 bytes.\n");
	else
		printf("Caso 3 - Las cadenas son diferentes en los primeros 3 bytes.\n");

	return (0);
}*/